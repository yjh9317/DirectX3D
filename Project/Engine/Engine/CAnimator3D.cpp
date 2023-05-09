#include "pch.h"

#include "CAnimator3D.h"

#include "CTimeMgr.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CResMgr.h"

#include "CAnimation3DShader.h"

#include "CKeyMgr.h"
#include "CAnimation3D.h"


CAnimator3D::CAnimator3D()
	: m_pVecBones(nullptr)
	, m_pVecClip(nullptr)
	, m_pBoneFinalMatBuffer(nullptr)
	, m_pSocketBuffer(nullptr)
	, m_bFinalMatUpdate(false)
	, m_bPlay(true)
	, m_bRepeat(true)
	, m_BoneIndex(0)
	, m_BoneIndex2(0)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
	m_pSocketBuffer = new CStructuredBuffer;
	m_pSocketBuffer2 = new CStructuredBuffer;
}

CAnimator3D::CAnimator3D(const CAnimator3D& _origin)
	: m_pVecBones(_origin.m_pVecBones)
	, m_pVecClip(_origin.m_pVecClip)
	, m_pBoneFinalMatBuffer(nullptr)
	, m_pSocketBuffer(nullptr)
	, m_bPlay(true)
	, m_bRepeat(true)
	, m_bFinalMatUpdate(false)
	, m_BoneIndex(_origin.m_BoneIndex)
	, m_BoneIndex2(_origin.m_BoneIndex2)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
{
	m_pBoneFinalMatBuffer = new CStructuredBuffer;
	m_pSocketBuffer = new CStructuredBuffer;
	m_pSocketBuffer2 = new CStructuredBuffer;

	for (auto& pair : _origin.m_mapAnim)
	{
		CAnimation3D* pCloneAnim = pair.second->Clone();
		m_mapAnim.insert(make_pair(pair.first, pCloneAnim));
		pCloneAnim->m_pOwner = this;
	}

	if (nullptr != _origin.m_pCurAnim)
	{
		m_pCurAnim = FindAnim(_origin.m_pCurAnim->GetName());
	}
	else
	{
		m_pCurAnim = m_mapAnim.begin()->second;
	}
}

CAnimator3D::~CAnimator3D()
{
	SAFE_DELETE(m_pBoneFinalMatBuffer);
	SAFE_DELETE(m_pSocketBuffer);
	SAFE_DELETE(m_pSocketBuffer2);
	Safe_Del_Map(m_mapAnim);
}


void CAnimator3D::finalupdate()
{
	if (nullptr == m_pCurAnim)
		return;

	if(m_bPlay)
		m_pCurAnim->finalupdate();

	if (m_pCurAnim->IsFinish() && m_bRepeat)
	{
		m_pCurAnim->Reset();
	}

	// 컴퓨트 쉐이더 연산여부
	m_bFinalMatUpdate = false;
	
}

void CAnimator3D::SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip)
{
	m_pVecClip = _vecAnimClip;
	m_vecClipUpdateTime.resize(m_pVecClip->size());

	static float fTime = 0.f;
	fTime += 1.f;
	m_vecClipUpdateTime[0] = fTime;
}


void CAnimator3D::UpdateData()
{
	if (!m_bFinalMatUpdate)
	{
		// Animation3D Update Compute Shader
		CAnimation3DShader* pUpdateShader = (CAnimation3DShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"Animation3DUpdateShader").Get();

		// Bone Data
		Ptr<CMesh> pMesh = MeshRender()->GetMesh();
		check_mesh(pMesh);

		pUpdateShader->SetFrameDataBuffer(pMesh->GetBoneFrameDataBuffer());
		pUpdateShader->SetOffsetMatBuffer(pMesh->GetBoneOffsetBuffer());
		pUpdateShader->SetOutputBuffer(m_pBoneFinalMatBuffer);
		pUpdateShader->SetSocketBuffer(m_pSocketBuffer);
		pUpdateShader->SetSocket2Buffer(m_pSocketBuffer2);

		UINT iBoneCount = (UINT)m_pVecBones->size();
		pUpdateShader->SetBoneCount(iBoneCount);

		if (m_pCurAnim)
		{
			pUpdateShader->SetFrameIndex(m_pCurAnim->GetCurFrameIdx());
			pUpdateShader->SetNextFrameIdx(m_pCurAnim->GetNextFrameIdx());
			pUpdateShader->SetFrameRatio(m_pCurAnim->GetRatio());
			pUpdateShader->SetBoneIdx(m_BoneIndex);
			pUpdateShader->SetBone2Idx(m_BoneIndex2);

			pUpdateShader->SetIsPrevAnim(m_pCurAnim->GetLerp());
			pUpdateShader->SetPrevFrameIdx((float)m_pCurAnim->GetPrevanimFrame());
		}
		else
		{
			pUpdateShader->SetFrameIndex(0);
			pUpdateShader->SetNextFrameIdx(0);
			pUpdateShader->SetFrameRatio(0.f);
			pUpdateShader->SetBoneIdx(m_BoneIndex);
			pUpdateShader->SetBone2Idx(m_BoneIndex2);

			pUpdateShader->SetIsPrevAnim(false);
			pUpdateShader->SetPrevFrameIdx(0);
		}

		// 업데이트 쉐이더 실행
		pUpdateShader->Excute();

		m_bFinalMatUpdate = true;
	}

	// t30 레지스터에 최종행렬 데이터(구조버퍼) 바인딩		
	m_pBoneFinalMatBuffer->UpdateData(PIPELINE_STAGE::VS, 30);

}

