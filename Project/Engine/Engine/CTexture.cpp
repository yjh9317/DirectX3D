#include "pch.h"
#include "CTexture.h"

#include "CDevice.h"


CTexture::CTexture()
    : CRes(RES_TYPE::TEXTURE)
{
}

CTexture::~CTexture()
{    
}

int CTexture::Load(const wstring& _strFilePath)
{
    // Texture File �� SystemMem �� �ε��Ѵ�.
    wchar_t szExt[25] = {};
    _wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 25);
        

    HRESULT hr = S_OK;
    int iErr = 0;
    
    if (!wcscmp(szExt, L".dds") || !wcscmp(szExt, L".DDS"))
    {
        hr = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS_NONE, nullptr, m_Image);
    }

    else if (!wcscmp(szExt, L".tga") || !wcscmp(szExt, L".TGA"))
    {
        hr = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_Image);
    }

    else // WIC (.png, jpg, jpeg, bmp...)
    {
        hr = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS_NONE, nullptr, m_Image);
    }

    if (CheckFail(hr))
        return E_FAIL;


    // System memory �� �ҷ����� �ȼ� �����͸� GPU �� ������   
    hr = CreateShaderResourceView(DEVICE, m_Image.GetImages()
        , m_Image.GetImageCount()
        , m_Image.GetMetadata()
        , m_pSRV.GetAddressOf());

    if (CheckFail(hr))
        return E_FAIL;

    m_pSRV->GetResource((ID3D11Resource**)m_pTex2D.GetAddressOf());

    m_pTex2D->GetDesc(&m_tDesc);

    return S_OK;
}


int CTexture::Load(const wstring& _strFilePath, int _iMipLevel)
{
    wchar_t strBuff[50] = {};
    _wsplitpath_s(_strFilePath.c_str(), 0, 0, 0, 0, 0, 0, strBuff, 50);
    wstring strExt = strBuff;

    HRESULT hRet = S_OK;

    if (strExt == L".dds" || strExt == L".DDS")
    {
        // .dds .DDS
        hRet = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS_FORCE_RGB, nullptr, m_Image);
    }
    else if (strExt == L".tga" || strExt == L".TGA")
    {
        // .tga .TGA
        hRet = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_Image);
    }
    else
    {
        // .png .jpg .jpeg .bmp
        hRet = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS_NONE, nullptr, m_Image);
    }

    if (FAILED(hRet))
    {
        wsprintf(strBuff, L"�����ڵ� : %d", hRet);
        MessageBox(nullptr, strBuff, L"�ؽ��� �ε� ����", MB_OK);
        return hRet;
    }

    // Texture2D ����
    m_tDesc.Format = m_Image.GetMetadata().format;

    if (m_Image.GetMetadata().IsCubemap())
    {
        m_tDesc.MipLevels = 1;	// 0 ==> ������ ��� �Ӹ��� ���� �� �� �ִ� ������ �������
        m_tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        m_tDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
    }
    else
    {
        m_tDesc.MipLevels = _iMipLevel;// MAX_MIP;	// 0 ==> ������ ��� �Ӹ��� ���� �� �� �ִ� ������ �������	
        m_tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        m_tDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

    }

    m_tDesc.ArraySize = (UINT)m_Image.GetMetadata().arraySize;

    m_tDesc.SampleDesc.Count = 1;
    m_tDesc.SampleDesc.Quality = 0;

    m_tDesc.Usage = D3D11_USAGE_DEFAULT;
    m_tDesc.CPUAccessFlags = 0;

    m_tDesc.Width = (UINT)m_Image.GetMetadata().width;
    m_tDesc.Height = (UINT)m_Image.GetMetadata().height;

    HRESULT hr = DEVICE->CreateTexture2D(&m_tDesc, nullptr, m_pTex2D.GetAddressOf());

    
    // ����������(�Ӹ� ���� 0) �� �� ĭ�� �ű��.	
    for (UINT i = 0; i < m_tDesc.ArraySize; ++i)
    {
        // GPU �� ������ �ű��(�Ӹ� ����)
        UINT iSubresIdx = D3D11CalcSubresource(0, i, m_tDesc.MipLevels);

        CONTEXT->UpdateSubresource(m_pTex2D.Get(), iSubresIdx, nullptr
            , m_Image.GetImage(0, i, 0)->pixels
            , (UINT)m_Image.GetImage(0, i, 0)->rowPitch
            , (UINT)m_Image.GetImage(0, i, 0)->slicePitch);
    }

    // SRV ����
    D3D11_SHADER_RESOURCE_VIEW_DESC viewdesc = {};
    viewdesc.Format = m_tDesc.Format;

    if (m_Image.GetMetadata().IsCubemap())
    {
        viewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    }
    else if (1 < m_tDesc.ArraySize)
    {
        viewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    }
    else
    {
        viewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    }

    if (1 < m_tDesc.ArraySize)
    {
        viewdesc.Texture2DArray.ArraySize = m_tDesc.ArraySize;
        viewdesc.Texture2DArray.MipLevels = m_tDesc.MipLevels;
        viewdesc.Texture2DArray.MostDetailedMip = 0;
    }
    else
    {
        viewdesc.Texture2DArray.ArraySize = 1;
        viewdesc.Texture2D.MipLevels = m_tDesc.MipLevels;
        viewdesc.Texture2D.MostDetailedMip = 0;
    }

    DEVICE->CreateShaderResourceView(m_pTex2D.Get(), &viewdesc, m_pSRV.GetAddressOf());

    // �Ӹ� ����
    if (false == m_Image.GetMetadata().IsCubemap())
    {
        CONTEXT->GenerateMips(m_pSRV.Get());
    }

    m_pTex2D->GetDesc(&m_tDesc);

    return hRet;
}

