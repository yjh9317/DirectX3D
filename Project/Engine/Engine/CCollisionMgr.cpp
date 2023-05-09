#include "pch.h"
#include "CCollisionMgr.h"

#include "CResMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CLayer.h"

#include "CGameObject.h"
#include "CCollider2D.h"
#include "CCollider3D.h"
#include "CSocket.h"

CCollisionMgr::CCollisionMgr()
	: m_arrCheck{}
{
}

CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::update()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		for (UINT j = i; j < MAX_LAYER; ++j)
		{
			if (m_arrCheck[i] & (1 << j))
			{
				// i 에 해당하는 EOBJ_TYPE 오브젝트들과, j 에 해당하는EOBJ_TYPE 오브젝트들 끼리 충돌 진행
				CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

				CollisionBetweenLayer(pCurScene->GetLayer(i)->GetObjects()
					, pCurScene->GetLayer(j)->GetObjects());
			}
		}
	}
}

void CCollisionMgr::CollisionBetweenLayer(const vector<CGameObject*>& _left, const vector<CGameObject*>& _right)
{
	CCollider3D* pLeftCol = nullptr;
	CCollider3D* pRightCol = nullptr;

	for (size_t i = 0; i < _left.size(); ++i)
	{
		pLeftCol = _left[i]->Collider3D();

		if (nullptr == pLeftCol)
			continue;

		for (size_t j = 0; j < _right.size(); ++j)
		{
			pRightCol = _right[j]->Collider3D();

			if (nullptr == pRightCol)
				continue;


			// 두 충돌체가 이전에 충돌했었는지 확인
			// 두 충돌체의 조합 아이디 생성
			COLLIDER_ID ID;
			ID.iLeftID = pLeftCol->GetID();
			ID.iRightID = pRightCol->GetID();
			map<long long, bool>::iterator iter = m_mapColInfo.find(ID.id);

			// 두 충돌체는 처음 검사를 진행하고 있다.
			if (iter == m_mapColInfo.end())
			{
				m_mapColInfo.insert(make_pair(ID.id, false));
				iter = m_mapColInfo.find(ID.id);
			}

			// 두 충돌체중 하나라도 데드 상태인지 
			bool bDead = pLeftCol->GetOwner()->IsDead() || pRightCol->GetOwner()->IsDead();

			// 두 충돌체 중 하나라도 비활성화 상태인지
			bool bDeactive = !pLeftCol->GetOwner()->IsActive() || !pRightCol->GetOwner()->IsActive() || !pLeftCol->IsActive() || !pRightCol->IsActive();

			// 이전 프레임에서는 충돌하지 않고 있었고, 현재 둘중 하나 이상이 비활성화 상태이면 충돌 검사를 하지 않겠다.
			if (bDeactive && false == iter->second)
				continue;

			// 충돌 검사
			// 충돌 중이다.
			if (IsCollision(pLeftCol, pRightCol))
			{
				// 이전 프레임에도 충돌 중이었다.
				if (iter->second)
				{
					// 충돌 중이다
					if (bDead || bDeactive)
					{
						// 둘중 하나라도 삭제 예정이라면(삭제를 통한 충돌 해제가 발생하는 것으로 본다)
						pLeftCol->OnCollisionExit(pRightCol);
						pRightCol->OnCollisionExit(pLeftCol);

						if (bDead)
							m_mapColInfo.erase(iter);
						else
							iter->second = false;
					}
					else
					{
						pLeftCol->OnCollision(pRightCol);
						pRightCol->OnCollision(pLeftCol);
					}
				}

				// 이전에는 충돌하지 않고 있었다.
				else
				{
					// 이번에 막 충돌 진입했다.
					if (!bDead)
					{
						// 둘 중 하나라도 삭제 예정이라면 충돌 시작 자체를 하지 않게 한다.
						pLeftCol->OnCollisionEnter(pRightCol);
						pRightCol->OnCollisionEnter(pLeftCol);
						iter->second = true;
					}
				}
			}

			// 충돌하지 않고 있다.
			else
			{
				// 이전 프레임에서는 충돌 중이었다.
				if (iter->second)
				{
					pLeftCol->OnCollisionExit(pRightCol);
					pRightCol->OnCollisionExit(pLeftCol);
					iter->second = false;
				}
			}
		}
	}
}