void CAnimator3D::ClearData()
{
	m_pBoneFinalMatBuffer->Clear();
	m_pSocketBuffer->Clear();
	m_pSocketBuffer2->Clear();

	UINT iMtrlCount = MeshRender()->GetMtrlCount();
	Ptr<CMaterial> pMtrl = nullptr;
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		pMtrl = MeshRender()->GetSharedMaterial(i);
		if (nullptr == pMtrl)
			continue;

		pMtrl->SetAnim3D(false); // Animation Mesh 알리기
		pMtrl->SetBoneCount(0);
	}
}

void CAnimator3D::check_mesh(Ptr<CMesh> _pMesh)
{
	UINT iBoneCount = _pMesh->GetBoneCount();
	if (m_pBoneFinalMatBuffer->GetElementCount() != iBoneCount)
	{
		m_pBoneFinalMatBuffer->Create(sizeof(Matrix), iBoneCount, SB_TYPE::READ_WRITE, false, nullptr);
		m_pSocketBuffer->Create(sizeof(Matrix), 1, SB_TYPE::READ_WRITE, true, nullptr);
		m_pSocketBuffer2->Create(sizeof(Matrix), 1, SB_TYPE::READ_WRITE, false, nullptr);
	}
}

CAnimation3D* CAnimator3D::CreateAnim(const wstring& _strName, int _ClipIdx, float _fStart, float _fEnd,  int _AnimFrame)
{
	CAnimation3D* pAnim3D = new CAnimation3D;

	if (_ClipIdx >= m_pVecClip->size() || _strName == L"")
		return nullptr;

	pAnim3D->m_pOwner = this;
	pAnim3D->CreateAnim(_strName, _ClipIdx, _fStart, _fEnd, _AnimFrame);

	m_mapAnim.insert(make_pair(_strName, pAnim3D));
	m_pCurAnim = pAnim3D;

	return pAnim3D;
}


CAnimation3D* CAnimator3D::FindAnim(const wstring& _strName)
{
	map<wstring, CAnimation3D*>::iterator iter = m_mapAnim.begin();

	for (; iter != m_mapAnim.end(); ++iter)
	{
		if (_strName == iter->first)
		{
			return iter->second;
		}
	}

	return nullptr;
}

void CAnimator3D::Play(const wstring& _strName, bool _bRepeat)
{
	if (0 == wcscmp(_strName.c_str(), GetCurAnim()->GetAnimClip().strAnimName.c_str()))
		return;

	CAnimation3D* pAnim = FindAnim(_strName);
	assert(pAnim);

	m_bRepeat = _bRepeat;

	// 이전 애니메이션의 마지막 프레임을 현재 애니메이션에 저장하고 리셋
	m_pPrevAnim = m_pCurAnim;
	int PrevAnimFrame = m_pCurAnim->GetCurFrameIdx();
	m_pPrevAnim->Reset();
	
	// 현재 애니메이션한테 보간하기 위한 값세팅 ( bool 보간해야하는지 , int 이전 애니메이션 프레임)
	m_pCurAnim = pAnim;
	m_pCurAnim->SetLerp(true);
	m_pCurAnim->SetPrevAnimFrame(PrevAnimFrame);
}