int CTexture::Load(const wstring& _strFilePath, int _iMipLevel, bool UAV)
{
    wchar_t strBuff[50] = {};
    _wsplitpath_s(_strFilePath.c_str(), 0, 0, 0, 0, 0, 0, strBuff, 50);
    wstring strExt = strBuff;

    HRESULT hRet = S_OK;

    if (strExt == L".dds" || strExt == L".DDS")
    {
        // .dds .DDS
        hRet = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS_FORCE_RGB, nullptr, m_Image);
    }
    else if (strExt == L".tga" || strExt == L".TGA")
    {
        // .tga .TGA
        hRet = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_Image);
    }
    else
    {
        // .png .jpg .jpeg .bmp
        hRet = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS_NONE, nullptr, m_Image);
    }

    if (FAILED(hRet))
    {
        wsprintf(strBuff, L"�����ڵ� : %d", hRet);
        MessageBox(nullptr, strBuff, L"�ؽ��� �ε� ����", MB_OK);
        return hRet;
    }

    // Texture2D ����
    m_tDesc.Format = m_Image.GetMetadata().format;

    if (m_Image.GetMetadata().IsCubemap())
    {
        m_tDesc.MipLevels = 1;	// 0 ==> ������ ��� �Ӹ��� ���� �� �� �ִ� ������ �������
        m_tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        m_tDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

        if (UAV)
        {
            m_tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
        }
    }
    else
    {
        m_tDesc.MipLevels = _iMipLevel;// MAX_MIP;	// 0 ==> ������ ��� �Ӹ��� ���� �� �� �ִ� ������ �������	
        m_tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        m_tDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

        if (UAV)
        {
            m_tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
        }
    }

    m_tDesc.ArraySize = (UINT)m_Image.GetMetadata().arraySize;

    m_tDesc.SampleDesc.Count = 1;
    m_tDesc.SampleDesc.Quality = 0;

    m_tDesc.Usage = D3D11_USAGE_DEFAULT;
    m_tDesc.CPUAccessFlags = 0;

    m_tDesc.Width = (UINT)m_Image.GetMetadata().width;
    m_tDesc.Height = (UINT)m_Image.GetMetadata().height;

    HRESULT hr = DEVICE->CreateTexture2D(&m_tDesc, nullptr, m_pTex2D.GetAddressOf());

    // ����������(�Ӹ� ���� 0) �� �� ĭ�� �ű��.	
    for (UINT i = 0; i < m_tDesc.ArraySize; ++i)
    {
        // GPU �� ������ �ű��(�Ӹ� ����)
        UINT iSubresIdx = D3D11CalcSubresource(0, i, m_tDesc.MipLevels);

        CONTEXT->UpdateSubresource(m_pTex2D.Get(), iSubresIdx, nullptr
            , m_Image.GetImage(0, i, 0)->pixels
            , (UINT)m_Image.GetImage(0, i, 0)->rowPitch
            , (UINT)m_Image.GetImage(0, i, 0)->slicePitch);
    }

    // SRV ����
    D3D11_SHADER_RESOURCE_VIEW_DESC viewdesc = {};
    viewdesc.Format = m_tDesc.Format;

    if (m_Image.GetMetadata().IsCubemap())
    {
        viewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    }
    else if (1 < m_tDesc.ArraySize)
    {
        viewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    }
    else
    {
        viewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    }

    if (1 < m_tDesc.ArraySize)
    {
        viewdesc.Texture2DArray.ArraySize = m_tDesc.ArraySize;
        viewdesc.Texture2DArray.MipLevels = m_tDesc.MipLevels;
        viewdesc.Texture2DArray.MostDetailedMip = 0;
    }
    else
    {
        viewdesc.Texture2DArray.ArraySize = 1;
        viewdesc.Texture2D.MipLevels = m_tDesc.MipLevels;
        viewdesc.Texture2D.MostDetailedMip = 0;
    }

    DEVICE->CreateShaderResourceView(m_pTex2D.Get(), &viewdesc, m_pSRV.GetAddressOf());

    // �Ӹ� ����
    if (false == m_Image.GetMetadata().IsCubemap())
    {
        CONTEXT->GenerateMips(m_pSRV.Get());
    }

    m_pTex2D->GetDesc(&m_tDesc);


    if (UAV)
    {
        D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
        tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;
        tUAVDesc.Texture2D.MipSlice = 0;

        DEVICE->CreateUnorderedAccessView(m_pTex2D.Get(), &tUAVDesc, m_pUAV.GetAddressOf());
    }

    return hRet;
}

