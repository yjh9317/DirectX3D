#pragma once
#include "CComponent.h"

enum class COLLIDER3D_TYPE
{
    CUBE,
    SPHERE,
    SOCKET,
};

class CCollider3D :
    public CComponent
{
private:
    Matrix              m_matColWorld;

    COLLIDER3D_TYPE     m_eColliderType;

    Vec3                m_vOffsetPos;       // 오브젝트로 부터의 충돌체의 상대 거리
    Vec3                m_vOffsetScale;     // 오브젝트 크기로 부터 충돌체의 배율

    Ptr<CMesh>          m_pMesh;            // 충돌체 모양
    Ptr<CMaterial>      m_pMtrl;            // 충돌체 랜더링 재질

    int                 m_iCollisionCount;  // 충돌 횟수


public:
    void SetCollider3DType(COLLIDER3D_TYPE _type);
    COLLIDER3D_TYPE GetCollider3DType() { return m_eColliderType; }

    void SetOffsetPos(Vec3 _vOffsetPos) { m_vOffsetPos = _vOffsetPos; }
    void SetOffsetPos(float _x, float _y, float _z) { m_vOffsetPos = Vec3(_x, _y, _z); }

    Vec3 GetOffsetPos() { return m_vOffsetPos; }

    void SetOffsetScale(Vec3 _vOffsetScale);
    void SetOffsetScale(float _x, float _y, float _z);

    Vec3 GetWorldPos() { return m_matColWorld.Translation(); }
    Vec3 GetWorldScale() { return Vec3(m_vOffsetScale); }

    Matrix GetWorldMat() { return m_matColWorld; }
    int GetCollisionCount() { return m_iCollisionCount; }

public:
    virtual void finalupdate() override;
    virtual void UpdateData() override;
    void render();

public:

    // 이전 프레임에는 충돌 X, 이번 프레임에 충돌 시작일 때
    void OnCollisionEnter(CCollider3D* _Other);

    // 이전 프레임에도 충돌 중, 지금도 충돌 중일 때
    void OnCollision(CCollider3D* _Other);

    // 이전 프레임에 충돌 중, 지금은 충돌 X 일때
    void OnCollisionExit(CCollider3D* _Other);

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;

    virtual void SaveJsonFile(Json::Value& root) override;
    virtual void LoadJsonFromFile(Json::Value& root) override;
    CLONE(CCollider3D)
public:
    CCollider3D();
    CCollider3D(const CCollider3D& _Origin);
    ~CCollider3D();

    friend class CBounding;
    friend class CScript;
};