void CAnimator3D::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);

	size_t iAnimCount = m_mapAnim.size();
	fwrite(&iAnimCount, sizeof(size_t), 1, _pFile);

	map<wstring, CAnimation3D*>::iterator iter = m_mapAnim.begin();
	for (; iter != m_mapAnim.end(); ++iter)
	{
		iter->second->SaveToScene(_pFile);
	}

	wstring strCurAnimName;
	if (nullptr != m_pCurAnim)
		strCurAnimName = m_pCurAnim->GetAnimClip().strAnimName.c_str();

	SaveWStringToFile(strCurAnimName, _pFile);

	fwrite(&m_BoneIndex, sizeof(int), 1, _pFile);
	fwrite(&m_BoneIndex2, sizeof(int), 1, _pFile);
}

void CAnimator3D::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);

	size_t iAnimCount = 0;
	fread(&iAnimCount, sizeof(size_t), 1, _pFile);

	for (size_t i = 0; i < iAnimCount; ++i)
	{
		CAnimation3D* pAnim = new CAnimation3D;
		pAnim->LoadFromScene(_pFile);

		m_mapAnim.insert(make_pair(pAnim->GetAnimClip().strAnimName, pAnim));
		pAnim->m_pOwner = this;
	}

	wstring strCurAnimName;
	LoadWStringFromFile(strCurAnimName, _pFile);
	m_pCurAnim = FindAnim(strCurAnimName);

	fread(&m_BoneIndex, sizeof(int), 1, _pFile);
	fread(&m_BoneIndex2, sizeof(int), 1, _pFile);
}

void CAnimator3D::SaveJsonFile(Json::Value& root)
{
	Json::Value Animator3DInfo;

	Animator3DInfo["AnimCount"] = m_mapAnim.size();

	map<wstring, CAnimation3D*>::iterator iter = m_mapAnim.begin();
	int i = 0;

	for (; iter != m_mapAnim.end(); ++iter)
	{
		string str = "Animation" + std::to_string(i);
		iter->second->SaveJsonFile(Animator3DInfo, str);
		++i;
	}

	wstring wstrCurAnimName;
	if (nullptr != m_pCurAnim)
		wstrCurAnimName = m_pCurAnim->GetAnimClip().strAnimName.c_str();

	Animator3DInfo["CurAnimName"] = string(wstrCurAnimName.begin(), wstrCurAnimName.end());
	Animator3DInfo["FirstBoneIndex"] = m_BoneIndex;
	Animator3DInfo["SecondBoneIndex"] = m_BoneIndex2;

	root["ANIMATOR3D"] = Animator3DInfo;
}

void CAnimator3D::LoadJsonFromFile(Json::Value& root)
{
	Json::Value Animator3DInfo = root;

	int iAnimCount = Animator3DInfo["AnimCount"].asInt();

	for (int i = 0 ; i < iAnimCount; i++)
	{
		CAnimation3D* pAnim = new CAnimation3D;

		string str = "Animation" + std::to_string(i);
		pAnim->LoadJsonFromFile(Animator3DInfo,str);

		m_mapAnim.insert(make_pair(pAnim->GetAnimClip().strAnimName, pAnim));
		pAnim->m_pOwner = this;
	}

	// Find Cur Anim
	if (Animator3DInfo["CurAnimName"].asString() != "")
	{
		string str = Animator3DInfo["CurAnimName"].asString();
		wstring CurAnimName = wstring(str.begin(), str.end());
		m_pCurAnim = FindAnim(CurAnimName);
	}


	// Bone Index
	m_BoneIndex = Animator3DInfo["FirstBoneIndex"].asInt();
	m_BoneIndex2 = Animator3DInfo["SecondBoneIndex"].asInt();
}

