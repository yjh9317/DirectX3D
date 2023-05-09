#pragma once


struct tEventInfo
{
	EVENT_TYPE	eType;
	DWORD_PTR	lParam;
	DWORD_PTR	wParam;
};


struct Vertex
{
	Vec3 vPos;		// ��ǥ	
	Vec4 vColor;	// ����
	Vec2 vUV;

	Vec3 vTangent;	// ����
	Vec3 vNormal;	// ����
	Vec3 vBinormal; // ������

	// 3DAnimation
	Vec4 vWeights;
	Vec4 vIndices;	
};

typedef Vertex Vtx;



// =======
// TileMap
// =======
struct tTileData
{
	int		iImgIdx;
	Vec2	vLTUV;
	int		iPadding;

	tTileData()
		: iImgIdx(-1)
		, iPadding(0)
	{}
};


// ========
// Material
// ========
struct tMtrlData
{
	Vec4 vDiff;
	Vec4 vSpec;
	Vec4 vAmb;
	Vec4 vEmv;
};


// ===========
// Animtion 2D
// ===========
struct tAnim2DFrame
{
	Vec2	vLT;
	Vec2	vSlice;
	Vec2	vOffset;
	float	fDuration;
};



// ============
// Animation 3D
// ============
struct tFrameTrans
{
	Vec4	vTranslate;
	Vec4	vScale;
	Vec4	qRot;
};

struct tMTKeyFrame
{
	double	dTime;
	int		iFrame;
	Vec3	vTranslate;
	Vec3	vScale;
	Vec4	qRot;
};


struct tMTBone
{
	wstring				strBoneName;
	int					iDepth;
	int					iParentIndx;
	Matrix				matOffset;	// Offset ���(�� -> ��Ʈ ������ ���)
	Matrix				matBone;   // �̰� �Ⱦ�
	vector<tMTKeyFrame>	vecKeyFrame;
};

struct tMTAnimClip
{
	wstring			strAnimName;
	int				iStartFrame;
	int				iEndFrame;
	int				iFrameLength;

	double			dStartTime;
	double			dEndTime;
	double			dTimeLength;
	float			fUpdateTime; // �̰� �Ⱦ�

	FbxTime::EMode	eMode;
};



//===============
// Struct of FBX 
//===============
struct tFbxMaterial
{
	tMtrlData	tMtrl;
	wstring     strMtrlName;
	wstring     strDiff;
	wstring		strNormal;
	wstring		strSpec;
};

struct tWeightsAndIndices
{
	int		iBoneIdx;
	double	dWeight;
};

struct tContainer
{
	wstring								strName;
	vector<Vec3>						vecPos;
	vector<Vec3>						vecTangent;
	vector<Vec3>						vecBinormal;
	vector<Vec3>						vecNormal;
	vector<Vec2>						vecUV;

	vector<Vec4>						vecIndices;
	vector<Vec4>						vecWeights;

	vector<vector<UINT>>				vecIdx;
	vector<tFbxMaterial>				vecMtrl;

	// Animation ���� ����
	bool								bAnimation;
	vector<vector<tWeightsAndIndices>>	vecWI;

	void Resize(UINT _iSize)
	{
		vecPos.resize(_iSize);
		vecTangent.resize(_iSize);
		vecBinormal.resize(_iSize);
		vecNormal.resize(_iSize);
		vecUV.resize(_iSize);
		vecIndices.resize(_iSize);
		vecWeights.resize(_iSize);
		vecWI.resize(_iSize);
	}
};

struct tKeyFrame
{
	FbxAMatrix  matTransform;
	double		dTime;
};

struct tBone
{
	wstring				strBoneName;
	int					iDepth;			// �������� ����
	int					iParentIndx;	// �θ� Bone �� �ε���
	FbxAMatrix			matOffset;		// Offset ���( -> �Ѹ� -> Local)
	FbxAMatrix			matBone;
	vector<tKeyFrame>	vecKeyFrame;
};

struct tAnimClip
{
	wstring		strName;
	FbxTime		tStartTime;
	FbxTime		tEndTime;
	FbxLongLong	llTimeLength;
	FbxTime::EMode eMode;
};




// ========
// Particle 
// ========
struct tParticle
{
	Vec3	vPos;
	Vec3	vScale;
	Vec3	vDir;
	Vec4	vColor;

	int		Alive;
	float   m_fCurTime; // ���� �ð�
	float	m_fMaxTime; // �ִ� ���� �ð�
};

struct tParticleData
{
	int		iAliveCount;
	Vec3	vPadding;
};


struct tLightColor
{
	Vec4 vDiff;	// Diffuse Light	���ݻ籤
	Vec4 vSpec; // Specular Light	�ݻ籤
	Vec4 vAmb;  // Ambient Light	ȯ�汤
};

struct tLightInfo
{
	tLightColor color;
	Vec3		vLightDir;	
	int			iLightType;
	Vec3		vWorldPos;
	float		fAngle;
	float		fRange;
	Vec3		vPadding;
};

// ���� ����ü
struct tRay
{
	Vec3 vStart;
	Vec3 vDir;
};

// Raycast ����� ���� ����ü
struct tRaycastOut
{
	Vec2 vUV;
	int  iDist;
	int  bSuccess;
};


struct tWeight_4
{
	float arrWeight[4];
};

// ===================
// ������� ���� ����ü
// ===================
struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;

	Matrix matWV;
	Matrix matWVP;

	Matrix matWorldInv;
	Matrix matViewInv;
};

struct tScalarParam
{
	tMtrlData mtrl;

	int iArr[4];
	float fArr[4];
	Vec2 v2Arr[4];
	Vec4 v4Arr[4];
	Matrix matArr[4];

	int bTex[10];
	int	arrAnimData[2];	// 3D Animation ����
};

struct tAnim2D
{
	Vec2	vLT;
	Vec2	vSlice;
	Vec2	vBackgroundSize;
	Vec2    vOffset;

	int     useAnim2D;
	float   Atlas_Width;
	float   Atlas_Height;
	float	Anim2D_Padding;
};

struct tGlobal
{
	Vec2	vResolution; // RenderTarget Resolution
	Vec2    vNoiseResolution; // NoiseTexture Resolution
	float	fDT;
	float	fAccTime;
	int		Light2DCount;
	int     Light3DCount;
};