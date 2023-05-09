#include "pch.h"
#include "CTestScene.h"

#include <Engine/CResMgr.h>
#include <Engine/CCollisionMgr.h>

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CTexture.h>

#include <Engine/CTransform.h>
#include <Engine/CMeshRender.h>
#include <Engine/CCollider2D.h>
#include <Engine/CParticleSystem.h>
#include <Engine/CCamera.h>
#include <Engine/CLight2D.h>
#include <Engine/CLight3D.h>
#include <Engine/CSkyBox.h>
#include <Engine\CDecal.h>
#include <Engine\CLandScape.h>
#include <Engine/CSceneFile.h>
#include <Engine/CSound.h>
#include <Engine/CFSM.h>
#include <Engine/CAnimator3D.h>
#include <Engine\CMeshData.h>


#include <Script/CPlayerScript.h>
#include <Script/CCameraMoveScript.h>
#include <Script/CMissileScript.h>
#include <Script/CHDRScript.h>
#include <Script/CBloodScreenScript.h>

#include <Script\CSceneSaveLoad.h>




void CTestScene::CreateTestScene()
{
	CScene* pCurScene = new CScene;
	
	CSceneMgr::GetInst()->ChangeScene(pCurScene);		
	
	pCurScene->SetLayerName(0, L"Default");
	pCurScene->SetLayerName(1, L"LandScape");
	pCurScene->SetLayerName(2, L"Player");
	pCurScene->SetLayerName(3, L"P_Collider");
	pCurScene->SetLayerName(4, L"Monster");
	pCurScene->SetLayerName(5, L"M_Collider");
	pCurScene->SetLayerName(6, L"Object");
	pCurScene->SetLayerName(7, L"MonAtkCollision");

	// Texture 한장 로딩해보기
	CResMgr::GetInst()->Load<CTexture>(L"texture\\MagicCircle.png", L"texture\\MagicCircle.png");

	Ptr<CTexture> pTileTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01.tga", L"texture\\tile\\TILE_01.tga");
	Ptr<CTexture> pTileNTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_01_N.tga", L"texture\\tile\\TILE_01_N.tga");

	Ptr<CTexture> pSkyTex_01 = CResMgr::GetInst()->Load<CTexture>(L"texture\\skybox\\Sky01.png", L"texture\\skybox\\Sky01.png");
	Ptr<CTexture> pSkyTex_02 = CResMgr::GetInst()->Load<CTexture>(L"texture\\skybox\\Sky02.jpg", L"texture\\skybox\\Sky02.jpg");
	Ptr<CTexture> pSkyTex_03 = CResMgr::GetInst()->Load<CTexture>(L"texture\\skybox\\SkyDawn.dds", L"texture\\skybox\\SkyDawn.dds");
	Ptr<CTexture> pSkyTex_04 = CResMgr::GetInst()->Load<CTexture>(L"texture\\skybox\\SkyWater.dds", L"texture\\skybox\\SkyWater.dds");

	Ptr<CTexture> _pTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\FBXTexture\\texCommandoPaletteDiffuse.tga", L"texture\\FBXTexture\\texCommandoPaletteDiffuse.tga");
	CResMgr::GetInst()->FindRes<CMaterial>(L"material\\SocketMtrl.mtrl")->SetTexParam(TEX_PARAM::TEX_0,_pTex);
	CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Socket2Mtrl.mtrl")->SetTexParam(TEX_PARAM::TEX_0, _pTex);

	Ptr<CTexture> UITex = CResMgr::GetInst()->Load<CTexture>(L"texture\\ui\\texUIBack.tga", L"texture\\ui\\texUIBack.tga");
	CResMgr::GetInst()->FindRes<CMaterial>(L"material\\BackgroundHPUI.mtrl")->SetTexParam(TEX_PARAM::TEX_0, UITex);
	// @@@@@@@@@ 위에서 Material Texture세팅

	
	// @@@@@@@@@@@@@@@@@@@@
	

	// Camera Object 추가
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraMoveScript);

	pCamObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pCamObj->Camera()->SetFar(100000);
	pCamObj->Camera()->SetCameraAsMain();
	pCamObj->Camera()->CheckLayerMaskAll();

	pCurScene->AddObject(pCamObj, L"Default");

	// Directional Light
	CGameObject* pLight3D = new CGameObject;
	pLight3D->SetName(L"Directional Light");

	pLight3D->AddComponent(new CTransform);
	pLight3D->AddComponent(new CLight3D);

	pLight3D->Transform()->SetRelativePos(Vec3(-1000.f, 1000.f, 0.f));

	
	pLight3D->Light3D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLight3D->Light3D()->SetLightDir(Vec3(90.f, 0.f, 0.f));
	pLight3D->Light3D()->SetRange(20000.f);

	pLight3D->Light3D()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
	pLight3D->Light3D()->SetSpecular(Vec3(0.15f, 0.15f, 0.15f));
	pLight3D->Light3D()->SetAmbient(Vec3(0.1f, 0.1f, 0.1f));
	

	pCurScene->AddObject(pLight3D, L"Default");
	

	// Point Light
	/*CGameObject* pLight3D = new CGameObject; 
	pLight3D->SetName(L"Point Light");

	pLight3D->AddComponent(new CTransform);
	pLight3D->AddComponent(new CLight3D);

	pLight3D->Transform()->SetRelativePos(Vec3(0.f, 500.f, 500.f));

	pLight3D->Light3D()->SetLightType(LIGHT_TYPE::POINT);	
	pLight3D->Light3D()->SetRange(1000.f);

	pLight3D->Light3D()->SetDiffuse(Vec3(1.f, 1.f, 1.f));
	pLight3D->Light3D()->SetSpecular(Vec3(0.1f, 0.1f, 0.1f));
	pLight3D->Light3D()->SetAmbient(Vec3(0.f, 0.f, 0.f));

	pCurScene->AddObject(pLight3D, L"Default");*/


	// SkyBox 추가
	CGameObject* pSkyBox = new CGameObject;

	pSkyBox->SetName(L"SkyBox");
	pSkyBox->AddComponent(new CTransform);
	pSkyBox->AddComponent(new CSkyBox);

	pSkyBox->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pSkyBox->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 100.f));

	pSkyBox->SkyBox()->SetSkyboxType(SKYBOX_TYPE::CUBE);
	pSkyBox->SkyBox()->GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_CUBE_0, pSkyTex_03);
	pSkyBox->SkyBox()->SetFrustumCulling(false);

	pCurScene->AddObject(pSkyBox, L"Default");

	


	// ============
	// FBX Loading
	// ============	
	{
		//Ptr<CMeshData> pMeshData = nullptr;
		//CGameObject* pObj = nullptr;

		//pMeshData = CResMgr::GetInst()->LoadFBX(L"fbx\\combatshrine.fbx");
		//pMeshData->Save(wstring(CPathMgr::GetInst()->GetContentPath()) + pMeshData->GetRelativePath());		

		////pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"meshdata\\Bison0.mdat", L"meshdata\\Bison0.mdat");

		//pObj = pMeshData->Instantiate();
		//pObj->SetName(L"Combatshrine3");
		//pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
		//pObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		//pObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 50.f));

		//pObj->Animator3D()->Load(L"Bison");
		//pObj->MeshRender()->SetDynamicShadow(true);

		//pCurScene->AddObject(pObj, 2);

	}




	// LandScape
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"LandScape");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CLandScape);

	pObject->Transform()->SetRelativePos(0.f, 0.f, 0.f);
	pObject->Transform()->SetRelativeScale(500.f, 500.f, 500.f);
	pObject->Transform()->SetRelativeRotation(0.f, 0.f, 0.f);

	pObject->LandScape()->SetDynamicShadow(false);
	pObject->LandScape()->SetFrustumCulling(false);	
	pObject->LandScape()->SetFaceCount(32, 32);
	pObject->LandScape()->Create();

	pCurScene->AddObject(pObject, L"LandScape");




	CGameObject* pPostProcess = new CGameObject;
	pPostProcess->SetName(L"DOF");

	pPostProcess->AddComponent(new CTransform);
	pPostProcess->AddComponent(new CMeshRender);

	pPostProcess->Transform()->SetRelativePos(0.f, 0.f, 100000.f);
	pPostProcess->Transform()->SetRelativeScale(0, 0, 1.f);

	pPostProcess->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pPostProcess->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\PostProcessMtrl.mtrl"), 0);
	

	pCurScene->AddObject(pPostProcess, L"Default");
	pPostProcess->Deactivate();

	CCameraMoveScript* pCamMoveScript = pCamObj->GetScript<CCameraMoveScript>();
	((CCameraMoveScript*)pCamObj->GetScript(0))->SetFilter(pPostProcess);

	//pPostProcess = new CGameObject;
	//pPostProcess->SetName(L"HDR");

	//pPostProcess->AddComponent(new CTransform);
	//pPostProcess->AddComponent(new CMeshRender);
	//pPostProcess->AddComponent(new CHDRScript);

	//pPostProcess->Transform()->SetRelativePos(0.f, 0.f, 100.f);
	//pPostProcess->Transform()->SetRelativeScale(100, 100, 100.f);

	//pPostProcess->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pPostProcess->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\HDRMtrl.mtrl"), 0);
	//
	//Ptr<CTexture> pp = CResMgr::GetInst()->FindRes<CTexture>(L"PostProcessTex");
	//pPostProcess->MeshRender()->GetSharedMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, pp);
	//
	//pCurScene->AddObject(pPostProcess, L"Default");
	//pPostProcess->Deactivate();



	pPostProcess = new CGameObject;
	pPostProcess->SetName(L"BloodScreen");

	pPostProcess->AddComponent(new CTransform);
	pPostProcess->AddComponent(new CMeshRender);
	pPostProcess->AddComponent(new CBloodScreenScript);

	pPostProcess->Transform()->SetRelativePos(0.f, 0.f, 10000.f);
	pPostProcess->Transform()->SetRelativeScale(0, 0, 1.f);

	pPostProcess->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pPostProcess->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\BloodScreenMtrl.mtrl"), 0);


	pCurScene->AddObject(pPostProcess, L"Default");
	pPostProcess->Deactivate();
	
	

	// 충돌 레이어 설정
	pCurScene->SetResKey(L"scene\\TestScene.scene");
	wstring strSceneFilePath = CPathMgr::GetInst()->GetContentPath();	
	CSceneSaveLoad::SaveScene(pCurScene, strSceneFilePath + L"scene\\TestScene.scene");

#ifdef _DEBUG
	CSceneSaveLoad::SaveSceneToJson(pCurScene, strSceneFilePath + L"scene\\TestScene.json");
#endif

	CCollisionMgr::GetInst()->CollisionCheck(1, 2);
	CCollisionMgr::GetInst()->CollisionCheck(3, 0);
	CCollisionMgr::GetInst()->CollisionCheck(1, 4);

	CCollisionMgr::GetInst()->CollisionCheck(2, 5);
	CCollisionMgr::GetInst()->CollisionCheck(2, 4);
	CCollisionMgr::GetInst()->CollisionCheck(3, 4);
	CCollisionMgr::GetInst()->CollisionCheck(6, 2);
	CCollisionMgr::GetInst()->CollisionCheck(7, 2);



	

	//pCurScene->start();	
	pCurScene->SetSceneState(SCENE_STATE::STOP);

}

void CTestScene::LoadScene()
{
	wstring strSceneFilePath = CPathMgr::GetInst()->GetContentPath();
	CScene* pLoadScene = CSceneSaveLoad::LoadScene(strSceneFilePath + L"scene\\map");
	CSceneMgr::GetInst()->ChangeScene(pLoadScene);
}
