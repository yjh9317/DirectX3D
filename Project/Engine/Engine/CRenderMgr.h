#pragma once
#include "singleton.h"

class CCamera;
class CLight2D;
class CLight3D;
class CMRT;

class CRenderMgr :
    public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr);
private:
    vector<CCamera*>    m_vecCam;           // Scene �� �ִ� Camera ��
    CCamera*            m_pEditorCam;       // Editor ���� ī�޶�
    CCamera*            m_pUICamera;

    vector<CLight2D*>   m_vecLight2D;
    CStructuredBuffer*  m_pLight2DBuffer;    // Scene �� 2D ���� ����

    vector<CLight3D*>   m_vecLight3D;
    CStructuredBuffer*  m_pLight3DBuffer;    // Scene �� 3D ���� ����

    CMRT*               m_arrMRT[(UINT)MRT_TYPE::END];

    // Merge 
    CGraphicsShader*    m_pMergeShader;
    CMaterial*          m_pMergeMtrl;



public:
    void init();
    void update();
    void render();

private:
    void render_play();
    void render_editor();

    void render_shadowmap();
    void render_lights();

    void render_collider();

public:    
    void RegisterCamera(CCamera* _pCam);
    void RegisterEditorCamera(CCamera* _pCam){ m_pEditorCam = _pCam; }
    void ClearCamera() { m_vecCam.clear(); }
    void SwapCameraIndex(CCamera* _pCam, int _iChangeIdx);
    void CopyTargetToPostProcess();

    int RegisterLight2D(CLight2D* _pLight2D)
    {
        m_vecLight2D.push_back(_pLight2D);
        return (int)m_vecLight2D.size() - 1;
    }

    int RegisterLight3D(CLight3D* _pLight3D)
    {
        m_vecLight3D.push_back(_pLight3D);
        return (int)m_vecLight3D.size() - 1;
    }


    // ���� ���� ī�޶� ��������
    CCamera* GetMainCam() const;
    CCamera* GetUICam();
    CMRT* GetMRT(MRT_TYPE _eType) const { return m_arrMRT[(UINT)_eType]; }

    


private:
    void CreateMRT();
    void CreateMaterial();
    void ClearMRT();
    void ClearTextureRegister();

    void UpdateLight2D();
    void UpdateLight3D();

};

