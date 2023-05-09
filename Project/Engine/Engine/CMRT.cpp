#include "pch.h"
#include "CMRT.h"

#include "CDevice.h"

CMRT::CMRT()
	: m_arrRT{}
	, m_iRTCount(0)
	, m_pDSTex(nullptr)
	, m_arrClearColor{Vec4(0.f, 0.f, 0.f, 1.f)
	, Vec4(0.f, 0.f, 0.f, 1.f)
	, Vec4(0.f, 0.f, 0.f, 1.f)
	, Vec4(0.f, 0.f, 0.f, 1.f)
	, Vec4(0.f, 0.f, 0.f, 1.f)
	, Vec4(0.f, 0.f, 0.f, 1.f)
	, Vec4(0.f, 0.f, 0.f, 1.f)
	, Vec4(0.f, 0.f, 0.f, 1.f)}
	, m_tViewPort{}
{
}

CMRT::~CMRT()
{
}

void CMRT::Create(int _iRTCount, Ptr<CTexture>* _ppTex, Ptr<CTexture> _pDSTex)
{
	m_iRTCount = _iRTCount;

	for (int i = 0; i < m_iRTCount; ++i)
	{
		m_arrRT[i] = _ppTex[i];
	}

	m_pDSTex = _pDSTex;

	// ViewPort ����	
	m_tViewPort.TopLeftX = 0;
	m_tViewPort.TopLeftY = 0;

	m_tViewPort.Width = m_arrRT[0]->Width();
	m_tViewPort.Height = m_arrRT[0]->Height();

	m_tViewPort.MinDepth = 0;
	m_tViewPort.MaxDepth = 1;
}

void CMRT::OMSet()
{
	ID3D11RenderTargetView* arrView[8] = {};

	for (int i = 0; i < m_iRTCount; ++i)
	{
		arrView[i] = m_arrRT[i]->GetRTV().Get();
	}

	if (nullptr != m_pDSTex)
	{
		CONTEXT->OMSetRenderTargets(m_iRTCount, arrView, m_pDSTex->GetDSV().Get());
	}

	else
	{
		// MRT �� DSTex �� ���� ���, ��ġ�� �ִ��� �״�� ����.

		//ID3D11DepthStencilView* pDSView = nullptr;
		//CONTEXT->OMGetRenderTargets(0, nullptr, &pDSView);
		//pDSView->Release();
		
		ComPtr<ID3D11DepthStencilView> pDSView = nullptr;
		CONTEXT->OMGetRenderTargets(0, nullptr, pDSView.GetAddressOf());
		CONTEXT->OMSetRenderTargets(m_iRTCount, arrView, pDSView.Get());
	}	

	// ViewPort ����
	CONTEXT->RSSetViewports(1, &m_tViewPort);
}

void CMRT::Clear()
{
	for (int i = 0; i < m_iRTCount; ++i)
	{
		CONTEXT->ClearRenderTargetView(m_arrRT[i]->GetRTV().Get(), m_arrClearColor[i]);
	}
	
	if(nullptr != m_pDSTex)
		CONTEXT->ClearDepthStencilView(m_pDSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}
