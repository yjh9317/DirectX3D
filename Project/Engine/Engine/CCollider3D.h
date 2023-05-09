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

    Vec3                m_vOffsetPos;       // ������Ʈ�� ������ �浹ü�� ��� �Ÿ�
    Vec3                m_vOffsetScale;     // ������Ʈ ũ��� ���� �浹ü�� ����

    Ptr<CMesh>          m_pMesh;            // �浹ü ���
    Ptr<CMaterial>      m_pMtrl;            // �浹ü ������ ����

    int                 m_iCollisionCount;  // �浹 Ƚ��


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

    // ���� �����ӿ��� �浹 X, �̹� �����ӿ� �浹 ������ ��
    void OnCollisionEnter(CCollider3D* _Other);

    // ���� �����ӿ��� �浹 ��, ���ݵ� �浹 ���� ��
    void OnCollision(CCollider3D* _Other);

    // ���� �����ӿ� �浹 ��, ������ �浹 X �϶�
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