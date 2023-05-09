#include "pch.h"
#include "CAnimation3D.h"

#include <Engine/CTimeMgr.h>


CAnimation3D::CAnimation3D()
	: m_pOwner(nullptr)
	, m_AnimInfo{}
	, m_iClipIdx(0)
	, m_iFrameCount(0)
	, m_bFinish(false)
	, m_fTime(0.f)
	, m_iPrevFrameIdx(0)
	, m_iFrameIdx(0)
	, m_iNextFrameIdx(0)
	, m_fRatio(0.f)
	, m_fLerpTime(0.07f)
	, m_bIsLerp(false)
	, m_fSpeed(1.f)
	, m_bFinalMatUpdate{}
	, m_pBoneFinalMatBuffer{}
{

}

CAnimation3D::CAnimation3D(const CAnimation3D& _origin)
	: m_pOwner(nullptr)
	, m_AnimInfo(_origin.m_AnimInfo)
	, m_iClipIdx(_origin.m_iClipIdx)
	, m_iFrameCount(_origin.m_iFrameCount)
	, m_bFinish(false)
	, m_fTime(0.f)
	, m_iPrevFrameIdx(0)
	, m_iFrameIdx(0)
	, m_iNextFrameIdx(0)
	, m_fRatio(_origin.m_fRatio)
	, m_fLerpTime(_origin.m_fLerpTime)
	, m_bIsLerp(false)
	, m_fSpeed(_origin.m_fSpeed)
	, m_bFinalMatUpdate{}
	, m_pBoneFinalMatBuffer{}
{
	int a = 0;
}

CAnimation3D::~CAnimation3D()
{
}


void CAnimation3D::finalupdate()
{
	if(m_bFinish)
		return;

	m_fTime += DT * m_fSpeed;

	// 이전 프레임 보간
	if (m_bIsLerp)
	{
		if (m_fTime > m_fLerpTime)
		{
			m_fTime = 0.f;
			m_bIsLerp = false;
		}
		m_fRatio = m_fTime / m_fLerpTime;
		return;
	}

	// Animation Info값으로 프레임 계산
	double dFrameIdx = m_AnimInfo.iStartFrame + m_fTime * (double)m_iFrameCount;
	m_iFrameIdx = (int)(dFrameIdx);

	// 다음 프레임 인덱스
	if (m_iFrameIdx >= m_AnimInfo.iStartFrame + m_AnimInfo.iFrameLength - 1)
	{
		m_fTime = 0;
		m_bFinish = true;
	}
	else
	{
		m_iNextFrameIdx = m_iFrameIdx + 1;
	}

	// 프레임간의 시간에 따른 비율을 구해준다.
	m_fRatio = (float)(dFrameIdx - (double)m_iFrameIdx);
}

CAnimation3D* CAnimation3D::CreateAnim(const wstring& _strName, int _ClipIdx, float _fStart, float _fEnd, int _AnimFrame)
{
	m_AnimInfo.strAnimName = _strName;
	m_AnimInfo.iStartFrame = _fStart;
	m_AnimInfo.iEndFrame = _fEnd;
	m_AnimInfo.iFrameLength = _fEnd - _fStart;

	m_AnimInfo.dStartTime = _fStart /(double) _AnimFrame;
	m_AnimInfo.dEndTime = _fEnd / (double)_AnimFrame;
	m_AnimInfo.dTimeLength = (_fEnd - _fStart) / (double)_AnimFrame;

	m_iClipIdx = _ClipIdx;
	m_iFrameCount = _AnimFrame;

	return this;
}

void CAnimation3D::SaveToScene(FILE* _pFile)
{
	CEntity::SaveToScene(_pFile);

	//int strlen = m_AnimInfo.strAnimName.length();
	//fwrite(&strlen, sizeof(int), 1, _pFile);
	//fwrite(m_AnimInfo.strAnimName.c_str(), sizeof(wchar_t), strlen, _pFile);

	SaveWStringToFile(m_AnimInfo.strAnimName, _pFile);

	
	fwrite(&m_iClipIdx, sizeof(int), 1, _pFile);
	fwrite(&m_iFrameCount, sizeof(int), 1, _pFile);
	fwrite(&m_AnimInfo.iStartFrame, sizeof(int), 1, _pFile);
	fwrite(&m_AnimInfo.iEndFrame, sizeof(int), 1, _pFile);
}

void CAnimation3D::LoadFromScene(FILE* _pFile)
{
	CEntity::LoadFromScene(_pFile);

	wstring wstr;
	LoadWStringFromFile(wstr, _pFile);
	//int strlen = 0;
	//fread(&strlen, sizeof(int), 1, _pFile);
	//fread(&wstr, sizeof(wchar_t), strlen, _pFile);
	m_AnimInfo.strAnimName = wstr;

	fread(&m_iClipIdx, sizeof(int), 1, _pFile);
	fread(&m_iFrameCount, sizeof(int), 1, _pFile);

	int _StartFrame = 0;
	int _EndFrame = 0;

	fread(&_StartFrame, sizeof(int), 1, _pFile);
	fread(&_EndFrame, sizeof(int), 1, _pFile);

	m_AnimInfo.iStartFrame = _StartFrame;
	m_AnimInfo.iEndFrame = _EndFrame;
	m_AnimInfo.iFrameLength = _EndFrame - _StartFrame;
	m_AnimInfo.dStartTime = double(_StartFrame / m_iFrameCount);
	m_AnimInfo.dEndTime = double(_EndFrame / m_iFrameCount);
	m_AnimInfo.dTimeLength = double((_EndFrame - _StartFrame) / m_iFrameCount);
}


void CAnimation3D::SaveJsonFile(Json::Value& root, const string& str)
{
	Json::Value Animation3DInfo;

	Animation3DInfo["AnimName"] = string(m_AnimInfo.strAnimName.begin(), m_AnimInfo.strAnimName.end());
	Animation3DInfo["ClipIndex"] = m_iClipIdx;
	Animation3DInfo["FrameCount"] = m_iFrameCount;
	Animation3DInfo["StartFrame"] = m_AnimInfo.iStartFrame;
	Animation3DInfo["EndFrame"] = m_AnimInfo.iEndFrame;

	root[str] = Animation3DInfo;
}

void CAnimation3D::LoadJsonFromFile(Json::Value& root, const string& str)
{
	Json::Value Animation3DInfo = root[str];

	string _str = Animation3DInfo["AnimName"].asString();
	wstring wstr = wstring(_str.begin(), _str.end());

	m_AnimInfo.strAnimName = wstr;
	m_iClipIdx = Animation3DInfo["ClipIndex"].asInt();
	m_iFrameCount = Animation3DInfo["FrameCount"].asInt();
	m_AnimInfo.iStartFrame = Animation3DInfo["StartFrame"].asInt();
	m_AnimInfo.iEndFrame = Animation3DInfo["EndFrame"].asInt();

	m_AnimInfo.iFrameLength = m_AnimInfo.iEndFrame - m_AnimInfo.iStartFrame;
	m_AnimInfo.dStartTime = double(m_AnimInfo.iStartFrame / m_iFrameCount);
	m_AnimInfo.dEndTime = double(m_AnimInfo.iEndFrame / m_iFrameCount);
	m_AnimInfo.dTimeLength = double((m_AnimInfo.iEndFrame - m_AnimInfo.iStartFrame) / m_iFrameCount);
}


