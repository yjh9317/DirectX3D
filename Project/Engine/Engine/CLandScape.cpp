#include "pch.h"
#include "CLandScape.h"

#include "CRenderMgr.h"
#include "CKeyMgr.h"

#include "CTransform.h"
#include "CCamera.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CStructuredBuffer.h"


static int ii = 0;

CLandScape::CLandScape()
	: CRenderComponent(COMPONENT_TYPE::LANDSCAPE)
	, m_iXFaceCount(0)
	, m_iZFaceCount(0)
	, m_pCrossBuffer(nullptr)
	, m_pWeightMapBuffer(nullptr)
	, m_iWeightWidth(0)
	, m_iWeightHeight(0)
	, m_iWeightIdx(0)
	, m_vBrushScale(Vec2(0.1f, 0.1f))
	, m_iBrushIdx(0)
	, m_iBrushDir(0)
	, m_fBrushPow(0.2f)
	, m_fBrushMod(0.f)
	, m_eMod(LANDSCAPE_MOD::NONE)
{
	m_pCrossBuffer = new CStructuredBuffer;
	m_pCrossBuffer->Create(sizeof(tRaycastOut), 1, SB_TYPE::READ_WRITE, true, nullptr);
}

CLandScape::~CLandScape()
{
	SAFE_DELETE(m_pCrossBuffer);
	SAFE_DELETE(m_pWeightMapBuffer);
}

void CLandScape::finalupdate()
{

	if (KEY_TAP(KEY::NUM0))
		m_eMod = LANDSCAPE_MOD::NONE;
	else if (KEY_TAP(KEY::NUM1))
		m_eMod = LANDSCAPE_MOD::HEIGHT_MAP;
	else if (KEY_TAP(KEY::NUM2))
		m_eMod = LANDSCAPE_MOD::SPLAT;
	else if (KEY_TAP(KEY::NUM3))
		m_eMod = LANDSCAPE_MOD::SMOOTHING;

	if (LANDSCAPE_MOD::NONE == m_eMod)
	{
		return;
	}

	Raycasting(); 

	// 픽킹 정보를 system memory 로 가져옴
	//m_pCrossBuffer->GetData(&out);

	if (KEY_PRESSED(KEY::LBTN))
	{
		//m_vBrushScale = Vec2(0.1f, 0.1f);

		if (LANDSCAPE_MOD::HEIGHT_MAP == m_eMod || LANDSCAPE_MOD::SMOOTHING == m_eMod)
		{
			if (LANDSCAPE_MOD::HEIGHT_MAP == m_eMod)
				m_pCSHeightMap->SetMod(0.f);
			else if(LANDSCAPE_MOD::SMOOTHING == m_eMod)
				m_pCSHeightMap->SetMod(1.f);
			// 교점 위치정보를 토대로 높이를 수정 함
			LandScape()->GetSharedMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, m_pHeightMap);

			m_pCSHeightMap->SetInputBuffer(m_pCrossBuffer); // 픽킹 정보를 HeightMapShader 에 세팅
			m_pCSHeightMap->SetBrushTex(m_pBrushArrTex);	// 사용할 브러쉬 텍스쳐 세팅
			m_pCSHeightMap->SetBrushIndex(m_iBrushIdx);		// 브러쉬 인덱스 설정
			m_pCSHeightMap->SetBrushScale(m_vBrushScale);   // 브러쉬 크기
			m_pCSHeightMap->SetHeightMap(m_pHeightMap);
			m_pCSHeightMap->SetDir(m_iBrushDir);
			m_pCSHeightMap->SetBrushPow(m_fBrushPow);
			m_pCSHeightMap->Excute();
		}
		else if (LANDSCAPE_MOD::SPLAT == m_eMod)
		{
			// 교점 위치정보를 가중치를 수정함	
			m_pCSWeightMap->SetBrushArrTex(m_pBrushArrTex);
			m_pCSWeightMap->SetBrushIndex(m_iBrushIdx);
			m_pCSWeightMap->SetBrushScale(m_vBrushScale); // 브러쉬 크기
			m_pCSWeightMap->SetWeightMap(m_pWeightMapBuffer, m_iWeightWidth, m_iWeightHeight); // 가중치맵, 가로 세로 개수
			m_pCSWeightMap->SetInputBuffer(m_pCrossBuffer); // 레이 캐스트 위치
			m_pCSWeightMap->SetWeightIdx(m_iWeightIdx);
			m_pCSWeightMap->SetWeightMapTex(m_pWeightTex);
			m_pCSWeightMap->Excute();
		}
	}


	//if (LANDSCAPE_MOD::NONE != m_eMod && CSceneMgr::GetInst()->GetCurScene()->GetSceneState() == SCENE_STATE::STOP)
	//{
	//	m_pCSPreviewMap->SetInputBuffer(m_pCrossBuffer);
	//	m_pCSPreviewMap->SetBrushTex(m_pBrushArrTex);
	//	m_pCSPreviewMap->SetBrushIndex(m_iBrushIdx);
	//	m_pCSPreviewMap->SetBrushScale(m_vBrushScale);
	//	m_pCSPreviewMap->SetPreviewMap(m_pPreviewTex);
	//	m_pCSPreviewMap->Excute();
	//}
	//else if(LANDSCAPE_MOD::NONE == m_eMod || CSceneMgr::GetInst()->GetCurScene()->GetSceneState() == SCENE_STATE::PLAY)
	//{
	//	m_pCSPreviewMap->SetInputBuffer(m_pCrossBuffer);
	//	m_pCSPreviewMap->SetBrushTex(m_pBrushArrTex);
	//	m_pCSPreviewMap->SetBrushIndex(m_iBrushIdx);
	//	m_pCSPreviewMap->SetBrushScale(m_vBrushScale);
	//	m_pCSPreviewMap->SetPreviewMap(m_pPreviewTex);
	//	m_pCSPreviewMap->SetZero(1);
	//	m_pCSPreviewMap->Excute();
	//}
}