int CAnimator3D::Save(const wstring& _strFileName)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += L"animation\\";
	strFilePath += _strFileName;
	strFilePath += L".anim";

	FILE* _pFile = nullptr;

	_wfopen_s(&_pFile, strFilePath.c_str(), L"wb");
	assert(_pFile);

	if (nullptr == _pFile)
		return E_FAIL;

	int iAnimCount = (int)m_mapAnim.size();
	fwrite(&iAnimCount, sizeof(size_t), 1, _pFile);

	map<wstring, CAnimation3D*>::iterator iter = m_mapAnim.begin();
	for (; iter != m_mapAnim.end(); ++iter)
	{
		tMTAnimClip _AnimClip = iter->second->GetAnimClip();
		// 이름 저장
		wstring AnimName = _AnimClip.strAnimName.c_str();
		SaveWStringToFile(AnimName, _pFile);

		// 애니메이션 프레임 저장
		fwrite(&_AnimClip.iStartFrame, sizeof(int), 1, _pFile);
		fwrite(&_AnimClip.iEndFrame, sizeof(int), 1, _pFile);

		// 프레임 카운트 저장
		int _iAnimCount = iter->second->GetFrameConunt();
		fwrite(&_iAnimCount, sizeof(int), 1, _pFile);

		// 클립 인덱스 저장
		int _ClipIdx = iter->second->GetClipIdx();
		fwrite(&_ClipIdx, sizeof(int), 1, _pFile);

		// 보간 시간 저장
		float _LerpTime = iter->second->GetLerpTime();
		fwrite(&_LerpTime, sizeof(float), 1, _pFile);

		float _fSpeed = iter->second->GetSpeed();
		fwrite(&_fSpeed, sizeof(float), 1, _pFile);

	}

	wstring _CurAnimName = m_pCurAnim->m_AnimInfo.strAnimName;
	SaveWStringToFile(_CurAnimName, _pFile);

	fwrite(&m_BoneIndex, sizeof(int), 1, _pFile);
	fwrite(&m_BoneIndex2, sizeof(int), 1, _pFile);

	fclose(_pFile);

	return S_OK;
}

int CAnimator3D::Load(const wstring& _strFileName)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += L"animation\\";
	strFilePath += _strFileName;
	strFilePath += L".anim";

	FILE* _pFile = nullptr;

	_wfopen_s(&_pFile, strFilePath.c_str(), L"rb");
	assert(_pFile);

	if (nullptr == _pFile)
		return E_FAIL;
	
	// 안에 있던 애니메이션 전부 삭제 한 후 로딩
	Safe_Del_Map(m_mapAnim);



	int iAnimCount;
	fread(&iAnimCount, sizeof(size_t), 1, _pFile);

	for (int i = 0; i < iAnimCount; ++i)
	{
		CAnimation3D* pAnim = new CAnimation3D;
	

		wstring _AnimName = {};
		LoadWStringFromFile(_AnimName, _pFile);
		pAnim->m_AnimInfo.strAnimName = _AnimName;
	
		int _iStartFrame = 0; 
		int _iEndFrame = 0;
		int _iFrameCount = 0;
		int _iClipIdx = 0;
		float _LerpTime = 0.f;
		float _fSpeed = 0.f;

		fread(&_iStartFrame, sizeof(int), 1, _pFile);
		fread(&_iEndFrame, sizeof(int), 1, _pFile);
		fread(&_iFrameCount, sizeof(int), 1, _pFile);
		fread(&_iClipIdx, sizeof(int), 1, _pFile);
		fread(&_LerpTime, sizeof(float), 1, _pFile);
		fread(&_fSpeed, sizeof(float), 1, _pFile);


		pAnim->m_AnimInfo.iStartFrame = _iStartFrame;
		pAnim->m_AnimInfo.iEndFrame = _iEndFrame;
		pAnim->m_AnimInfo.iFrameLength = _iEndFrame - _iStartFrame;

		pAnim->m_AnimInfo.dStartTime = _iStartFrame / (double)_iFrameCount;
		pAnim->m_AnimInfo.dEndTime = _iEndFrame / (double)_iFrameCount;
		pAnim->m_AnimInfo.dTimeLength = (_iEndFrame - _iStartFrame) / (double)_iFrameCount;

		pAnim->m_iClipIdx = _iClipIdx;
		pAnim->m_iFrameCount = _iFrameCount;

		pAnim->SetSpeed(_fSpeed);

		pAnim->m_pOwner = this;
		m_mapAnim.insert(make_pair(_AnimName, pAnim));

		pAnim->SetLerpTime(_LerpTime);
	}

	wstring _CurAnimName = {};
	LoadWStringFromFile(_CurAnimName, _pFile);

	m_pCurAnim = m_mapAnim.find(_CurAnimName)->second;

	fread(&m_BoneIndex, sizeof(int), 1, _pFile);
	fread(&m_BoneIndex2, sizeof(int), 1, _pFile);

	fclose(_pFile);

	return S_OK;
}

