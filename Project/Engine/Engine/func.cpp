#include "pch.h"

void SaveStringToFile(const string& _str, FILE* _pFile)
{
    BYTE len = (BYTE)_str.length();
    fwrite(&len, sizeof(BYTE), 1, _pFile);
    fwrite(_str.c_str(), sizeof(char), len, _pFile);
}

void LoadStringFromFile(string& _str, FILE* _pFile)
{
    char szBuffer[256] = {};
    BYTE len = 0;
    fread(&len, sizeof(BYTE), 1, _pFile);
    fread(szBuffer, sizeof(char), (size_t)len, _pFile);

    _str = szBuffer;
}

void SaveWStringToFile(const wstring& _str, FILE* _pFile)
{
    BYTE len = (BYTE)_str.length();
    fwrite(&len, sizeof(BYTE), 1, _pFile);
    fwrite(_str.c_str(), sizeof(wchar_t), len, _pFile);
}

void LoadWStringFromFile(wstring& _str, FILE* _pFile)
{
    wchar_t szBuffer[256] = {};
    BYTE len = 0;
    fread(&len, sizeof(BYTE), 1, _pFile);
    fread(szBuffer, sizeof(wchar_t), (size_t)len, _pFile);

    _str = szBuffer;
}

void Vec3ToJsonFile(Json::Value& V,const string& name, const Vec3& vector)
{
    V[name.c_str()].append(vector.x);
    vector.x;
    vector.y;
    vector.z;
}

bool closeEnough(const float& a, const float& b
    , const float& epsilon = std::numeric_limits<float>::epsilon())
{
    return (epsilon > std::abs(a - b));
}

Vec3 DecomposeRotMat(const Matrix& _matRot)
{
    // _mat 을 분해 후 다시 행렬 만들기	
    Vec4 vMat[4];

    vMat[0] = Vec4(_matRot._11, _matRot._12, _matRot._13, _matRot._14);
    vMat[1] = Vec4(_matRot._21, _matRot._22, _matRot._23, _matRot._24);
    vMat[2] = Vec4(_matRot._31, _matRot._32, _matRot._33, _matRot._34);
    vMat[3] = Vec4(_matRot._41, _matRot._42, _matRot._43, _matRot._44);

    /*XMStoreFloat4(&vMat[0], _matRot._11);
    XMStoreFloat4(&vMat[1], _matRot.r[1]);
    XMStoreFloat4(&vMat[2], _matRot.r[2]);
    XMStoreFloat4(&vMat[3], _matRot.r[3]);*/

    Vec3 vNewRot;
    if (closeEnough(vMat[0].z, -1.0f)) {
        float x = 0; //gimbal lock, value of x doesn't matter
        float y = XM_PI / 2;
        float z = x + atan2f(vMat[1].x, vMat[2].x);
        vNewRot = Vec3{ x, y, z };
    }
    else if (closeEnough(vMat[0].z, 1.0f)) {
        float x = 0;
        float y = -XM_PI / 2;
        float z = -x + atan2f(-vMat[1].x, -vMat[2].x);
        vNewRot = Vec3{ x, y, z };
    }
    else { //two solutions exist
        float y1 = -asinf(vMat[0].z);
        float y2 = XM_PI - y1;

        float x1 = atan2f(vMat[1].z / cosf(y1), vMat[2].z / cosf(y1));
        float x2 = atan2f(vMat[1].z / cosf(y2), vMat[2].z / cosf(y2));

        float z1 = atan2f(vMat[0].y / cosf(y1), vMat[0].x / cosf(y1));
        float z2 = atan2f(vMat[0].y / cosf(y2), vMat[0].x / cosf(y2));

        //choose one solution to return
        //for example the "shortest" rotation
        if ((std::abs(x1) + std::abs(y1) + std::abs(z1)) <= (std::abs(x2) + std::abs(y2) + std::abs(z2)))
        {
            vNewRot = Vec3{ x1, y1, z1 };
        }
        else {
            vNewRot = Vec3{ x2, y2, z2 };
        }
    }
    return vNewRot;
}

Vec3 LookAtVector(const Vec3& EyePosition, const Vec3& FocusPosition, const Vec3& UpDirection)
{
    Vec3 zAxis = (FocusPosition - EyePosition).Normalize();
    Vec3 xAxis = UpDirection.Cross(zAxis).Normalize();
    Vec3 yAxis = zAxis.Cross(xAxis);

    Matrix Mat = {};
    Mat._11 = xAxis.x;		Mat._21 = yAxis.x;	  	Mat._31 = zAxis.x;
    Mat._12 = xAxis.y;		Mat._22 = yAxis.y;	  	Mat._32 = zAxis.y;
    Mat._13 = xAxis.z;		Mat._23 = yAxis.z;	  	Mat._33 = zAxis.z;
    Mat._14 = -xAxis.Dot(FocusPosition); Mat._24 = -yAxis.Dot(FocusPosition); Mat._34 = -zAxis.Dot(FocusPosition); Mat._44 = 1;

    Vec3 Result = DecomposeRotMat(Mat);

    return Result;
}


Matrix GetMatrixFromFbxMatrix(FbxAMatrix& _mat)
{
    Matrix mat;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            mat.m[i][j] = (float)_mat.Get(i, j);
        }
    }
    return mat;
}