void CLandScape::render()
{


	Transform()->UpdateData();

	// 가중치 버퍼 전달
	m_pWeightMapBuffer->UpdateData(PIPELINE_STAGE::PS, 17); 

	// 가중치 버퍼 해상도 전달
	Vec2 vWeightMapResolution = Vec2((float)m_iWeightWidth, (float)m_iWeightHeight); 
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC2_1, &vWeightMapResolution);
		
	// 타일 배열 개수 전달
	float m_fTileCount = float(m_pTileArrTex->GetArraySize() / 2); // 색상, 노말 합쳐져있어서 나누기 2 해줌
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, &m_fTileCount);

	// 타일 텍스쳐 전달
	GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_ARR_0, m_pTileArrTex);

	GetMaterial(0)->UpdateData();
	GetMesh()->render(0);

	m_pWeightMapBuffer->Clear();


}


void CLandScape::Raycasting()
{
	// 시점 카메라를 가져옴
	CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();
	if (nullptr == pMainCam)
		return;

	// 월드 기준 광선을 지형의 로컬로 보냄
	const Matrix& matWorldInv = Transform()->GetWorldInvMat();
	const tRay& ray = pMainCam->GetRay();

	tRay CamRay = {};
	CamRay.vStart = XMVector3TransformCoord(ray.vStart, matWorldInv);
	CamRay.vDir = XMVector3TransformNormal(ray.vDir, matWorldInv);
	CamRay.vDir.Normalize();

	// 지형과 카메라 Ray 의 교점을 구함
	tRaycastOut out = { Vec2(0.f, 0.f), 0x7fffffff, 0 };
	m_pCrossBuffer->SetData(&out, 1);

	m_pCSRaycast->SetHeightMap(m_pHeightMap);
	m_pCSRaycast->SetFaceCount(m_iXFaceCount, m_iZFaceCount);
	m_pCSRaycast->SetCameraRay(CamRay);
	m_pCSRaycast->SetOuputBuffer(m_pCrossBuffer);

	m_pCSRaycast->Excute();
}



void CLandScape::WeightExecute()
{
	m_pCSWeightMap->SetBrushArrTex(m_pBrushArrTex);
	m_pCSWeightMap->SetBrushIndex(m_iBrushIdx);
	m_pCSWeightMap->SetBrushScale(m_vBrushScale); // 브러쉬 크기
	m_pCSWeightMap->SetWeightMap(m_pWeightMapBuffer, m_iWeightWidth, m_iWeightHeight); // 가중치맵, 가로 세로 개수
	m_pCSWeightMap->SetInputBuffer(m_pCrossBuffer); // 레이 캐스트 위치
	m_pCSWeightMap->SetWeightIdx(m_iWeightIdx);
	m_pCSWeightMap->SetWeightMapTex(m_pWeightTex);
	m_pCSWeightMap->SetSave(1);
	m_pCSWeightMap->Excute();
}

