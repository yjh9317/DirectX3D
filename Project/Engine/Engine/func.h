#pragma once


template<typename T>
void Safe_Del_Vec(vector<T*>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		SAFE_DELETE(_vec[i]);
	}
	_vec.clear();
}

template<typename T>
void Safe_Del_list(list<T*>& _list)
{
	typename list<T*>::iterator iter = _list.begin();

	for (; iter != _list.end(); ++iter)
	{
		SAFE_DELETE(*iter);
	}
	_list.clear();
}

template<typename T1, typename T2>
void Safe_Del_Map(map<T1, T2>& _map)
{
	typename map<T1, T2>::iterator iter = _map.begin();
	for (; iter != _map.end(); ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	_map.clear();
}

template<typename T, UINT _iSize>
void Safe_Del_Arr(T(&_Arr)[_iSize])
{
	for (UINT i = 0; i < _iSize; ++i)
	{
		SAFE_DELETE(_Arr[i]);
	}
}


void SaveStringToFile(const string& _str, FILE* _pFile);
void LoadStringFromFile(string& _str, FILE* _pFile);

void SaveWStringToFile(const wstring& _str, FILE* _pFile);
void LoadWStringFromFile(wstring& _str, FILE* _pFile);

Matrix GetMatrixFromFbxMatrix(FbxAMatrix& _mat);

const wchar_t* ToWString(COMPONENT_TYPE _type);
const char* ToString(COMPONENT_TYPE _type);
const wchar_t* ToWString(RES_TYPE _type);
const char* ToString(RES_TYPE _type);







#include "Ptr.h"
#include "CResMgr.h"


template<typename RES_TYPE>
wstring FindResName(const wstring& _strBaseName)
{
	wchar_t szName[256] = {};
	wchar_t szNum[10] = {};

	wcscpy_s(szName, 256, _strBaseName.c_str());
	wstring strFindName;

	for (UINT i = 1; i < 100; ++i)
	{
		wsprintf(szNum, L"_%02d", i);
		strFindName = wstring(szName) + szNum;
		if (nullptr == CResMgr::GetInst()->FindRes<RES_TYPE>(strFindName))
		{
			return strFindName;
		}
	}

	return wstring();
}






template<typename RES>
void SaveResPtr(Ptr<RES> _ptr, FILE* _pFile)
{
	bool bNullPtr = nullptr == _ptr ? true : false;
	fwrite(&bNullPtr, sizeof(bool), 1, _pFile);

	if (!bNullPtr)
	{
		SaveWStringToFile(_ptr->GetKey(), _pFile);
		SaveWStringToFile(_ptr->GetRelativePath(), _pFile);
	}
}

template<typename RES>
void LoadResPtr(Ptr<RES>& _ptr, FILE* _pFile)
{
	bool bNull = false;
	fread(&bNull, sizeof(bool), 1, _pFile);

	if (!bNull)
	{
		wstring strKey, strRelativePath;

		LoadWStringFromFile(strKey, _pFile);
		LoadWStringFromFile(strRelativePath, _pFile);

		_ptr = CResMgr::GetInst()->Load<RES>(strKey, strRelativePath);
	}	
}

Vec3 DecomposeRotMat(const Matrix& _matRot);
Vec3 LookAtVector(const Vec3& EyePosition, const Vec3& FocusPosition, const Vec3& UpDirection);

void InitWELLRNG1024a(unsigned int* init);
double WELLRNG1024a(void);


void VecToJson(Json::Value* node, const string& str, Vec4 v);
void VecToJson(Json::Value* node, const string& str, Vec3 v);
void VecToJson(Json::Value* node, const string& str, Vec2 v);

void LoadVecFromJson(Json::Value& node, Vec2& v);
void LoadVecFromJson(Json::Value& node, Vec3& v);
void LoadVecFromJson(Json::Value& node, Vec4& v);


void SaveWStringToJson(const wstring& _str, Json::Value* node);
void SaveWStringToJson(const string& _str, const wstring& _wstr, Json::Value* node);
wstring LoadWStringFromJson(const string _str, Json::Value* node);

template<typename RES>
void SaveJsonRes(Ptr<RES> _ptr,const string& _str ,Json::Value* node)
{
	Json::Value NewNode;

	bool bNullPtr = nullptr == _ptr ? true : false;
	NewNode["IsNull"] = bNullPtr;

	if (!bNullPtr)
	{
		string StrKey = "Key";
		string StrPath = "Path";

		wstring WstrKey = _ptr->GetKey();
		wstring WstrPath = _ptr->GetRelativePath();

		SaveWStringToJson(StrKey, WstrKey, &NewNode);
		SaveWStringToJson(StrPath, WstrPath, &NewNode);
	}
	(*node)[_str] = NewNode;
}


template<typename RES>
void LoadJsonRes(Ptr<RES>& _ptr, Json::Value* node)
{
	bool IsNull = (*node)["IsNull"].asBool();

	if (!IsNull)
	{
		wstring strKey = LoadWStringFromJson("Key", node);
		wstring strRelativePath = LoadWStringFromJson("Path", node);

		_ptr = CResMgr::GetInst()->Load<RES>(strKey, strRelativePath);
	}
}


