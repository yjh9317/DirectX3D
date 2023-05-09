#include "pch.h"
#include "CHDRShader.h"


CHDRShader::CHDRShader()
	: CComputeShader(1024, 1, 1)
{
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
	
	m_iGroupX = 1;
	m_iGroupY = 1;
	m_iGroupZ = 1;
}

CHDRShader::~CHDRShader()
{
}

void CHDRShader::UpdateData()
{
	//static const float4 LUM_FACTOR = float4(0.299, 0.587, 0.114, 0);
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();

	// Resolution of down scale (Res : Back buffer width and height divided by 4)
	m_Param.v2Arr[0] = Vec2(vResolution.x / 4.f, vResolution.y / 4.f);

	// Total pixel in the downscaled image ( Domain :  Back buffer width times height divided by 16)
	m_Param.iArr[0] = (vResolution.x * vResolution.y) / 16;

	// Number of groups dispached on the first pass ( GroupSize : Back buffer width times height divided by 16 times 1024)
	m_Param.iArr[1] = (UINT)ceil((float)(vResolution.x * vResolution.y) / (16 * 1024));

	// HDRTex
	m_FirstBuffer->UpdateData_CS(0, true);

	m_SecondBuffer->UpdateData_CS(0, false);


}

void CHDRShader::Clear()
{
	m_FirstBuffer->Clear();
	
	m_SecondBuffer->Clear();
}