void CLandScape::WeightLoad()
{
	m_pCSWeightMap->SetBrushArrTex(m_pBrushArrTex);
	m_pCSWeightMap->SetBrushIndex(m_iBrushIdx);
	m_pCSWeightMap->SetBrushScale(m_vBrushScale); // 브러쉬 크기
	m_pCSWeightMap->SetWeightMap(m_pWeightMapBuffer, m_iWeightWidth, m_iWeightHeight); // 가중치맵, 가로 세로 개수
	m_pCSWeightMap->SetInputBuffer(m_pCrossBuffer); // 레이 캐스트 위치
	m_pCSWeightMap->SetWeightIdx(m_iWeightIdx);
	m_pCSWeightMap->SetWeightMapTex(m_pWeightTex);
	m_pCSWeightMap->SetLoad(1);
	m_pCSWeightMap->Excute();
}

void CLandScape::HeightLoad()
{
	LandScape()->GetSharedMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, m_pHeightMap);

	m_pCSHeightMap->SetInputBuffer(m_pCrossBuffer); // 픽킹 정보를 HeightMapShader 에 세팅
	m_pCSHeightMap->SetBrushTex(m_pBrushArrTex);	// 사용할 브러쉬 텍스쳐 세팅
	m_pCSHeightMap->SetBrushIndex(m_iBrushIdx);		// 브러쉬 인덱스 설정
	m_pCSHeightMap->SetBrushScale(m_vBrushScale);   // 브러쉬 크기
	m_pCSHeightMap->SetHeightMap(m_pHeightMap);
	m_pCSHeightMap->SetDir(m_iBrushDir);
	m_pCSHeightMap->SetBrushPow(m_fBrushPow);
	m_pCSHeightMap->SetLoad(1);
	m_pCSHeightMap->Excute();
}
void CLandScape::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);

	SaveResPtr(m_pHeightMap,_pFile);
	SaveResPtr(m_pWeightTex, _pFile);
	SaveResPtr(m_pBrushArrTex, _pFile);
	SaveResPtr(m_pPreviewTex, _pFile);
	SaveResPtr(m_pTileArrTex, _pFile);

	fwrite(&m_iXFaceCount, sizeof(UINT), 1, _pFile);
	fwrite(&m_iZFaceCount, sizeof(UINT), 1, _pFile);
	fwrite(&m_iWeightWidth, sizeof(UINT), 1, _pFile);
	fwrite(&m_iWeightHeight, sizeof(UINT), 1, _pFile);

	SaveResPtr(GetMesh(), _pFile);
	SaveResPtr(GetSharedMaterial(0), _pFile);

}

void CLandScape::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);

	m_pCSRaycast = (CRaycastShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"RaycastShader").Get();
	m_pCSHeightMap = (CHeightMapShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"HeightMapShader").Get();
	m_pCSWeightMap = (CWeightMapShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"WeightMapShader").Get();
	m_pCSPreviewMap = (CPreivewMapShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"PreviewMapShader").Get();

	LoadResPtr(m_pHeightMap, _pFile);
	LoadResPtr(m_pWeightTex, _pFile);
	LoadResPtr(m_pBrushArrTex, _pFile);
	LoadResPtr(m_pPreviewTex, _pFile);
	LoadResPtr(m_pTileArrTex, _pFile);

	fread(&m_iXFaceCount, sizeof(UINT), 1, _pFile);
	fread(&m_iZFaceCount, sizeof(UINT), 1, _pFile);
	fread(&m_iWeightWidth, sizeof(UINT), 1, _pFile);
	fread(&m_iWeightHeight, sizeof(UINT), 1, _pFile);

	Ptr<CMesh> pMesh;
	Ptr<CMaterial> pMtrl;
	LoadResPtr(pMesh, _pFile);
	LoadResPtr(pMtrl, _pFile);

	SetMesh(pMesh);
	SetSharedMaterial(pMtrl, 0);

	if (!m_pWeightMapBuffer)
	{
		m_pWeightMapBuffer = new CStructuredBuffer;
		m_pWeightMapBuffer->Create(sizeof(tWeight_4), m_iWeightWidth * m_iWeightHeight, SB_TYPE::READ_WRITE, false, nullptr);
	}
	//m_pHeightMap != CResMgr::GetInst()->FindRes<CTexture>(L"HeightMap")
	if (m_pHeightMap != nullptr)
	{
		wstring wstr = CPathMgr::GetInst()->GetContentPath() + m_pHeightMap->GetRelativePath();
		m_pHeightMap->Load(wstr, 1, true);
		HeightLoad();
	}
	//m_pWeightTex != CResMgr::GetInst()->FindRes<CTexture>(L"WeightMap")
	if (m_pWeightTex != nullptr)
	{
		wstring wstr = CPathMgr::GetInst()->GetContentPath() + m_pWeightTex->GetRelativePath();
		m_pWeightTex->Load(wstr, 1, true);
		WeightLoad();
	}

}