bool CCollisionMgr::IsCollision(CCollider2D* _pLeftCol, CCollider2D* _pRightCol)
{
	if (_pLeftCol->GetCollider2DType() == COLLIDER2D_TYPE::BOX
		&& _pRightCol->GetCollider2DType() == COLLIDER2D_TYPE::BOX)
	{
		return IsCollision_Box(_pLeftCol, _pRightCol);
	}
	else if (_pLeftCol->GetCollider2DType() == COLLIDER2D_TYPE::CIRCLE
		&& _pRightCol->GetCollider2DType() == COLLIDER2D_TYPE::CIRCLE)
	{
		return IsCollision_Circle(_pLeftCol, _pRightCol);
	}
	else
	{
		return false;
	}
}

bool CCollisionMgr::IsCollision_Box(CCollider2D* _pLeftCol, CCollider2D* _pRightCol)
{
	// 충돌체가 사용하는 기본 도형(사각형) 로컬 정점위치를 알아낸다.
	// 0 -- 1
	// | \  |
	// 3 -- 2	
	static CMesh* pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh").Get();
	static Vtx* pVtx = pRectMesh->GetVtxSysMem();
	static Vec3 vLocalPos[4] = { pVtx[0].vPos, pVtx[1].vPos, pVtx[2].vPos, pVtx[3].vPos };

	Matrix matLeft = _pLeftCol->GetWorldMat();
	Matrix matRight = _pRightCol->GetWorldMat();

	// Local 스페이스의 네개의 정점을 각 충돌체 월드 위치로 보낸다.
	Vec3 vAsix[4] = {};

	// (Vector3, 0.f) X Matirx(4x4)
	// XMVector3TransformNormal();

	// (Vector3, 1.f) X Matirx(4x4)
	// 월드로 보낸 정점을 통해서 각 투영 축 이면서 투영시킬 벡터 성분 4개를 구한다.
	vAsix[0] = XMVector3TransformCoord(vLocalPos[1], matLeft) - XMVector3TransformCoord(vLocalPos[0], matLeft);
	vAsix[1] = XMVector3TransformCoord(vLocalPos[3], matLeft) - XMVector3TransformCoord(vLocalPos[0], matLeft);
	vAsix[2] = XMVector3TransformCoord(vLocalPos[1], matRight) - XMVector3TransformCoord(vLocalPos[0], matRight);
	vAsix[3] = XMVector3TransformCoord(vLocalPos[3], matRight) - XMVector3TransformCoord(vLocalPos[0], matRight);

	// 월드에 배치된 두 충돌체의 중심을 이은 벡터
	//Vec3 vCenter = XMVector3TransformCoord(Vec3::Zero, matRight) - XMVector3TransformCoord(Vec3::Zero, matLeft);	
	Vec3 vCenter = _pRightCol->GetWorldPos() - _pLeftCol->GetWorldPos();

	for (int i = 0; i < 4; ++i)
	{
		vAsix[i].z = 0.f;
		Vec3 vProj = vAsix[i];
		vProj.Normalize();

		float fDist = 0.f;

		for (int j = 0; j < 4; ++j)
		{
			// vProj 에 vAsix[j] 를 투영시킨 길이		
			fDist += abs(vAsix[j].Dot(vProj));
		}
		fDist *= 0.5f;
		float fCenterDist = abs(vCenter.Dot(vProj));

		if (fDist < fCenterDist)
			return false;
	}

	return true;
}

bool CCollisionMgr::IsCollision_Circle(CCollider2D* _pLeftCol, CCollider2D* _pRightCol)
{
	Vec3 vCenter = _pLeftCol->GetWorldPos() - _pRightCol->GetWorldPos();
	float fDist = vCenter.Length();
	float fRadius = fabsf(_pLeftCol->GetWorldScale().x) * 0.5f + fabsf(_pRightCol->GetWorldScale().x) * 0.5f;

	if (fRadius < fDist)
	{
		return false;
	}

	return true;
}

