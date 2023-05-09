#pragma once
#include "CComponent.h"


class CLight3D :
    public CComponent
{
private:
    tLightInfo      m_LightInfo;
    int             m_iLightIdx;

    Ptr<CMesh>      m_pVolumeMesh;
    Ptr<CMaterial>  m_pLightMtrl;

    CGameObject*    m_pLightCam;

public:
    const tLightInfo& GetLightInfo() { return m_LightInfo; }

    void SetDiffuse(Vec3 _vDiffuse) { m_LightInfo.color.vDiff = _vDiffuse; }
    void SetSpecular(Vec3 _vSpec) { m_LightInfo.color.vSpec = _vSpec; }
    void SetAmbient(Vec3 _vAmb) { m_LightInfo.color.vAmb = _vAmb; }
    void SetLightDir(Vec3 _vDir);    
    void SetRange(float _fRange);
    void SetAngle(float _fAngle) { m_LightInfo.fAngle = _fAngle; }
    void SetLightType(LIGHT_TYPE _eType);

    LIGHT_TYPE GetLightType() { return (LIGHT_TYPE) m_LightInfo.iLightType; }


public:
    virtual void update() override;
    virtual void finalupdate() override;

    void render();
    void render_shadowmap();

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;

    virtual void SaveJsonFile(Json::Value& root) override;
    virtual void LoadJsonFromFile(Json::Value& root) override;
    CLONE(CLight3D);
public:
    CLight3D();
    CLight3D(const CLight3D& _origin);
    ~CLight3D();
};

