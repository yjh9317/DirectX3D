#pragma once
#include "CComponent.h"

class CSocket :
    public CComponent
{
private:
    Vec3    m_vTransform;
    Vec3    m_vScale;
    Vec3    m_vRotation;
    Matrix  m_matWorld;
    Matrix  m_Finalmat;
    
    CStructuredBuffer* pBuffer;
    CStructuredBuffer* pBuffer2;

    bool    m_bCollision;

public:
    virtual void finalupdate() override;
    void Regist();
    void Clear();

public:
    const Vec3& GetTransform() { return m_vTransform; }
    const Vec3& GetScale() { return m_vScale; }
    const Vec3& GetRotation() { return m_vRotation; };
    const Matrix& GetMatrix() { return m_matWorld; }
    const Matrix& GetFinalMatrix() { return m_Finalmat; }

    void SetTransform(Vec3 _v) { m_vTransform = _v; }
    void SetScale(Vec3 _v) { m_vScale = _v; }
    void SetRotation(Vec3 _v) { m_vRotation = _v; }
 
    Matrix ConvertColToRow(Matrix _mat);

    void SetCollision(bool _b) { m_bCollision = _b; }
    bool IsCollision() { return m_bCollision; }

public:
    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);

    virtual void SaveJsonFile(Json::Value& root) override;
    virtual void LoadJsonFromFile(Json::Value& root) override;
    CLONE(CSocket)

public:
    CSocket();
    ~CSocket();
};

