#include "pch.h"
#include "CGameObject.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CLayer.h"
#include "CEventMgr.h"

#include "CComponent.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CCollider2D.h"
#include "CCollider3D.h"
#include "CLight2D.h"
#include "CLight3D.h"
#include "CBounding.h"
#include "CSocket.h"
#include "CRenderComponent.h"
#include "CAnimator3D.h"
#include "CLandScape.h"
#include "CSkyBox.h"
#include "CDecal.h"

#include "CScript.h"


CGameObject::CGameObject()
	: m_arrCom{}
	, m_pParent(nullptr)
	, m_pRenderComponent(nullptr)
	, m_iLayerIdx(-1)
	, m_bDead(false)
	, m_bActive(true)
	, m_bDynamicShadow(false)
	, m_bFrustumCulling(false)
{
}

CGameObject::CGameObject(const CGameObject& _origin)
	: CEntity(_origin)
	, m_arrCom{}
	, m_pParent(nullptr)
	, m_pRenderComponent(nullptr)
	, m_iLayerIdx(-1)
	, m_bActive(true)
	, m_bDead(false)
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != _origin.m_arrCom[i])
		{
			AddComponent(_origin.m_arrCom[i]->Clone());
		}
	}

	for (size_t i = 0; i < _origin.m_vecScript.size(); ++i)
	{
		AddComponent(_origin.m_vecScript[i]->Clone());
	}

	for (size_t i = 0; i < _origin.m_vecChild.size(); ++i)
	{
		AddChild(_origin.m_vecChild[i]->Clone());
	}
}

CGameObject::~CGameObject()
{
	Safe_Del_Arr(m_arrCom);
	Safe_Del_Vec(m_vecScript);
	Safe_Del_Vec(m_vecChild);
}

void CGameObject::start()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->start();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->start();
	}


	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->start();
	}
}

void CGameObject::update()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i] && m_arrCom[i]->IsActive())
			m_arrCom[i]->update();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		if (m_vecScript[i]->IsActive())
			m_vecScript[i]->update();
	}



	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->IsActive())
			m_vecChild[i]->update();
	}
}

void CGameObject::lateupdate()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i] && m_arrCom[i]->IsActive())
			m_arrCom[i]->lateupdate();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		if (m_vecScript[i]->IsActive())
			m_vecScript[i]->lateupdate();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->IsActive())
			m_vecChild[i]->lateupdate();
	}
}

void CGameObject::finalupdate()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finalupdate();
	}

	// Layer �� ���
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pCurScene->GetLayer(m_iLayerIdx);
	pLayer->RegisterObject(this);

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->finalupdate();
	}
}

void CGameObject::finalupdate_module()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finalupdate_module();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->finalupdate_module();
	}
}

void CGameObject::render()
{
	if (m_pRenderComponent->IsActive())
		m_pRenderComponent->render();

	//if (nullptr != Collider3D())
	//	Collider3D()->render();
}

CScript* CGameObject::GetScript(UINT _iIdx)
{
	return m_vecScript[_iIdx];
}

CScript* CGameObject::GetScriptByName(const wstring& _strName)
{
	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		if (m_vecScript[i]->GetName() == _strName)
			return m_vecScript[i];
	}

	return nullptr;
}


void CGameObject::active()
{
	for (UINT i = 1; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->active();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->active();
	}


	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->active();
	}
}

void CGameObject::deactive()
{
	for (UINT i = 1; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->deactive();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->deactive();
	}

	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->deactive();
	}
}



void CGameObject::Deregister()
{
	if (-1 == m_iLayerIdx)
	{
		return;
	}

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pCurLayer = pCurScene->GetLayer(m_iLayerIdx);
	pCurLayer->DeregisterObject(this);
}

void CGameObject::DisconnectBetweenParent()
{
	assert(m_pParent);

	vector<CGameObject*>::iterator iter = m_pParent->m_vecChild.begin();
	for (; iter != m_pParent->m_vecChild.end(); ++iter)
	{
		if ((*iter) == this)
		{
			m_pParent->m_vecChild.erase(iter);
			m_pParent = nullptr;
			return;
		}
	}
}

void CGameObject::Activate()
{
	tEventInfo info = {};

	info.eType = EVENT_TYPE::ACTIVATE_OBJECT;
	info.lParam = (DWORD_PTR)this;

	CEventMgr::GetInst()->AddEvent(info);
}

void CGameObject::Deactivate()
{
	tEventInfo info = {};

	info.eType = EVENT_TYPE::DEACTIVATE_OBJECT;
	info.lParam = (DWORD_PTR)this;

	CEventMgr::GetInst()->AddEvent(info);
}

bool CGameObject::IsAncestor(CGameObject* _pObj)
{
	CGameObject* pObj = m_pParent;

	while (pObj)
	{
		if (pObj == _pObj)
			return true;

		pObj = pObj->m_pParent;
	}

	return false;
}

