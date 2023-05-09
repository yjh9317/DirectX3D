#include "pch.h"
#include "CSocket.h"

#include "CTransform.h"
#include "CMeshRender.h"
#include "CAnimator3D.h"

#include "CConstBuffer.h"

CSocket::CSocket()
	:CComponent(COMPONENT_TYPE::SOCKET)
	,m_vScale(Vec3(1.f,1.f,1.f))
{
}

CSocket::~CSocket()
{
}


void CSocket::finalupdate()
{
}


Matrix CSocket::ConvertColToRow(Matrix _mat)
{
	Matrix _Temp;

	_Temp._11 = _mat._11;	_Temp._12 = _mat._21;	_Temp._13 = _mat._31;	_Temp._14 = _mat._41;
	_Temp._21 = _mat._12;	_Temp._22 = _mat._22;	_Temp._23 = _mat._32;	_Temp._24 = _mat._42;
	_Temp._31 = _mat._13;	_Temp._32 = _mat._23;	_Temp._33 = _mat._33;	_Temp._34 = _mat._43;
	_Temp._41 = _mat._14;	_Temp._42 = _mat._24;	_Temp._43 = _mat._34;	_Temp._44 = 1;

	return _Temp;
}

void CSocket::Regist()
{
	CGameObject* pParent = GetOwner()->GetParent();
	if (pParent)
	{
		CAnimator3D* pAnim = pParent->Animator3D();

		if (pAnim)
		{
			pBuffer = pAnim->GetSocket();
			pBuffer->UpdateData(PIPELINE_STAGE::VS, 19);			

			pBuffer2 = pAnim->GetSocket2();
			pBuffer2->UpdateData(PIPELINE_STAGE::VS, 20);
			
			Matrix matScale = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);
			Matrix matTranslation = XMMatrixTranslation(m_vTransform.x, m_vTransform.y, m_vTransform.z);

			Matrix matRotX = XMMatrixRotationX(m_vRotation.x);
			Matrix matRotY = XMMatrixRotationY(m_vRotation.y);
			Matrix matRotZ = XMMatrixRotationZ(m_vRotation.z);
			Matrix matRelativeRot = matRotX * matRotY * matRotZ;
			
			// 애니메이션에 저장돼 있던 소켓을 가져온다.
			Matrix SocketMat{};
			pBuffer->GetData(&SocketMat);

			// HLSL에서 사용하기 위해 열우선 행렬 -> 행우선 행렬로 변경
			Matrix RowSocket = ConvertColToRow(SocketMat);
			// World 행렬 만들기
			m_matWorld = matScale * matRelativeRot * matTranslation;
			m_Finalmat = m_matWorld * RowSocket * g_transform.matWorld;

			// 만약 충돌체크가 되어있다면  Final행렬을 통해 소켓Transform을 사용
			if (m_bCollision)
			{
				g_transform.matWorld = m_Finalmat;
				g_transform.matWV = g_transform.matWorld * g_transform.matView;
				g_transform.matWVP = g_transform.matWV * g_transform.matProj;

				CConstBuffer* pBuffer = CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);
				pBuffer->SetData(&g_transform, sizeof(tTransform));
				pBuffer->UpdateData();
			}
			MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::MAT_3, &m_matWorld);
		}
	}
}

void CSocket::Clear()
{
	if (nullptr != pBuffer)
	{
		pBuffer->Clear();
		pBuffer = nullptr;
	}

	if (nullptr != pBuffer2)
	{
		pBuffer2->Clear();
		pBuffer2 = nullptr;
	}
}

void CSocket::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);
	fwrite(&m_vTransform, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vScale, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vRotation, sizeof(Vec3), 1, _pFile);
	fwrite(&m_bCollision, sizeof(bool), 1, _pFile);
}

void CSocket::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);

	fread(&m_vTransform, sizeof(Vec3), 1, _pFile);
	fread(&m_vScale, sizeof(Vec3), 1, _pFile);
	fread(&m_vRotation, sizeof(Vec3), 1, _pFile);
	fread(&m_bCollision, sizeof(bool), 1, _pFile);
}

void CSocket::SaveJsonFile(Json::Value& root)
{
	Json::Value Socket;

	VecToJson(&Socket, "Transform", m_vTransform);
	VecToJson(&Socket, "Scale", m_vScale);
	VecToJson(&Socket, "Rotation", m_vRotation);
	Socket["Collision"] = m_bCollision;

	root["SOCKET"] = Socket;
}

void CSocket::LoadJsonFromFile(Json::Value& root)
{
	Json::Value Socket = root;

	LoadVecFromJson(Socket["Transform"], m_vTransform);
	LoadVecFromJson(Socket["Scale"], m_vScale);
	LoadVecFromJson(Socket["Rotation"], m_vRotation);

	m_bCollision = Socket["Collision"].asBool();

}