bool CCollisionMgr::IsCollision(CCollider3D* _pLeftCol, CCollider3D* _pRightCol)
{
	if (_pLeftCol->GetCollider3DType() == COLLIDER3D_TYPE::CUBE
		&& _pRightCol->GetCollider3DType() == COLLIDER3D_TYPE::CUBE)
	{
		//return IsCollision_Cube(_pLeftCol, _pRightCol);
		return IsCollision_OBB(_pLeftCol, _pRightCol);
	}
	else if (_pLeftCol->GetCollider3DType() == COLLIDER3D_TYPE::SPHERE
		&& _pRightCol->GetCollider3DType() == COLLIDER3D_TYPE::SPHERE)
	{
		return IsCollision_Sphere(_pLeftCol, _pRightCol);
	}
	else if ((_pLeftCol->GetCollider3DType() == COLLIDER3D_TYPE::SPHERE &&
		_pRightCol->GetCollider3DType() == COLLIDER3D_TYPE::CUBE) ||
		(_pLeftCol->GetCollider3DType() == COLLIDER3D_TYPE::CUBE &&
			_pRightCol->GetCollider3DType() == COLLIDER3D_TYPE::SPHERE))
	{
		return IsCollision_CubeAndSphere(_pRightCol, _pLeftCol);
	}
	else
	{
		return false;
	}
}

bool CCollisionMgr::IsCollision_Cube(CCollider3D* _pLeftCol, CCollider3D* _pRightCol)
{
	Vec3 vCenter = _pRightCol->GetWorldPos() - _pLeftCol->GetWorldPos();

	float fXDist = _pRightCol->GetWorldPos().x - _pLeftCol->GetWorldPos().x;
	float fYDist = _pRightCol->GetWorldPos().y - _pLeftCol->GetWorldPos().y;
	float fZDist = _pRightCol->GetWorldPos().z - _pLeftCol->GetWorldPos().z;

	//float fDist = vCenter.Length();

	Vec3 vLeftScale = _pLeftCol->GetWorldScale();
	Vec3 vRightScale = _pRightCol->GetWorldScale();

	if (abs(_pLeftCol->GetWorldScale().x * 0.5f + _pLeftCol->GetWorldScale().x * 0.5f) < abs(fXDist) ||
		abs(_pLeftCol->GetWorldScale().y * 0.5f + _pLeftCol->GetWorldScale().y * 0.5f) < abs(fYDist) ||
		abs(_pLeftCol->GetWorldScale().z * 0.5f + _pLeftCol->GetWorldScale().z * 0.5f) < abs(fZDist))
	{
		return false;
	}

	return true;
}

bool CCollisionMgr::IsCollision_Sphere(CCollider3D* _pLeftCol, CCollider3D* _pRightCol)
{
	Vec3 vCenter = _pLeftCol->GetWorldPos() - _pRightCol->GetWorldPos();
	float fDist = vCenter.Length();

	float fRadius = fabsf(_pLeftCol->GetWorldScale().x) * 0.5f + fabsf(_pRightCol->GetWorldScale().x) * 0.5f;

	if (fRadius < fDist)
	{
		return false;
	}

	return true;
}

