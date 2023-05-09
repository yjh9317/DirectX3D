#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;


class CAnimation3DShader :
	public CComputeShader
{
private:
	CStructuredBuffer* m_pFrameDataBuffer;			// t13
	CStructuredBuffer* m_pOffsetMatBuffer;			// t14 
	CStructuredBuffer* m_pOutputBuffer;				// u0
	CStructuredBuffer* m_pSocketBuffer;				// u1
	CStructuredBuffer* m_pSocketBuffer2;				// u2

public:
	// g_int_0 : BonCount, g_int_1 : Frame Index
	void SetBoneCount(int _iBoneCount) { m_Param.iArr[0] = _iBoneCount; }
	void SetFrameIndex(int _iFrameIdx) { m_Param.iArr[1] = _iFrameIdx; }
	void SetNextFrameIdx(int _iFrameIdx) { m_Param.iArr[2] = _iFrameIdx; }
	void SetIsPrevAnim(int _iIsAnim) { m_Param.iArr[3] = _iIsAnim; }
	

	void SetFrameRatio(float _fFrameRatio) { m_Param.fArr[0] = _fFrameRatio; }
	void SetPrevFrameIdx(float _fFramIdx) { m_Param.fArr[1] = _fFramIdx; }
	void SetBoneIdx(float _fBoneIdx) { m_Param.fArr[2] = (int)_fBoneIdx; }
	void SetBone2Idx(float _fBoneIdx) { m_Param.fArr[3] = (int)_fBoneIdx; }

	void SetFrameDataBuffer(CStructuredBuffer* _buffer) { m_pFrameDataBuffer = _buffer; }
	void SetOffsetMatBuffer(CStructuredBuffer* _buffer) { m_pOffsetMatBuffer = _buffer; }
	void SetOutputBuffer(CStructuredBuffer* _buffer) { m_pOutputBuffer = _buffer; }
	void SetSocketBuffer(CStructuredBuffer* _buffer) { m_pSocketBuffer = _buffer; }
	void SetSocket2Buffer(CStructuredBuffer* _buffer) { m_pSocketBuffer2 = _buffer; }

public:
	virtual void UpdateData();
	virtual void Clear();

public:
	CAnimation3DShader();
	virtual ~CAnimation3DShader();
};


