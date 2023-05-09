#pragma once

class CGameObject;
class CCollider2D;
class CCollider3D;

union COLLIDER_ID
{
	struct {
		UINT iLeftID;
		UINT iRightID;
	};
	long long id;
};

class CCollisionMgr
	: public CSingleton<CCollisionMgr>
{
	SINGLE(CCollisionMgr);
private:
	UINT					m_arrCheck[MAX_LAYER];
	map<long long, bool>	m_mapColInfo;	// �浹 ���� ���� Ű


public:
	void update();

public:
	void CollisionCheck(const wstring& _strLeftName, const wstring& _strRightName);
	void CollisionOff(const wstring& _strLeftName, const wstring& _strRightName);	

	void CollisionCheck(int _iLayerLeftIdx, int _iLayerRightIdx);
	void CollisionOff(int _iLayerLeftIdx, int _iLayerRightIdx);

private:
	void CollisionBetweenLayer(const vector<CGameObject*>& _left, const vector<CGameObject*>& _right);
	bool IsCollision(CCollider2D* _pLeftCol, CCollider2D* _pRightCol);
	bool IsCollision_Box(CCollider2D* _pLeftCol, CCollider2D* _pRightCol);
	bool IsCollision_Circle(CCollider2D* _pLeftCol, CCollider2D* _pRightCol);

	bool IsCollision(CCollider3D* _pLeftCol, CCollider3D* _pRightCol);
	bool IsCollision_Cube(CCollider3D* _pLeftCol, CCollider3D* _pRightCol);
	bool IsCollision_Sphere(CCollider3D* _pLeftCol, CCollider3D* _pRightCol);
	bool IsCollision_OBB(CCollider3D* _pLeftCol, CCollider3D* _pRightCol);
	bool IsCollision_CubeAndSphere(CCollider3D* _pLeftCol, CCollider3D* _pRightCol);

	float Clamp(float _f1, float _f2, float _f3);
};