bool CCollisionMgr::IsCollision_OBB(CCollider3D* _pLeftCol, CCollider3D* _pRightCol)
{
	// 충돌체가 사용하는 기본 도형(사각형) 로컬 정점위치를 알아낸다.
	//    0 ㅡ 1
	//   /|   /|
	//  3 ㅡ 2 |	
	//  | 7 '|'/6
	//  |____|/
	//  4    5  
	//
	// Cube 메쉬의 정점위치

	// 메쉬로부터 정점의 로컬 위치를 저장
	static CMesh* pCubeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh").Get();
	static Vtx* pVtx = pCubeMesh->GetVtxSysMem();
	static Vec3 vLocalPos[8] = { pVtx[0].vPos, pVtx[1].vPos, pVtx[2].vPos, pVtx[3].vPos,
								pVtx[4].vPos, pVtx[5].vPos, pVtx[6].vPos, pVtx[7].vPos };

	Matrix matLeft = _pLeftCol->GetWorldMat();
	Matrix matRight = _pRightCol->GetWorldMat();


	// Local 스페이스의 여섯 개의 정점을 각 충돌체의 월드행렬을 곱해서 월드 위치로 보낸다.
	Vec3 vAsix[6] = {};

	// (Vector3, 1.f) X Matirx(4x4)
	// 월드로 보낸 정점을 통해서 각 투영 축 이면서 투영시킬 벡터 성분 6개를 구한다.
	vAsix[0] = XMVector3TransformCoord(vLocalPos[1], matLeft) - XMVector3TransformCoord(vLocalPos[0], matLeft);
	vAsix[1] = XMVector3TransformCoord(vLocalPos[3], matLeft) - XMVector3TransformCoord(vLocalPos[0], matLeft);
	vAsix[2] = XMVector3TransformCoord(vLocalPos[1], matRight) - XMVector3TransformCoord(vLocalPos[0], matRight);
	vAsix[3] = XMVector3TransformCoord(vLocalPos[3], matRight) - XMVector3TransformCoord(vLocalPos[0], matRight);
	vAsix[4] = XMVector3TransformCoord(vLocalPos[7], matLeft) - XMVector3TransformCoord(vLocalPos[0], matLeft);
	vAsix[5] = XMVector3TransformCoord(vLocalPos[7], matRight) - XMVector3TransformCoord(vLocalPos[0], matRight);

	// 두 물체의 중심간의 거리
	Vec3 vCenter = _pRightCol->GetWorldPos() - _pLeftCol->GetWorldPos();

	// 각 물체의 면에 해당하는 축에 투영하여 중심간의 거리와 거리값을 비교
	for (int i = 0; i < 6; ++i)
	{
		Vec3 vProj = vAsix[i];
		vProj.Normalize();

		float fDist = 0.f;

		for (int j = 0; j < 6; ++j)
		{
			// vProj 에 vAsix[j] 를 투영시킨 길이		
			fDist += abs(vAsix[j].Dot(vProj));
		}
		fDist *= 0.5f;
		float fCenterDist = abs(vCenter.Dot(vProj));

		if (fDist < fCenterDist)
			return false;
	}

	// 왼쪽 충돌체의 축과 오른쪽 충돌체의 축을 설정
	Vec3 _LeftAxis[3] = { XMVector3TransformCoord(vLocalPos[1], matLeft) - XMVector3TransformCoord(vLocalPos[0], matLeft) ,		// x축
						XMVector3TransformCoord(vLocalPos[3], matLeft) - XMVector3TransformCoord(vLocalPos[0], matLeft) ,		// z축
						XMVector3TransformCoord(vLocalPos[7], matLeft) - XMVector3TransformCoord(vLocalPos[0], matLeft) };		// y축

	Vec3 _RightAxis[3] = { XMVector3TransformCoord(vLocalPos[1], matRight) - XMVector3TransformCoord(vLocalPos[0], matRight) ,  // x축
						XMVector3TransformCoord(vLocalPos[3], matRight) - XMVector3TransformCoord(vLocalPos[0], matRight) ,		// z축
						XMVector3TransformCoord(vLocalPos[7], matRight) - XMVector3TransformCoord(vLocalPos[0], matRight) };	// y축

	// 두 물체의 축을 하나씩 받아와서 외적으로 수직인 벡터를 구하고 그 벡터에 투영해서 중심간의 거리와 거리값을 비교

	for (int i = 0; i < 3; ++i)
	{

		for (int j = 0; j < 3; ++j)
		{
			float fCenterDist;
			float fDist = 0.f;
			Vec3 Left = _LeftAxis[i];
			Vec3 Right = _RightAxis[j];
			// 두 충돌체의 축을 하나씩 받아와서 외적으로 구한 수직벡터
			Vec3 vCross = Left.Cross(Right);

			// 양쪽 충돌체의 각 축들을 수직인 벡터에 투영한 길이의 합
			fDist += abs(vCross.Dot(_LeftAxis[0]));
			fDist += abs(vCross.Dot(_LeftAxis[1]));
			fDist += abs(vCross.Dot(_LeftAxis[2]));
			fDist += abs(vCross.Dot(_RightAxis[0]));
			fDist += abs(vCross.Dot(_RightAxis[1]));
			fDist += abs(vCross.Dot(_RightAxis[2]));

			// 중심간의 거리
			fCenterDist = abs(vCenter.Dot(vCross));

			// 로컬 위치의 기본 정점들의 길이가 0.5이므로 0.5를 곱함.
			fDist *= 0.5f;

			// 중심간의 거리가 더 길면 충돌 X
			if (fDist < fCenterDist)
				return false;
		}

	}
	return true;
}