void CGameObject::AddChild(CGameObject* _pChild)
{
	int iLayerIdx = _pChild->m_iLayerIdx;

	// �ڽ����� ������ ������Ʈ�� ��Ʈ ������Ʈ�̰�, Ư�� ���̾� �Ҽ��̶��
	if (nullptr == _pChild->GetParent() && -1 != iLayerIdx)
	{
		// ���̾�� ��Ʈ ������Ʈ�μ� ��� ����
		_pChild->Deregister();
		_pChild->m_iLayerIdx = iLayerIdx;
	}

	// �ٸ� �θ������Ʈ�� �̹� �ִٸ�
	if (_pChild->GetParent())
	{
		_pChild->DisconnectBetweenParent();
	}


	m_vecChild.push_back(_pChild);
	_pChild->m_pParent = this;
}

void CGameObject::AddComponent(CComponent* _component)
{
	COMPONENT_TYPE eType = _component->GetType();

	if (COMPONENT_TYPE::SCRIPT != eType)
	{
		assert(nullptr == m_arrCom[(UINT)eType]);

		m_arrCom[(UINT)eType] = _component;
		_component->m_pOwner = this;

		switch (_component->GetType())
		{
		case COMPONENT_TYPE::MESHRENDER:
		case COMPONENT_TYPE::TILEMAP:
		case COMPONENT_TYPE::PARTICLESYSTEM:
		case COMPONENT_TYPE::LANDSCAPE:
		case COMPONENT_TYPE::DECAL:
		case COMPONENT_TYPE::SKYBOX:
		{
			// �ϳ��� ������Ʈ�� Render ����� ���� ������Ʈ�� 2���̻� ���� �� ����.
			assert(!m_pRenderComponent);
			m_pRenderComponent = (CRenderComponent*)_component;
		}
		break;
		}
	}
	else
	{
		m_vecScript.push_back((CScript*)_component);
		_component->m_pOwner = this;
	}
}

void CGameObject::Destroy()
{
	if (m_bDead)
		return;

	tEventInfo info = {};

	info.eType = EVENT_TYPE::DELETE_OBJ;
	info.lParam = (DWORD_PTR)this;

	CEventMgr::GetInst()->AddEvent(info);
}


#include "CCamera.h"
#include "CCollider2D.h"
//#include "CCollider3D.h"
#include "CAnimator2D.h"
//#include "CAnimator3D.h"
#include "CParticleSystem.h"
#include "CTileMap.h"


void CGameObject::SaveToScene(FILE* _pFile)
{
	CEntity::SaveToScene(_pFile);

	fwrite(&m_bActive, sizeof(BYTE), 3, _pFile);

	// Component ����
	for (int i = 0; i < (int)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			SaveWStringToFile(ToWString((COMPONENT_TYPE)i), _pFile);
			m_arrCom[i]->SaveToScene(_pFile);

		}
	}
	SaveWStringToFile(L"END", _pFile);
}

void CGameObject::LoadFromScene(FILE* _pFile)
{
	CEntity::LoadFromScene(_pFile);
	fread(&m_bActive, sizeof(BYTE), 3, _pFile);

	// Component �ҷ�����
	wstring strComponentName;

	while (true)
	{
		LoadWStringFromFile(strComponentName, _pFile);
		if (strComponentName == L"END")
			break;

		if (strComponentName == ToWString(COMPONENT_TYPE::TRANSFORM))
		{
			AddComponent(new CTransform);
			Transform()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::CAMERA))
		{
			AddComponent(new CCamera);
			Camera()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::COLLIDER2D))
		{
			AddComponent(new CCollider2D);
			Collider2D()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::COLLIDER3D))
		{
			AddComponent(new CCollider3D);
			Collider3D()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::ANIMATOR2D))
		{
			AddComponent(new CAnimator2D);
			Animator2D()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::ANIMATOR3D))
		{
			AddComponent(new CAnimator3D);
			Animator3D()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::LIGHT2D))
		{
			AddComponent(new CLight2D);
			Light2D()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::LIGHT3D))
		{
			AddComponent(new CLight3D);
			Light3D()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::BOUNDING))
		{
			AddComponent(new CBounding);
			Bounding()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::SOCKET))
		{
			AddComponent(new CSocket);
			Socket()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::MESHRENDER))
		{
			AddComponent(new CMeshRender);
			MeshRender()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::PARTICLESYSTEM))
		{
			AddComponent(new CParticleSystem);
			ParticleSystem()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::TILEMAP))
		{
			AddComponent(new CTileMap);
			TileMap()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::SKYBOX))
		{
			AddComponent(new CSkyBox);
			SkyBox()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::LANDSCAPE))
		{
			AddComponent(new CLandScape);
			LandScape()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::DECAL))
		{
			AddComponent(new CDecal);
			Decal()->LoadFromScene(_pFile);
		}
		else if (strComponentName == ToWString(COMPONENT_TYPE::FSM))
		{
			AddComponent(new CFSM);
			FSM()->LoadFromScene(_pFile);
		}

	}
}