void CLandScape::SaveJsonFile(Json::Value& root)
{
	Json::Value LandScape;

	SaveJsonRes(m_pHeightMap,"HeightMap", &LandScape);
	SaveJsonRes(m_pWeightTex,"WeightTex", &LandScape);
	SaveJsonRes(m_pBrushArrTex,"BrushArrTex", &LandScape);
	SaveJsonRes(m_pPreviewTex,"PreviewTex", & LandScape);
	SaveJsonRes(m_pTileArrTex,"TileArrTex", &LandScape);

	LandScape["XFaceCount"] = m_iXFaceCount;
	LandScape["ZFaceCount"] = m_iZFaceCount;
	LandScape["WeightWidth"] = m_iWeightWidth;
	LandScape["iWeightHeight"] = m_iWeightHeight;

	SaveJsonRes(GetMesh(),"Mesh", &LandScape);
	SaveJsonRes(GetMaterial(0),"Material", &LandScape);

	root["LANDSCAPE"] = LandScape;
}

void CLandScape::LoadJsonFromFile(Json::Value& root)
{
	m_pCSRaycast = (CRaycastShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"RaycastShader").Get();
	m_pCSHeightMap = (CHeightMapShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"HeightMapShader").Get();
	m_pCSWeightMap = (CWeightMapShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"WeightMapShader").Get();
	m_pCSPreviewMap = (CPreivewMapShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"PreviewMapShader").Get();

	Json::Value LandScape = root;

	LoadJsonRes(m_pHeightMap, &(LandScape["HeightMap"]));
	LoadJsonRes(m_pWeightTex, &(LandScape["WeightTex"]));
	LoadJsonRes(m_pBrushArrTex, &(LandScape["BrushArrTex"]));
	LoadJsonRes(m_pPreviewTex, &(LandScape["PreviewTex"]));
	LoadJsonRes(m_pTileArrTex, &(LandScape["TileArrTex"]));

	m_iXFaceCount = LandScape["XFaceCount"].asUInt();
	m_iZFaceCount = LandScape["ZFaceCount"].asUInt();
	m_iWeightWidth = LandScape["WeightWidth"].asUInt();
	m_iWeightHeight = LandScape["iWeightHeight"].asUInt();

	Ptr<CMesh> pMesh;
	Ptr<CMaterial> pMtrl;
	LoadJsonRes(pMesh, &(LandScape["Mesh"]));
	LoadJsonRes(pMtrl, &(LandScape["Material"]));

	SetMesh(pMesh);
	SetSharedMaterial(pMtrl, 0);

	if (!m_pWeightMapBuffer)
	{
		m_pWeightMapBuffer = new CStructuredBuffer;
		m_pWeightMapBuffer->Create(sizeof(tWeight_4), m_iWeightWidth * m_iWeightHeight, SB_TYPE::READ_WRITE, false, nullptr);
	}

	if (m_pHeightMap != nullptr)
	{
		wstring wstr = CPathMgr::GetInst()->GetContentPath() + m_pHeightMap->GetRelativePath();
		m_pHeightMap->Load(wstr, 1, true);
		HeightLoad();
	}

	if (m_pWeightTex != nullptr)
	{
		wstring wstr = CPathMgr::GetInst()->GetContentPath() + m_pWeightTex->GetRelativePath();
		m_pWeightTex->Load(wstr, 1, true);
		WeightLoad();
	}
}
