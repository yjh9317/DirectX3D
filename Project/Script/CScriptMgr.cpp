#include "pch.h"
#include "CScriptMgr.h"

#include "CAttackCollisionScript.h"
#include "CBellScript.h"
#include "CBisonDeath.h"
#include "CBisonHeadbut.h"
#include "CBisonHurt.h"
#include "CBisonIdle.h"
#include "CBisonRun.h"
#include "CBisonScript.h"
#include "CBisonSpawn.h"
#include "CBisonWalk.h"
#include "CBloodScreenScript.h"
#include "CBulletScript.h"
#include "CCameraMoveScript.h"
#include "CChestIdle.h"
#include "CChestOpen.h"
#include "CChestOpened.h"
#include "CChestScript.h"
#include "CCoffeeScript.h"
#include "CDirectionalLightScript.h"
#include "CGCScript.h"
#include "CGigantAttack.h"
#include "CGigantDeath.h"
#include "CGigantEnd.h"
#include "CGigantIdle.h"
#include "CGigantLoop.h"
#include "CGigantScript.h"
#include "CGigantSpawn.h"
#include "CHDRScript.h"
#include "CHPUIScript.h"
#include "CImpAttack.h"
#include "CImpDeath.h"
#include "CImpHurt.h"
#include "CImpIdle.h"
#include "CImpRun.h"
#include "CImpScript.h"
#include "CImpSpawn.h"
#include "CItemObjectScript.h"
#include "CJellyFishDeath.h"
#include "CJellyFishIdle.h"
#include "CJellyFishScript.h"
#include "CJellyFishSpawn.h"
#include "CLemurianDeath.h"
#include "CLemurianFireball.h"
#include "CLemurianHurt.h"
#include "CLemurianIdle.h"
#include "CLemurianPre.h"
#include "CLemurianRun.h"
#include "CLemurianScript.h"
#include "CLemurianSpawn.h"
#include "CMedikitScript.h"
#include "CMissileScript.h"
#include "CPlayerFire.h"
#include "CPlayerIdle.h"
#include "CPlayerJump.h"
#include "CPlayerMove.h"
#include "CPlayerRoll.h"
#include "CPlayerRun.h"
#include "CPlayerScript.h"
#include "CPlayerSkill.h"
#include "CScreenUIScript.h"
#include "CShrineScript.h"
#include "CSubAttack.h"
#include "CSubDeath.h"
#include "CSubIdle.h"
#include "CSubScript.h"
#include "CSubSpawn.h"
#include "CSubWalk.h"
#include "CSummon1Script.h"
#include "CSummon2Script.h"
#include "CSummon3Script.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CAttackCollisionScript");
	_vec.push_back(L"CBellScript");
	_vec.push_back(L"CBisonDeath");
	_vec.push_back(L"CBisonHeadbut");
	_vec.push_back(L"CBisonHurt");
	_vec.push_back(L"CBisonIdle");
	_vec.push_back(L"CBisonRun");
	_vec.push_back(L"CBisonScript");
	_vec.push_back(L"CBisonSpawn");
	_vec.push_back(L"CBisonWalk");
	_vec.push_back(L"CBloodScreenScript");
	_vec.push_back(L"CBulletScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CChestIdle");
	_vec.push_back(L"CChestOpen");
	_vec.push_back(L"CChestOpened");
	_vec.push_back(L"CChestScript");
	_vec.push_back(L"CCoffeeScript");
	_vec.push_back(L"CDirectionalLightScript");
	_vec.push_back(L"CGCScript");
	_vec.push_back(L"CGigantAttack");
	_vec.push_back(L"CGigantDeath");
	_vec.push_back(L"CGigantEnd");
	_vec.push_back(L"CGigantIdle");
	_vec.push_back(L"CGigantLoop");
	_vec.push_back(L"CGigantScript");
	_vec.push_back(L"CGigantSpawn");
	_vec.push_back(L"CHDRScript");
	_vec.push_back(L"CHPUIScript");
	_vec.push_back(L"CImpAttack");
	_vec.push_back(L"CImpDeath");
	_vec.push_back(L"CImpHurt");
	_vec.push_back(L"CImpIdle");
	_vec.push_back(L"CImpRun");
	_vec.push_back(L"CImpScript");
	_vec.push_back(L"CImpSpawn");
	_vec.push_back(L"CItemObjectScript");
	_vec.push_back(L"CJellyFishDeath");
	_vec.push_back(L"CJellyFishIdle");
	_vec.push_back(L"CJellyFishScript");
	_vec.push_back(L"CJellyFishSpawn");
	_vec.push_back(L"CLemurianDeath");
	_vec.push_back(L"CLemurianFireball");
	_vec.push_back(L"CLemurianHurt");
	_vec.push_back(L"CLemurianIdle");
	_vec.push_back(L"CLemurianPre");
	_vec.push_back(L"CLemurianRun");
	_vec.push_back(L"CLemurianScript");
	_vec.push_back(L"CLemurianSpawn");
	_vec.push_back(L"CMedikitScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CPlayerFire");
	_vec.push_back(L"CPlayerIdle");
	_vec.push_back(L"CPlayerJump");
	_vec.push_back(L"CPlayerMove");
	_vec.push_back(L"CPlayerRoll");
	_vec.push_back(L"CPlayerRun");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CPlayerSkill");
	_vec.push_back(L"CScreenUIScript");
	_vec.push_back(L"CShrineScript");
	_vec.push_back(L"CSubAttack");
	_vec.push_back(L"CSubDeath");
	_vec.push_back(L"CSubIdle");
	_vec.push_back(L"CSubScript");
	_vec.push_back(L"CSubSpawn");
	_vec.push_back(L"CSubWalk");
	_vec.push_back(L"CSummon1Script");
	_vec.push_back(L"CSummon2Script");
	_vec.push_back(L"CSummon3Script");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CAttackCollisionScript" == _strScriptName)
		return new CAttackCollisionScript;
	if (L"CBellScript" == _strScriptName)
		return new CBellScript;
	if (L"CBisonDeath" == _strScriptName)
		return new CBisonDeath;
	if (L"CBisonHeadbut" == _strScriptName)
		return new CBisonHeadbut;
	if (L"CBisonHurt" == _strScriptName)
		return new CBisonHurt;
	if (L"CBisonIdle" == _strScriptName)
		return new CBisonIdle;
	if (L"CBisonRun" == _strScriptName)
		return new CBisonRun;
	if (L"CBisonScript" == _strScriptName)
		return new CBisonScript;
	if (L"CBisonSpawn" == _strScriptName)
		return new CBisonSpawn;
	if (L"CBisonWalk" == _strScriptName)
		return new CBisonWalk;
	if (L"CBloodScreenScript" == _strScriptName)
		return new CBloodScreenScript;
	if (L"CBulletScript" == _strScriptName)
		return new CBulletScript;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CChestIdle" == _strScriptName)
		return new CChestIdle;
	if (L"CChestOpen" == _strScriptName)
		return new CChestOpen;
	if (L"CChestOpened" == _strScriptName)
		return new CChestOpened;
	if (L"CChestScript" == _strScriptName)
		return new CChestScript;
	if (L"CCoffeeScript" == _strScriptName)
		return new CCoffeeScript;
	if (L"CDirectionalLightScript" == _strScriptName)
		return new CDirectionalLightScript;
	if (L"CGCScript" == _strScriptName)
		return new CGCScript;
	if (L"CGigantAttack" == _strScriptName)
		return new CGigantAttack;
	if (L"CGigantDeath" == _strScriptName)
		return new CGigantDeath;
	if (L"CGigantEnd" == _strScriptName)
		return new CGigantEnd;
	if (L"CGigantIdle" == _strScriptName)
		return new CGigantIdle;
	if (L"CGigantLoop" == _strScriptName)
		return new CGigantLoop;
	if (L"CGigantScript" == _strScriptName)
		return new CGigantScript;
	if (L"CGigantSpawn" == _strScriptName)
		return new CGigantSpawn;
	if (L"CHDRScript" == _strScriptName)
		return new CHDRScript;
	if (L"CHPUIScript" == _strScriptName)
		return new CHPUIScript;
	if (L"CImpAttack" == _strScriptName)
		return new CImpAttack;
	if (L"CImpDeath" == _strScriptName)
		return new CImpDeath;
	if (L"CImpHurt" == _strScriptName)
		return new CImpHurt;
	if (L"CImpIdle" == _strScriptName)
		return new CImpIdle;
	if (L"CImpRun" == _strScriptName)
		return new CImpRun;
	if (L"CImpScript" == _strScriptName)
		return new CImpScript;
	if (L"CImpSpawn" == _strScriptName)
		return new CImpSpawn;
	if (L"CItemObjectScript" == _strScriptName)
		return new CItemObjectScript;
	if (L"CJellyFishDeath" == _strScriptName)
		return new CJellyFishDeath;
	if (L"CJellyFishIdle" == _strScriptName)
		return new CJellyFishIdle;
	if (L"CJellyFishScript" == _strScriptName)
		return new CJellyFishScript;
	if (L"CJellyFishSpawn" == _strScriptName)
		return new CJellyFishSpawn;
	if (L"CLemurianDeath" == _strScriptName)
		return new CLemurianDeath;
	if (L"CLemurianFireball" == _strScriptName)
		return new CLemurianFireball;
	if (L"CLemurianHurt" == _strScriptName)
		return new CLemurianHurt;
	if (L"CLemurianIdle" == _strScriptName)
		return new CLemurianIdle;
	if (L"CLemurianPre" == _strScriptName)
		return new CLemurianPre;
	if (L"CLemurianRun" == _strScriptName)
		return new CLemurianRun;
	if (L"CLemurianScript" == _strScriptName)
		return new CLemurianScript;
	if (L"CLemurianSpawn" == _strScriptName)
		return new CLemurianSpawn;
	if (L"CMedikitScript" == _strScriptName)
		return new CMedikitScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CPlayerFire" == _strScriptName)
		return new CPlayerFire;
	if (L"CPlayerIdle" == _strScriptName)
		return new CPlayerIdle;
	if (L"CPlayerJump" == _strScriptName)
		return new CPlayerJump;
	if (L"CPlayerMove" == _strScriptName)
		return new CPlayerMove;
	if (L"CPlayerRoll" == _strScriptName)
		return new CPlayerRoll;
	if (L"CPlayerRun" == _strScriptName)
		return new CPlayerRun;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CPlayerSkill" == _strScriptName)
		return new CPlayerSkill;
	if (L"CScreenUIScript" == _strScriptName)
		return new CScreenUIScript;
	if (L"CShrineScript" == _strScriptName)
		return new CShrineScript;
	if (L"CSubAttack" == _strScriptName)
		return new CSubAttack;
	if (L"CSubDeath" == _strScriptName)
		return new CSubDeath;
	if (L"CSubIdle" == _strScriptName)
		return new CSubIdle;
	if (L"CSubScript" == _strScriptName)
		return new CSubScript;
	if (L"CSubSpawn" == _strScriptName)
		return new CSubSpawn;
	if (L"CSubWalk" == _strScriptName)
		return new CSubWalk;
	if (L"CSummon1Script" == _strScriptName)
		return new CSummon1Script;
	if (L"CSummon2Script" == _strScriptName)
		return new CSummon2Script;
	if (L"CSummon3Script" == _strScriptName)
		return new CSummon3Script;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::ATTACKCOLLISIONSCRIPT:
		return new CAttackCollisionScript;
		break;
	case (UINT)SCRIPT_TYPE::BELLSCRIPT:
		return new CBellScript;
		break;
	case (UINT)SCRIPT_TYPE::BISONDEATH:
		return new CBisonDeath;
		break;
	case (UINT)SCRIPT_TYPE::BISONHEADBUT:
		return new CBisonHeadbut;
		break;
	case (UINT)SCRIPT_TYPE::BISONHURT:
		return new CBisonHurt;
		break;
	case (UINT)SCRIPT_TYPE::BISONIDLE:
		return new CBisonIdle;
		break;
	case (UINT)SCRIPT_TYPE::BISONRUN:
		return new CBisonRun;
		break;
	case (UINT)SCRIPT_TYPE::BISONSCRIPT:
		return new CBisonScript;
		break;
	case (UINT)SCRIPT_TYPE::BISONSPAWN:
		return new CBisonSpawn;
		break;
	case (UINT)SCRIPT_TYPE::BISONWALK:
		return new CBisonWalk;
		break;
	case (UINT)SCRIPT_TYPE::BLOODSCREENSCRIPT:
		return new CBloodScreenScript;
		break;
	case (UINT)SCRIPT_TYPE::BULLETSCRIPT:
		return new CBulletScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::CHESTIDLE:
		return new CChestIdle;
		break;
	case (UINT)SCRIPT_TYPE::CHESTOPEN:
		return new CChestOpen;
		break;
	case (UINT)SCRIPT_TYPE::CHESTOPENED:
		return new CChestOpened;
		break;
	case (UINT)SCRIPT_TYPE::CHESTSCRIPT:
		return new CChestScript;
		break;
	case (UINT)SCRIPT_TYPE::COFFEESCRIPT:
		return new CCoffeeScript;
		break;
	case (UINT)SCRIPT_TYPE::DIRECTIONALLIGHTSCRIPT:
		return new CDirectionalLightScript;
		break;
	case (UINT)SCRIPT_TYPE::GCSCRIPT:
		return new CGCScript;
		break;
	case (UINT)SCRIPT_TYPE::GIGANTATTACK:
		return new CGigantAttack;
		break;
	case (UINT)SCRIPT_TYPE::GIGANTDEATH:
		return new CGigantDeath;
		break;
	case (UINT)SCRIPT_TYPE::GIGANTEND:
		return new CGigantEnd;
		break;
	case (UINT)SCRIPT_TYPE::GIGANTIDLE:
		return new CGigantIdle;
		break;
	case (UINT)SCRIPT_TYPE::GIGANTLOOP:
		return new CGigantLoop;
		break;
	case (UINT)SCRIPT_TYPE::GIGANTSCRIPT:
		return new CGigantScript;
		break;
	case (UINT)SCRIPT_TYPE::GIGANTSPAWN:
		return new CGigantSpawn;
		break;
	case (UINT)SCRIPT_TYPE::HDRSCRIPT:
		return new CHDRScript;
		break;
	case (UINT)SCRIPT_TYPE::HPUISCRIPT:
		return new CHPUIScript;
		break;
	case (UINT)SCRIPT_TYPE::IMPATTACK:
		return new CImpAttack;
		break;
	case (UINT)SCRIPT_TYPE::IMPDEATH:
		return new CImpDeath;
		break;
	case (UINT)SCRIPT_TYPE::IMPHURT:
		return new CImpHurt;
		break;
	case (UINT)SCRIPT_TYPE::IMPIDLE:
		return new CImpIdle;
		break;
	case (UINT)SCRIPT_TYPE::IMPRUN:
		return new CImpRun;
		break;
	case (UINT)SCRIPT_TYPE::IMPSCRIPT:
		return new CImpScript;
		break;
	case (UINT)SCRIPT_TYPE::IMPSPAWN:
		return new CImpSpawn;
		break;
	case (UINT)SCRIPT_TYPE::ITEMOBJECTSCRIPT:
		return new CItemObjectScript;
		break;
	case (UINT)SCRIPT_TYPE::JELLYFISHDEATH:
		return new CJellyFishDeath;
		break;
	case (UINT)SCRIPT_TYPE::JELLYFISHIDLE:
		return new CJellyFishIdle;
		break;
	case (UINT)SCRIPT_TYPE::JELLYFISHSCRIPT:
		return new CJellyFishScript;
		break;
	case (UINT)SCRIPT_TYPE::JELLYFISHSPAWN:
		return new CJellyFishSpawn;
		break;
	case (UINT)SCRIPT_TYPE::LEMURIANDEATH:
		return new CLemurianDeath;
		break;
	case (UINT)SCRIPT_TYPE::LEMURIANFIREBALL:
		return new CLemurianFireball;
		break;
	case (UINT)SCRIPT_TYPE::LEMURIANHURT:
		return new CLemurianHurt;
		break;
	case (UINT)SCRIPT_TYPE::LEMURIANIDLE:
		return new CLemurianIdle;
		break;
	case (UINT)SCRIPT_TYPE::LEMURIANPRE:
		return new CLemurianPre;
		break;
	case (UINT)SCRIPT_TYPE::LEMURIANRUN:
		return new CLemurianRun;
		break;
	case (UINT)SCRIPT_TYPE::LEMURIANSCRIPT:
		return new CLemurianScript;
		break;
	case (UINT)SCRIPT_TYPE::LEMURIANSPAWN:
		return new CLemurianSpawn;
		break;
	case (UINT)SCRIPT_TYPE::MEDIKITSCRIPT:
		return new CMedikitScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERFIRE:
		return new CPlayerFire;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERIDLE:
		return new CPlayerIdle;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERJUMP:
		return new CPlayerJump;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERMOVE:
		return new CPlayerMove;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERROLL:
		return new CPlayerRoll;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERRUN:
		return new CPlayerRun;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSKILL:
		return new CPlayerSkill;
		break;
	case (UINT)SCRIPT_TYPE::SCREENUISCRIPT:
		return new CScreenUIScript;
		break;
	case (UINT)SCRIPT_TYPE::SHRINESCRIPT:
		return new CShrineScript;
		break;
	case (UINT)SCRIPT_TYPE::SUBATTACK:
		return new CSubAttack;
		break;
	case (UINT)SCRIPT_TYPE::SUBDEATH:
		return new CSubDeath;
		break;
	case (UINT)SCRIPT_TYPE::SUBIDLE:
		return new CSubIdle;
		break;
	case (UINT)SCRIPT_TYPE::SUBSCRIPT:
		return new CSubScript;
		break;
	case (UINT)SCRIPT_TYPE::SUBSPAWN:
		return new CSubSpawn;
		break;
	case (UINT)SCRIPT_TYPE::SUBWALK:
		return new CSubWalk;
		break;
	case (UINT)SCRIPT_TYPE::SUMMON1SCRIPT:
		return new CSummon1Script;
		break;
	case (UINT)SCRIPT_TYPE::SUMMON2SCRIPT:
		return new CSummon2Script;
		break;
	case (UINT)SCRIPT_TYPE::SUMMON3SCRIPT:
		return new CSummon3Script;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::ATTACKCOLLISIONSCRIPT:
		return L"CAttackCollisionScript";
		break;

	case SCRIPT_TYPE::BELLSCRIPT:
		return L"CBellScript";
		break;

	case SCRIPT_TYPE::BISONDEATH:
		return L"CBisonDeath";
		break;

	case SCRIPT_TYPE::BISONHEADBUT:
		return L"CBisonHeadbut";
		break;

	case SCRIPT_TYPE::BISONHURT:
		return L"CBisonHurt";
		break;

	case SCRIPT_TYPE::BISONIDLE:
		return L"CBisonIdle";
		break;

	case SCRIPT_TYPE::BISONRUN:
		return L"CBisonRun";
		break;

	case SCRIPT_TYPE::BISONSCRIPT:
		return L"CBisonScript";
		break;

	case SCRIPT_TYPE::BISONSPAWN:
		return L"CBisonSpawn";
		break;

	case SCRIPT_TYPE::BISONWALK:
		return L"CBisonWalk";
		break;

	case SCRIPT_TYPE::BLOODSCREENSCRIPT:
		return L"CBloodScreenScript";
		break;

	case SCRIPT_TYPE::BULLETSCRIPT:
		return L"CBulletScript";
		break;

	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::CHESTIDLE:
		return L"CChestIdle";
		break;

	case SCRIPT_TYPE::CHESTOPEN:
		return L"CChestOpen";
		break;

	case SCRIPT_TYPE::CHESTOPENED:
		return L"CChestOpened";
		break;

	case SCRIPT_TYPE::CHESTSCRIPT:
		return L"CChestScript";
		break;

	case SCRIPT_TYPE::COFFEESCRIPT:
		return L"CCoffeeScript";
		break;

	case SCRIPT_TYPE::DIRECTIONALLIGHTSCRIPT:
		return L"CDirectionalLightScript";
		break;

	case SCRIPT_TYPE::GCSCRIPT:
		return L"CGCScript";
		break;

	case SCRIPT_TYPE::GIGANTATTACK:
		return L"CGigantAttack";
		break;

	case SCRIPT_TYPE::GIGANTDEATH:
		return L"CGigantDeath";
		break;

	case SCRIPT_TYPE::GIGANTEND:
		return L"CGigantEnd";
		break;

	case SCRIPT_TYPE::GIGANTIDLE:
		return L"CGigantIdle";
		break;

	case SCRIPT_TYPE::GIGANTLOOP:
		return L"CGigantLoop";
		break;

	case SCRIPT_TYPE::GIGANTSCRIPT:
		return L"CGigantScript";
		break;

	case SCRIPT_TYPE::GIGANTSPAWN:
		return L"CGigantSpawn";
		break;

	case SCRIPT_TYPE::HDRSCRIPT:
		return L"CHDRScript";
		break;

	case SCRIPT_TYPE::HPUISCRIPT:
		return L"CHPUIScript";
		break;

	case SCRIPT_TYPE::IMPATTACK:
		return L"CImpAttack";
		break;

	case SCRIPT_TYPE::IMPDEATH:
		return L"CImpDeath";
		break;

	case SCRIPT_TYPE::IMPHURT:
		return L"CImpHurt";
		break;

	case SCRIPT_TYPE::IMPIDLE:
		return L"CImpIdle";
		break;

	case SCRIPT_TYPE::IMPRUN:
		return L"CImpRun";
		break;

	case SCRIPT_TYPE::IMPSCRIPT:
		return L"CImpScript";
		break;

	case SCRIPT_TYPE::IMPSPAWN:
		return L"CImpSpawn";
		break;

	case SCRIPT_TYPE::ITEMOBJECTSCRIPT:
		return L"CItemObjectScript";
		break;

	case SCRIPT_TYPE::JELLYFISHDEATH:
		return L"CJellyFishDeath";
		break;

	case SCRIPT_TYPE::JELLYFISHIDLE:
		return L"CJellyFishIdle";
		break;

	case SCRIPT_TYPE::JELLYFISHSCRIPT:
		return L"CJellyFishScript";
		break;

	case SCRIPT_TYPE::JELLYFISHSPAWN:
		return L"CJellyFishSpawn";
		break;

	case SCRIPT_TYPE::LEMURIANDEATH:
		return L"CLemurianDeath";
		break;

	case SCRIPT_TYPE::LEMURIANFIREBALL:
		return L"CLemurianFireball";
		break;

	case SCRIPT_TYPE::LEMURIANHURT:
		return L"CLemurianHurt";
		break;

	case SCRIPT_TYPE::LEMURIANIDLE:
		return L"CLemurianIdle";
		break;

	case SCRIPT_TYPE::LEMURIANPRE:
		return L"CLemurianPre";
		break;

	case SCRIPT_TYPE::LEMURIANRUN:
		return L"CLemurianRun";
		break;

	case SCRIPT_TYPE::LEMURIANSCRIPT:
		return L"CLemurianScript";
		break;

	case SCRIPT_TYPE::LEMURIANSPAWN:
		return L"CLemurianSpawn";
		break;

	case SCRIPT_TYPE::MEDIKITSCRIPT:
		return L"CMedikitScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::PLAYERFIRE:
		return L"CPlayerFire";
		break;

	case SCRIPT_TYPE::PLAYERIDLE:
		return L"CPlayerIdle";
		break;

	case SCRIPT_TYPE::PLAYERJUMP:
		return L"CPlayerJump";
		break;

	case SCRIPT_TYPE::PLAYERMOVE:
		return L"CPlayerMove";
		break;

	case SCRIPT_TYPE::PLAYERROLL:
		return L"CPlayerRoll";
		break;

	case SCRIPT_TYPE::PLAYERRUN:
		return L"CPlayerRun";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::PLAYERSKILL:
		return L"CPlayerSkill";
		break;

	case SCRIPT_TYPE::SCREENUISCRIPT:
		return L"CScreenUIScript";
		break;

	case SCRIPT_TYPE::SHRINESCRIPT:
		return L"CShrineScript";
		break;

	case SCRIPT_TYPE::SUBATTACK:
		return L"CSubAttack";
		break;

	case SCRIPT_TYPE::SUBDEATH:
		return L"CSubDeath";
		break;

	case SCRIPT_TYPE::SUBIDLE:
		return L"CSubIdle";
		break;

	case SCRIPT_TYPE::SUBSCRIPT:
		return L"CSubScript";
		break;

	case SCRIPT_TYPE::SUBSPAWN:
		return L"CSubSpawn";
		break;

	case SCRIPT_TYPE::SUBWALK:
		return L"CSubWalk";
		break;

	case SCRIPT_TYPE::SUMMON1SCRIPT:
		return L"CSummon1Script";
		break;

	case SCRIPT_TYPE::SUMMON2SCRIPT:
		return L"CSummon2Script";
		break;

	case SCRIPT_TYPE::SUMMON3SCRIPT:
		return L"CSummon3Script";
		break;

	}
	return nullptr;
}