bool CCollisionMgr::IsCollision_CubeAndSphere(CCollider3D* _pLeftCol, CCollider3D* _pRightCol)
{
	CCollider3D* pCube = nullptr;
	CCollider3D* pSphere = nullptr;

	if (_pLeftCol->GetCollider3DType() == COLLIDER3D_TYPE::CUBE)
	{
		pCube = _pLeftCol;
		pSphere = _pRightCol;
	}
	else if (_pLeftCol->GetCollider3DType() == COLLIDER3D_TYPE::SPHERE)
	{
		pCube = _pRightCol;
		pSphere = _pLeftCol;
	}

	Vec3 vDir = pCube->GetWorldPos() - pSphere->GetWorldPos();
	Vec3 vScale = pSphere->GetWorldScale() * 0.5f;

	Matrix CubeMat = pCube->GetWorldMat();

	Vec3& axisX = CubeMat.Right().Normalize();
	float distanceX = Clamp(vDir.Dot(axisX), -vScale.x, vScale.x);
	Vec3& axisY = CubeMat.Up().Normalize();
	float distanceY = Clamp(vDir.Dot(axisY), -vScale.y, vScale.y);
	Vec3& axisZ = CubeMat.Forward().Normalize();
	float distanceZ = Clamp(vDir.Dot(axisZ), -vScale.z, vScale.z);

	Vec3 value = pSphere->GetWorldPos() + axisX * distanceX + axisY * distanceY + axisZ * distanceZ;

	float Dist = ((value - pCube->GetWorldPos()).Length());
	float radius = pSphere->GetWorldScale().x / 2.f;

	if (Dist < radius)
		return true;

	return false;
}

float CCollisionMgr::Clamp(float _f1, float _f2, float _f3)
{

	float temp = _f1;
	if (temp < _f2)
	{
		temp = _f2;
	}
	else if (temp > _f3)
	{
		temp = _f3;
	}

	return temp;
}


void CCollisionMgr::CollisionCheck(int _iLayerLeftIdx, int _iLayerRightIdx)
{
	UINT iCol = 0, iRow = 0;

	if ((UINT)_iLayerLeftIdx > (UINT)_iLayerRightIdx)
	{
		iCol = (UINT)_iLayerLeftIdx;
		iRow = (UINT)_iLayerRightIdx;
	}
	else
	{
		iCol = (UINT)_iLayerRightIdx;
		iRow = (UINT)_iLayerLeftIdx;
	}

	m_arrCheck[iRow] |= (1 << iCol);
}

void CCollisionMgr::CollisionOff(int _iLayerLeftIdx, int _iLayerRightIdx)
{
	UINT iCol = 0, iRow = 0;

	if ((UINT)_iLayerLeftIdx > (UINT)_iLayerRightIdx)
	{
		iCol = (UINT)_iLayerLeftIdx;
		iRow = (UINT)_iLayerRightIdx;
	}
	else
	{
		iCol = (UINT)_iLayerRightIdx;
		iRow = (UINT)_iLayerLeftIdx;
	}

	m_arrCheck[iRow] &= ~(1 << iCol);
}

void CCollisionMgr::CollisionCheck(const wstring& _strLeftName, const wstring& _strRightName)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLeftLayer = pCurScene->GetLayer(_strLeftName);
	CLayer* pRightLayer = pCurScene->GetLayer(_strRightName);

	if (pLeftLayer && pRightLayer)
		CollisionCheck(pLeftLayer->GetLayerIdx(), pRightLayer->GetLayerIdx());
}

void CCollisionMgr::CollisionOff(const wstring& _strLeftName, const wstring& _strRightName)
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLeftLayer = pCurScene->GetLayer(_strLeftName);
	CLayer* pRightLayer = pCurScene->GetLayer(_strRightName);

	if (pLeftLayer && pRightLayer)
		CollisionOff(pLeftLayer->GetLayerIdx(), pRightLayer->GetLayerIdx());
}