int CTexture::Save(const wstring& _strFilePath)
{
    CaptureTexture(DEVICE, CONTEXT, m_pTex2D.Get(), m_Image);

    HRESULT hr = S_OK;
    
    size_t arrsize = m_Image.GetMetadata().arraySize;


    // �Ӹ��� 1 �̻��� ���
    if (1 < m_tDesc.MipLevels)
    {
        D3D11_TEXTURE2D_DESC texdesc = {};
        texdesc = m_tDesc;
        texdesc.MipLevels = 1;
        
        ComPtr<ID3D11Texture2D> pCopyTex2D;
        HRESULT hr = DEVICE->CreateTexture2D(&texdesc, nullptr, pCopyTex2D.GetAddressOf());
        
        for (int i = 0; i < (int)m_tDesc.ArraySize; ++i)
        {
            // m_pTex2D ==> pCopyTex2D
            UINT iDestSubresIdx = D3D11CalcSubresource(0, i, 1);
            UINT iSrcSubresIdx = D3D11CalcSubresource(0, i, MAX_MIP);

            CONTEXT->CopySubresourceRegion(pCopyTex2D.Get(), iDestSubresIdx, 0, 0, 0
                , m_pTex2D.Get(), iSrcSubresIdx, nullptr);
        }

        ScratchImage tempimage;
        CaptureTexture(DEVICE, CONTEXT, pCopyTex2D.Get(), tempimage);

        if (1 < arrsize)
        {
            hr = SaveToDDSFile(tempimage.GetImages()
                , arrsize, tempimage.GetMetadata()
                , DDS_FLAGS::DDS_FLAGS_NONE
                , _strFilePath.c_str());
        }
        else
        {
            hr = SaveToWICFile(*(tempimage.GetImages())
                , WIC_FLAGS_NONE
                , GetWICCodec(static_cast<WICCodecs>(WICCodecs::WIC_CODEC_JPEG))
                , _strFilePath.c_str());
        }
    }
    
    // �Ӹ��� 1 ����(������) �ִ� ���, ĸ�� �� �ٷ� ����
    else
    {
        if (1 < arrsize)
        {
            hr = SaveToDDSFile(m_Image.GetImages()
                , arrsize
                , m_Image.GetMetadata()
                , DDS_FLAGS::DDS_FLAGS_NONE
                , _strFilePath.c_str());
        }
        else
        {
            
            hr = SaveToWICFile(*(m_Image.GetImages())
                , WIC_FLAGS_NONE
                , GetWICCodec(static_cast<WICCodecs>(WICCodecs::WIC_CODEC_PNG))
                , _strFilePath.c_str()
                );
        }
    }

    CRes::Save(_strFilePath);

    return hr;
}


