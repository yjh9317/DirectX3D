#include "pch.h"
#include "SceneOutliner.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

#include <Engine/CEventMgr.h>
#include <Script/CSceneSaveLoad.h>

#include "TreeUI.h"
#include "CImGuiMgr.h"
#include "InspectorUI.h"
#include <Engine/CTransform.h>

SceneOutliner::SceneOutliner()
	: UI("SceneOutliner")
{
	m_TreeUI = new TreeUI(true);
	m_TreeUI->SetTitle("SceneOutliner");
	m_TreeUI->UseFrame(false);
	m_TreeUI->ShowDummyRoot(false);
	m_TreeUI->UseDragDropOuter(true);
	m_TreeUI->UseDragDropSelf(true);


	AddChild(m_TreeUI);

	// Clicked Delegate ���
	m_TreeUI->SetClickedDelegate(this, (CLICKED)&SceneOutliner::ObjectClicked);

	// Drag and Drop Delegate ���
	m_TreeUI->SetDragAndDropDelegate(this, (DRAG_DROP)&SceneOutliner::DragAndDropDelegate);

	// Key Delegate ���
	m_TreeUI->SetKeyBinding(KEY::DEL, this, (CLICKED)&SceneOutliner::PressDelete);
	

	Reset();
}

SceneOutliner::~SceneOutliner()
{

}

void SceneOutliner::update()
{
	if (CEventMgr::GetInst()->HasOccurObjEvent())
	{
		Reset();
	}

	UI::update();
}

void SceneOutliner::render_update()
{
	if(ImGui::Button("Prefab"))
	{
		AddPrefab();
	}

	ImGui::SameLine();

	if (ImGui::Button("Delete"))
	{
		AddDelete();
	}

	ImGui::Separator();


}

void SceneOutliner::Reset()
{
	m_TreeUI->Clear();

	// ���� Scene �� �����´�.
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pCurScene->GetLayer(i);

		vector<CGameObject*>& vecRoots = pLayer->GetRootObjects();

		for (size_t i = 0; i < vecRoots.size(); ++i)
		{
			AddGameObjectToTree(vecRoots[i], m_TreeUI->GetDummyNode());
		}
	}

	// InspectorUI �� ����
	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	pInspectorUI->SetTargetObject(nullptr);
}

void SceneOutliner::ObjectClicked(DWORD_PTR _dw)
{
	TreeNode* pNode = (TreeNode*)_dw;

	string strKey = pNode->GetName();
	CGameObject* pObject = (CGameObject*)pNode->GetData();
		
	assert(pObject);		

	// InspectorUI �� ����
	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	pInspectorUI->SetTargetObject(pObject);
}

void SceneOutliner::AddGameObjectToTree(CGameObject* _pObject, TreeNode* _pDestNode)
{
	TreeNode* pNode = m_TreeUI->AddTreeNode(_pDestNode
											, string(_pObject->GetName().begin(), _pObject->GetName().end())
											, (DWORD_PTR)_pObject);

	const vector<CGameObject*>& vecChild = _pObject->GetChild();

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		AddGameObjectToTree(vecChild[i], pNode);
	}
}

void SceneOutliner::PressDelete(DWORD_PTR _dw)
{
	TreeNode* pNode = (TreeNode*)_dw;

	if (nullptr == pNode)
		return;

	CGameObject* pTargetObj = (CGameObject*)pNode->GetData();
	pTargetObj->Destroy();

	// InspectorUI �� ã�Ƽ� Object �� nullptr �� �����Ѵ�.
		
	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	pInspectorUI->SetTargetObject(nullptr);
}

void SceneOutliner::DragAndDropDelegate(DWORD_PTR _dwDrag, DWORD_PTR _dwDrop)
{
	CGameObject* pChildObject = (CGameObject*)_dwDrag;
	CGameObject* pDropTargetObject = (CGameObject*)_dwDrop;


	// ��� �������� ����� ��� �� ���
	if (nullptr != pDropTargetObject)
	{
		if (pChildObject == pDropTargetObject
			|| pDropTargetObject->IsAncestor(pChildObject))
		{
			return;
		}

		CSceneMgr::GetInst()->AddChild(pDropTargetObject, pChildObject);
	}

	// �ڽ� ������Ʈ�� �������� nullptr �� ���
	else
	{
		// �̹� �ֻ��� �θ� ������Ʈ�� �̺�Ʈ ó���� �ʿ����.
		if (nullptr == pChildObject->GetParent())
		{
			return;
		}

		CSceneMgr::GetInst()->DisconnectParent(pChildObject);		
	}
}

void SceneOutliner::ResDrop(DWORD_PTR _resPtr)
{
	if (ImGui::BeginDragDropTarget())
	{
		DWORD_PTR dwData = 0;
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Resource"))
		{
			memcpy(&dwData, payload->Data, sizeof(DWORD_PTR));
		}

		ImGui::EndDragDropTarget();
	}
}

void SceneOutliner::AddObjToScene(string str, int layer)
{
	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();

	CGameObject* pObj = new CGameObject;
	pObj->AddComponent(new CTransform);

	char* ch = (char*)str.c_str();
	string stmp = ch;
	wstring wstr = wstring(stmp.begin(), stmp.end());
	pObj->SetName(wstr);

	m_TreeUI->AddTreeNode(m_TreeUI->GetDummyNode(), str, (DWORD_PTR)pObj);

	pScene->AddObject(pObj, layer);
}

void SceneOutliner::AddPrefab()
{
	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	CGameObject* pObject = pInspectorUI->GetTargetObject();

	if (nullptr == pObject)
		return;

	Ptr<CPrefab> pPrefab = new CPrefab;
	CGameObject* pClone = pObject->Clone();
	pPrefab->SetProto(pClone);

	wstring PrefabKey = CPathMgr::GetInst()->GetContentPath();
	PrefabKey += L"prefab\\";
	PrefabKey += pObject->GetName();
	PrefabKey += L".pref";

	CSceneSaveLoad::SavePrefab(pPrefab.Get(), PrefabKey);

	PrefabKey = {};
	PrefabKey += L"prefab\\";
	PrefabKey += pObject->GetName();
	PrefabKey += L".pref";

	CResMgr::GetInst()->AddRes<CPrefab>(PrefabKey, pPrefab.Get(), false);
}

void SceneOutliner::AddDelete()
{
	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	CGameObject* pObject = pInspectorUI->GetTargetObject();

	if (nullptr == pObject)
		return;

	tEventInfo info;
	info.eType = EVENT_TYPE::DELETE_OBJ;
	info.lParam = (DWORD_PTR)pObject;
	CEventMgr::GetInst()->AddEvent(info);
}
