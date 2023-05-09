#include "pch.h"
#include "CFrustum.h"

#include "CCamera.h"

CFrustum::CFrustum()
	: m_ProjPos{}
	, m_WorldPos{}
	, m_arrPlane{}
	, m_pCam(nullptr)
{
	//   4 ---- 5
	//  /|    / |
	// 0 -- 1   |
	// | 6 -|-- 7
	// 2 -- 3/
	m_ProjPos[0] = Vec3(-1.f, 1.f, 0.f);
	m_ProjPos[1] = Vec3(1.f, 1.f, 0.f);
	m_ProjPos[2] = Vec3(-1.f, -1.f, 0.f);
	m_ProjPos[3] = Vec3(1.f, -1.f, 0.f);

	m_ProjPos[4] = Vec3(-1.f, 1.f, 1.f);
	m_ProjPos[5] = Vec3(1.f, 1.f,	1.f);
	m_ProjPos[6] = Vec3(-1.f, -1.f,1.f);
	m_ProjPos[7] = Vec3(1.f, -1.f, 1.f);
}

CFrustum::~CFrustum()
{
}

void CFrustum::finalupdate()
{
	// Frustum �� �����ϰ� �ִ� ī�޶��� Proj �����, View ����� �� �����´�.
	const Matrix& matViewInv = m_pCam->GetViewInvMat();
	const Matrix& matProjInv = m_pCam->GetProjInvMat();

	// VP ������� ���ؼ� WorldPos �� ���Ѵ�.
	Matrix matVPInv = matProjInv * matViewInv;

	for (int i = 0; i < 8; ++i)
	{
		m_WorldPos[i] = XMVector3TransformCoord(m_ProjPos[i], matVPInv);
	}

	// 8���� ���� ��ǥ�� �̿��ؼ� ����󿡼� ����ü�� �����ϴ� 6���� ����� �����Ѵ�.
	//   4 ---- 5
	//  /|    / |
	// 0 -- 1   |
	// | 6 -|-- 7
	// 2 -- 3/
	m_arrPlane[(UINT)PLANE::PL_LEFT] = XMPlaneFromPoints(m_WorldPos[4], m_WorldPos[0], m_WorldPos[2]);
	m_arrPlane[(UINT)PLANE::PL_RIGHT] = XMPlaneFromPoints(m_WorldPos[1], m_WorldPos[5], m_WorldPos[7]);

	m_arrPlane[(UINT)PLANE::PL_UP] = XMPlaneFromPoints(m_WorldPos[0], m_WorldPos[4], m_WorldPos[5]);
	m_arrPlane[(UINT)PLANE::PL_DOWN] = XMPlaneFromPoints(m_WorldPos[2], m_WorldPos[3], m_WorldPos[7]);

	m_arrPlane[(UINT)PLANE::PL_NEAR] = XMPlaneFromPoints(m_WorldPos[2], m_WorldPos[0], m_WorldPos[1]);
	m_arrPlane[(UINT)PLANE::PL_FAR] = XMPlaneFromPoints(m_WorldPos[7], m_WorldPos[5], m_WorldPos[4]);
}

bool CFrustum::PointCheck(Vec3 _vPos)
{
	for (int i = 0; i < (UINT)PLANE::END; ++i)
	{
		// N dot P = D
		float fDot = _vPos.Dot(m_arrPlane[i]);
		if (fDot + m_arrPlane[i].w > 0)
		{
			return false;
		}
	}

	return true;
}

bool CFrustum::SphereCheck(Vec3 _vPos, float _fRadius)
{
	for (int i = 0; i < (UINT)PLANE::END; ++i)
	{
		// N dot P = D
		float fDot = _vPos.Dot(m_arrPlane[i]);
		if (fDot + m_arrPlane[i].w > _fRadius)
		{
			return false;
		}
	}

	return true;
}
