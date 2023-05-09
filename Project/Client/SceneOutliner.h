#pragma once
#include "UI.h"

#include "TreeUI.h"

class CGameObject;

class SceneOutliner :
    public UI
{
private:
    TreeUI*     m_TreeUI;

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    void Reset();
    void ObjectClicked(DWORD_PTR _dw);

    void AddGameObjectToTree(CGameObject* _pObject, TreeNode* _pDestNode);
    void PressDelete(DWORD_PTR _dw);
    void DragAndDropDelegate(DWORD_PTR _dwDrag, DWORD_PTR _dwDrop);
    void ResDrop(DWORD_PTR _resPtr);

    void AddObjToScene(string str, int layer);

    void AddPrefab();
    void AddDelete();
public:
    SceneOutliner();
    ~SceneOutliner();
};

