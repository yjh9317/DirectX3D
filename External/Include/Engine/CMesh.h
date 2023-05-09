#pragma once
#include "CRes.h"
#include "CFBXLoader.h"

class CStructuredBuffer;

struct tIndexInfo
{
    ComPtr<ID3D11Buffer>    pIB;
    D3D11_BUFFER_DESC       tIBDesc;
    UINT				    iIdxCount;
    void*                   pIdxSysMem;
};

class CMesh :
    public CRes
{
private:
    ComPtr<ID3D11Buffer>    m_VB;
    D3D11_BUFFER_DESC       m_tVBDesc;
    UINT                    m_iVtxCount;
    void*                   m_pVtxSys;

    // �ϳ��� ���ؽ����ۿ� �������� �ε������۰� ����
    vector<tIndexInfo>		m_vecIdxInfo;

    // Animation3D ����
    vector<tMTAnimClip>		m_vecAnimClip;
    vector<tMTBone>			m_vecBones;

    CStructuredBuffer*      m_pBoneFrameData;   // ��ü �� ������ ����(ũ��, �̵�, ȸ��) (������ ������ŭ)
    CStructuredBuffer*      m_pBoneOffset;	    // �� ���� offset ���(�� ���� ��ġ�� �ǵ����� ���) (1�� ¥��)
    

public:
    Vtx* GetVtxSysMem() { return (Vtx*)m_pVtxSys; }
    UINT GetSubsetCount() { return (UINT)m_vecIdxInfo.size(); }

    const vector<tIndexInfo>& GetvecIdxInfo() { return m_vecIdxInfo; }
    const vector<tMTBone>* GetBones() { return &m_vecBones; }
    UINT GetBoneCount() { return (UINT)m_vecBones.size(); }
    const vector<tMTAnimClip>* GetAnimClip() { return &m_vecAnimClip; }
    bool IsAnimMesh() { return !m_vecAnimClip.empty(); }

    CStructuredBuffer* GetBoneFrameDataBuffer() { return m_pBoneFrameData; } // ��ü �� ������ ����
    CStructuredBuffer* GetBoneOffsetBuffer() { return  m_pBoneOffset; }	   // �� ���� offset ���	

    UINT GetVtxCount() { return m_iVtxCount;}
public:
    virtual int Save(const wstring& _strFilePath);
    virtual int Load(const wstring& _strFilePath);
    
    int Create(void* _pVtxSys, UINT _iVtxCount, void* _pIdxSys, UINT _iIdxCount);
    static CMesh* CreateFromContainer(CFBXLoader& _loader);
    static CMesh* CreateFromContainer(CFBXLoader& _loader, int _idx);

    void render(UINT _iSubset);
    void render_particle(UINT _iCount);

    void UpdateData(UINT _iSubset);
public:
    CMesh();
    ~CMesh();
};