void CGameObject::SaveJsonFile(Json::Value& root)
{
	Json::Value GameObject;
	string str = string(GetName().begin(), GetName().end());
	GameObject["Name"] = str;
	GameObject["Active"] = m_bActive;

	// Component ��� ����
	Json::Value Component;
	for (int i = 0; i < (int)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			string ComponentName = ToString((COMPONENT_TYPE)i);;
			m_arrCom[i]->SaveJsonFile(GameObject);
			Component.append(ComponentName);
		}
	}

	GameObject["Component"] = Component;
	root["ObjectInfo"] = GameObject;

}

void CGameObject::LoadJsonFromFile(Json::Value& root)
{
	Json::Value GameObject = root["ObjectInfo"];
	// �̸� , active
	m_bActive = GameObject["Active"].asBool();
	string tmp = GameObject["Name"].asString();
	wstring wstr = wstring(tmp.begin(), tmp.end());
	SetName(wstr);


	// Component �ҷ�����
	wstring strComponentName;
	Json::Value Component = GameObject["Component"];

	Json::ValueIterator iter = Component.begin();

	for (; iter != Component.end(); iter++)
	{
		string ComponentName = (*iter).asString();
		Json::Value CurrentJsonComponent = GameObject[ComponentName];

		// ToString�� ��ȯ�� ���� �빮���̹Ƿ� �� ������Ʈ�� LoadJsonFromFile���� ����� Json Component�� �̸��� �빮�ڷ� �ۼ��ؾ��Ѵ�.
		if (ComponentName == ToString(COMPONENT_TYPE::TRANSFORM))
		{
			AddComponent(new CTransform);
			Transform()->LoadJsonFromFile(CurrentJsonComponent);
		}
		else if (ComponentName == ToString(COMPONENT_TYPE::CAMERA))
		{
			AddComponent(new CCamera);
			Camera()->LoadJsonFromFile(CurrentJsonComponent);
		}
		else if (ComponentName == ToString(COMPONENT_TYPE::COLLIDER2D))
		{
			AddComponent(new CCollider2D);
			Collider2D()->LoadJsonFromFile(CurrentJsonComponent);
		}
		else if (ComponentName == ToString(COMPONENT_TYPE::COLLIDER3D))
		{
			AddComponent(new CCollider3D);
			Collider3D()->LoadJsonFromFile(CurrentJsonComponent);
		}
		else if (ComponentName == ToString(COMPONENT_TYPE::ANIMATOR2D))
		{
			AddComponent(new CAnimator2D);
			Animator2D()->LoadJsonFromFile(CurrentJsonComponent);
		}
		else if (ComponentName == ToString(COMPONENT_TYPE::ANIMATOR3D))
		{
			AddComponent(new CAnimator3D);
			Animator3D()->LoadJsonFromFile(CurrentJsonComponent);
		}
		else if (ComponentName == ToString(COMPONENT_TYPE::LIGHT2D))
		{
			AddComponent(new CLight2D);
			Light2D()->LoadJsonFromFile(CurrentJsonComponent);
		}
		else if (ComponentName == ToString(COMPONENT_TYPE::LIGHT3D))
		{
			AddComponent(new CLight3D);
			Light3D()->LoadJsonFromFile(CurrentJsonComponent);
		}
		else if (ComponentName == ToString(COMPONENT_TYPE::BOUNDING))
		{
			AddComponent(new CBounding);
			Bounding()->LoadJsonFromFile(CurrentJsonComponent);
		}
		else if (ComponentName == ToString(COMPONENT_TYPE::SOCKET))
		{
			AddComponent(new CSocket);
			Socket()->LoadJsonFromFile(CurrentJsonComponent);
		}
		else if (ComponentName == ToString(COMPONENT_TYPE::MESHRENDER))
		{
			AddComponent(new CMeshRender);
			MeshRender()->LoadJsonFromFile(CurrentJsonComponent);
		}
		else if (ComponentName == ToString(COMPONENT_TYPE::PARTICLESYSTEM))
		{
			AddComponent(new CParticleSystem);
			ParticleSystem()->LoadJsonFromFile(CurrentJsonComponent);
		}
		else if (ComponentName == ToString(COMPONENT_TYPE::TILEMAP))
		{
			AddComponent(new CTileMap);
			TileMap()->LoadJsonFromFile(CurrentJsonComponent);
		}
		else if (ComponentName == ToString(COMPONENT_TYPE::SKYBOX))
		{
			AddComponent(new CSkyBox);
			SkyBox()->LoadJsonFromFile(CurrentJsonComponent);
		}
		else if (ComponentName == ToString(COMPONENT_TYPE::LANDSCAPE))
		{
			AddComponent(new CLandScape);
			LandScape()->LoadJsonFromFile(CurrentJsonComponent);
		}
		else if (ComponentName == ToString(COMPONENT_TYPE::DECAL))
		{
			AddComponent(new CDecal);
			Decal()->LoadJsonFromFile(CurrentJsonComponent);
		}
		else if (ComponentName == ToString(COMPONENT_TYPE::FSM))
		{
			AddComponent(new CFSM);
			FSM()->LoadJsonFromFile(CurrentJsonComponent);
		}

	}
}