void CTexture::Create(UINT _iWidth, UINT _iHeight, DXGI_FORMAT _format, UINT _flag)
{
    // Texture �����    
    m_tDesc.Width = _iWidth;
    m_tDesc.Height = _iHeight;
    m_tDesc.MipLevels = 1;    
    m_tDesc.ArraySize = 1;

    m_tDesc.CPUAccessFlags = 0;
    m_tDesc.BindFlags = _flag;
    m_tDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
    m_tDesc.MiscFlags = 0;

    m_tDesc.SampleDesc.Count = 1;
    m_tDesc.SampleDesc.Quality = 0;

    m_tDesc.Format = _format;

    DEVICE->CreateTexture2D(&m_tDesc, nullptr, m_pTex2D.GetAddressOf());
    assert(m_pTex2D);


    if (D3D11_BIND_DEPTH_STENCIL & _flag)
    {
        DEVICE->CreateDepthStencilView(m_pTex2D.Get(), nullptr, m_pDSV.GetAddressOf());
        assert(m_pDSV);
    }
    else
    {
        if (D3D11_BIND_RENDER_TARGET & _flag)
        {
            DEVICE->CreateRenderTargetView(m_pTex2D.Get(), nullptr, m_pRTV.GetAddressOf());
            assert(m_pRTV);
        }

        if (D3D11_BIND_SHADER_RESOURCE & _flag)
        {

            // SRV ����
            D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
            tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
            tSRVDesc.Texture2D.MipLevels = 1;
            tSRVDesc.Texture2D.MostDetailedMip = 0;

            DEVICE->CreateShaderResourceView(m_pTex2D.Get(), &tSRVDesc, m_pSRV.GetAddressOf());
            assert(m_pSRV);

        }

        if (D3D11_BIND_UNORDERED_ACCESS & _flag)
        {
            // UAV ����
            D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
            tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;
            tUAVDesc.Texture2D.MipSlice = 0;

            DEVICE->CreateUnorderedAccessView(m_pTex2D.Get(), &tUAVDesc, m_pUAV.GetAddressOf());
            assert(m_pUAV);
        }
    }   
}

void CTexture::Create(ComPtr<ID3D11Texture2D> _pTex2D)
{
    // Texture �����    
    m_pTex2D = _pTex2D;
    m_pTex2D->GetDesc(&m_tDesc);


    if (D3D11_BIND_DEPTH_STENCIL & m_tDesc.BindFlags)
    {
        DEVICE->CreateDepthStencilView(m_pTex2D.Get(), nullptr, m_pDSV.GetAddressOf());
        assert(m_pDSV);
    }
    else
    {
        if (D3D11_BIND_RENDER_TARGET & m_tDesc.BindFlags)
        {
            DEVICE->CreateRenderTargetView(m_pTex2D.Get(), nullptr, m_pRTV.GetAddressOf());
            assert(m_pRTV);
        }

        if (D3D11_BIND_SHADER_RESOURCE & m_tDesc.BindFlags)
        {

            // SRV ����
            D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
            tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
            tSRVDesc.Texture2D.MipLevels = 1;
            tSRVDesc.Texture2D.MostDetailedMip = 0;

            DEVICE->CreateShaderResourceView(m_pTex2D.Get(), &tSRVDesc, m_pSRV.GetAddressOf());
            assert(m_pSRV);

        }

        if (D3D11_BIND_UNORDERED_ACCESS & m_tDesc.BindFlags)
        {
            // UAV ����
            D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
            tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;
            tUAVDesc.Texture2D.MipSlice = 0;

            m_pSRV->GetResource((ID3D11Resource**)m_pTex2D.GetAddressOf());
            DEVICE->CreateUnorderedAccessView(m_pTex2D.Get(), &tUAVDesc, m_pUAV.GetAddressOf());
            assert(m_pUAV);
        }
    }
}

