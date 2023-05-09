#pragma once

class CConstBuffer;

class CDevice
	: public CSingleton<CDevice>
{
	SINGLE(CDevice);
private:
	HWND							m_hWnd; // Main Window Handle
	Vec2							m_vRenderResolution; // ������ �ػ�
		
	ComPtr<ID3D11Device>			m_pDevice;			// GPU �޸� ����
	ComPtr<ID3D11DeviceContext>		m_pDeviceContext;	// GPU Rendering ����

	ComPtr<IDXGISwapChain>			m_pSwapChain;
	DXGI_SWAP_CHAIN_DESC			m_tSwapChainDesc;

	ComPtr<ID3D11RasterizerState>	m_arrRS[(UINT)RS_TYPE::END];
	ComPtr<ID3D11DepthStencilState> m_arrDS[(UINT)DS_TYPE::END];
	ComPtr<ID3D11BlendState>		m_arrBS[(UINT)BS_TYPE::END];

	CConstBuffer*					m_arrCB[(UINT)CB_TYPE::END];

	ComPtr<ID3D11SamplerState>		m_arrSam[4];

public:
	int init(HWND _hWnd, Vec2 _vRenderResolution);
	void Present() { m_pSwapChain->Present(0, 0); }	

	Vec2 GetRenderResolution() { return m_vRenderResolution; }

	ComPtr<ID3D11Device> GetDevice() const { return m_pDevice; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() const { return m_pDeviceContext; }

	ComPtr<ID3D11RasterizerState> GetRS(RS_TYPE _eType) const { return m_arrRS[(UINT)_eType]; }
	ComPtr<ID3D11DepthStencilState> GetDS(DS_TYPE _eType) const { return m_arrDS[(UINT)_eType]; }
	ComPtr<ID3D11BlendState> GetBS(BS_TYPE _eType) const { return m_arrBS[(UINT)_eType]; }
	CConstBuffer* GetCB(CB_TYPE _eType) const { return m_arrCB[(UINT)_eType]; }

private:
	int CreateSwapchain();
	int CreateView();
	int CreateRasterizerState();
	int CreateDepthStencilState();
	int CreateBlendState();
	int CreateConstBuffer();
	void CreateSamplerState();
};

