#include "pch.h"
#include "CGigantDeath.h"


CGigantDeath::CGigantDeath()
	:CScript((int)SCRIPT_TYPE::GIGANTDEATH)
{
}

CGigantDeath::~CGigantDeath()
{
}

void CGigantDeath::enter()
{
	Animator3D()->Play(L"Death", false);
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\PaperBurnMtrl.mtrl");
	Ptr<CTexture> pColorTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\texGrandparentDiffuse.tga");
	Ptr<CTexture> pNormalTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\FBXTexture\\texGrandparentDetailBNormal.tga");
	pMtrl->SetTexParam(TEX_PARAM::TEX_0, pColorTex);
	pMtrl->SetTexParam(TEX_PARAM::TEX_1, pNormalTex);
	Vec4 vBurnColor = Vec4(255.f,100.f,10.f,1.f);
	MeshRender()->GetSharedMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, &vBurnColor);
	MeshRender()->SetSharedMaterial(pMtrl, 0);
	
}

void CGigantDeath::StateUpdate()
{
	if (Animator3D()->GetCurAnim()->IsFinish())
	{
		static float BurnTime =0.f;
		BurnTime += DT/3.f;
		MeshRender()->GetSharedMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, &BurnTime);

		if (BurnTime > 1.f)
		{
			GetOwner()->Destroy();
		}
	}
}

void CGigantDeath::exit()
{
}

void CGigantDeath::start()
{
	CState* pState = FSM()->FindState(L"Death");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Death", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CGigantDeath::update()
{

}