int CTexture::CreateArrayTexture(const vector<Ptr<CTexture>>& _vecTex, int _iMapLevel)
{

    m_tDesc = _vecTex[0].Get()->GetDesc();
    m_tDesc.ArraySize = (UINT)_vecTex.size();
    
    m_tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    m_tDesc.MipLevels = _iMapLevel;

    HRESULT hr = DEVICE->CreateTexture2D(&m_tDesc, nullptr, m_pTex2D.GetAddressOf());
    if (FAILED(hr))
        return hr;

    // ����������(�Ӹ� ���� 0) �� �� ĭ�� �ű��
    for (int i = 0; i < _vecTex.size(); ++i)
    {
        UINT iOffset = D3D11CalcSubresource(0, i, _iMapLevel);
        
        CTexture* Tex = _vecTex[i].Get();
        ScratchImage& Img = Tex->m_Image;
        
        Img.GetPixels();
        
        CONTEXT->UpdateSubresource(m_pTex2D.Get(), iOffset, nullptr
            , Img.GetImage(0, 0, 0)->pixels
            , (UINT)Img.GetImage(0, 0, 0)->rowPitch
            , (UINT)Img.GetImage(0, 0, 0)->slicePitch);
    }

    
    // Shader Resource View ����
    D3D11_SHADER_RESOURCE_VIEW_DESC viewdesc = {};
    viewdesc.Format = m_tDesc.Format;
    viewdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    viewdesc.Texture2DArray.MipLevels = _iMapLevel;
    viewdesc.Texture2DArray.MostDetailedMip = 0;
    viewdesc.Texture2DArray.ArraySize = _vecTex.size();

    DEVICE->CreateShaderResourceView(m_pTex2D.Get(), &viewdesc, m_pSRV.GetAddressOf());

    // �Ӹ� ����
    CONTEXT->GenerateMips(m_pSRV.Get());
    m_pTex2D->GetDesc(&m_tDesc);

    return hr;
}

void CTexture::UpdateData(UINT _PipelineStage, UINT _iRegisterNum)
{
    if (_PipelineStage & (UINT)PIPELINE_STAGE::VS)
        CONTEXT->VSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());

    if (_PipelineStage & (UINT)PIPELINE_STAGE::HS)
        CONTEXT->HSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());

    if (_PipelineStage & (UINT)PIPELINE_STAGE::DS)
        CONTEXT->DSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());

    if (_PipelineStage & (UINT)PIPELINE_STAGE::GS)
        CONTEXT->GSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());

    if (_PipelineStage & (UINT)PIPELINE_STAGE::PS)
        CONTEXT->PSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
}

void CTexture::UpdateData_CS(UINT _iRegisterNum, bool _bShaderResource)
{
    if (_bShaderResource)
    {
        CONTEXT->CSSetShaderResources(_iRegisterNum, 1, m_pSRV.GetAddressOf());
    }
    else
    {
        UINT i = -1;
        CONTEXT->CSSetUnorderedAccessViews(_iRegisterNum, 1, m_pUAV.GetAddressOf(), &i);
    }    
}

void CTexture::Clear(int _iRegisterNum)
{
    ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->VSSetShaderResources(_iRegisterNum, 1, &pSRV);
	CONTEXT->HSSetShaderResources(_iRegisterNum, 1, &pSRV);
	CONTEXT->DSSetShaderResources(_iRegisterNum, 1, &pSRV);
	CONTEXT->GSSetShaderResources(_iRegisterNum, 1, &pSRV);
	CONTEXT->PSSetShaderResources(_iRegisterNum, 1, &pSRV);
}

void CTexture::ClearCS(int _iRegisterNum)
{
    ID3D11UnorderedAccessView* pUAV = nullptr;
    ID3D11ShaderResourceView* pSRV = nullptr;
    UINT i = -1;

    CONTEXT->CSSetUnorderedAccessViews(_iRegisterNum, 1, &pUAV, &i);
    CONTEXT->CSSetShaderResources(_iRegisterNum, 1, &pSRV);
}