const wchar_t* ToWString(COMPONENT_TYPE _type)
{
    static const wchar_t* szWString[(UINT)COMPONENT_TYPE::END] = 
    {
        L"TRANSFORM",
        L"CAMERA",
        L"COLLIDER2D",
        L"COLLIDER3D",
        L"ANIMATOR2D",
        L"ANIMATOR3D",
        L"LIGHT2D",
        L"LIGHT3D",
        L"BOUNDING",
        L"SOCKET",
        L"MESHRENDER",
        L"TILEMAP",
        L"PARTICLESYSTEM",
        L"LANDSCAPE",        
        L"DECAL",
        L"SKYBOX",
        L"FSM",
    };

    return szWString[(UINT)_type];
}

const char* ToString(COMPONENT_TYPE _type)
{
    static const char* szString[(UINT)COMPONENT_TYPE::END] =
    {
        "TRANSFORM",
        "CAMERA",
        "COLLIDER2D",
        "COLLIDER3D",
        "ANIMATOR2D",
        "ANIMATOR3D",
        "LIGHT2D",
        "LIGHT3D",
        "BOUNDING",
        "SOCKET",
        "MESHRENDER",
        "TILEMAP",
        "PARTICLESYSTEM",
        "LANDSCAPE",
        "DECAL",
        "SKYBOX",
        "FSM",
    };

    return szString[(UINT)_type];
}

const wchar_t* ToWString(RES_TYPE _type)
{
    static const wchar_t* szWString[(UINT)RES_TYPE::END] =
    {
        L"PREFAB",
        L"MESHDATA",
        L"MATERIAL",
        L"GRAPHICS_SHADER",
        L"COMPUTE_SHADER",
        L"MESH",
        L"TEXTURE",
        L"SOUND",
        L"SCENEFILE"
    };

    return szWString[(UINT)_type];
}

const char* ToString(RES_TYPE _type)
{
    static const char* szString[(UINT)RES_TYPE::END] =
    {
        "PREFAB",
        "MESHDATA",
        "MATERIAL",
        "GRAPHICS_SHADER",
        "COMPUTE_SHADER",
        "MESH",
        "TEXTURE",
        "SOUND",
        "SCENEFILE"
    };

    return szString[(UINT)_type];
}


#define W 32
#define R 32
#define M1 3
#define M2 24
#define M3 10

#define MAT0POS(t,v) (v^(v>>t))
#define MAT0NEG(t,v) (v^(v<<(-(t))))
#define Identity(v) (v)

#define V0            STATE[state_i                   ]
#define VM1           STATE[(state_i+M1) & 0x0000001fU]
#define VM2           STATE[(state_i+M2) & 0x0000001fU]
#define VM3           STATE[(state_i+M3) & 0x0000001fU]
#define VRm1          STATE[(state_i+31) & 0x0000001fU]
#define newV0         STATE[(state_i+31) & 0x0000001fU]
#define newV1         STATE[state_i                   ]

#define FACT 2.32830643653869628906e-10

static unsigned int state_i = 0;
static unsigned int STATE[R];
static unsigned int z0, z1, z2;

void InitWELLRNG1024a(unsigned int* init) {
    int j;
    state_i = 0;
    for (j = 0; j < R; j++)
        STATE[j] = init[j];
}

double WELLRNG1024a(void) {
    z0 = VRm1;
    z1 = Identity(V0) ^ MAT0POS(8, VM1);
    z2 = MAT0NEG(-19, VM2) ^ MAT0NEG(-14, VM3);
    newV1 = z1 ^ z2;
    newV0 = MAT0NEG(-11, z0) ^ MAT0NEG(-7, z1) ^ MAT0NEG(-13, z2);
    state_i = (state_i + 31) & 0x0000001fU;
    return ((double)STATE[state_i] * FACT);
}

void VecToJson(Json::Value* node, const string& str, Vec4 v)
{
    (*node)[str].append(v.x);
    (*node)[str].append(v.y);
    (*node)[str].append(v.z);
    (*node)[str].append(v.w);
}

void VecToJson(Json::Value* node, const string& str, Vec3 v)
{
    (*node)[str].append(v.x);
    (*node)[str].append(v.y);
    (*node)[str].append(v.z);
}

void VecToJson(Json::Value* node, const string& str, Vec2 v)
{
    (*node)[str].append(v.x);
    (*node)[str].append(v.y);
}



void SaveWStringToJson(const wstring& _wstr, Json::Value* node)
{
    string str = string(_wstr.begin(), _wstr.end());
    (*node)[str] = str;
}

void SaveWStringToJson(const string& _str, const wstring& _wstr, Json::Value* node)
{
    string str = string(_wstr.begin(), _wstr.end());
    (*node)[_str] = str;
}

wstring LoadWStringFromJson(const string _str, Json::Value* node)
{
    string str = (*node)[_str].asString();

    wstring wstr = wstring(str.begin(), str.end());

    return wstr;
}



void LoadVecFromJson(Json::Value& node, Vec2& v)
{
    v.x = node[0].asFloat();
    v.y = node[1].asFloat();
}

void LoadVecFromJson(Json::Value& node, Vec3& v)
{
    v.x = node[0].asFloat();
    v.y = node[1].asFloat();
    v.z = node[2].asFloat();
}

void LoadVecFromJson(Json::Value& node, Vec4& v)
{
    v.x = node[0].asFloat();
    v.y = node[1].asFloat();
    v.z = node[2].asFloat();
    v.w = node[3].asFloat();
}

