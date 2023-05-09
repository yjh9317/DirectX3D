#pragma once
#include "CRenderComponent.h"


#include "CRaycastShader.h"
#include "CHeightMapShader.h"
#include "CWeightMapShader.h"
#include "CPreivewMapShader.h"

class CStructuredBuffer;

class CLandScape :
    public CRenderComponent
{
private:
    Ptr<CTexture>           m_pHeightMap;
    Ptr<CTexture>           m_pWeightTex;

    UINT                    m_iXFaceCount;
    UINT                    m_iZFaceCount;

    Ptr<CRaycastShader>     m_pCSRaycast;   // 픽킹 쉐이더
    CStructuredBuffer*      m_pCrossBuffer;	// 마우스 피킹되는 지점 정보 받는 버퍼

    Ptr<CHeightMapShader>   m_pCSHeightMap; // 높이맵 쉐이더
    Ptr<CTexture>           m_pBrushArrTex; // 브러쉬용 텍스쳐
    Ptr<CTexture>           m_pPreviewTex;

    Ptr<CPreivewMapShader>  m_pCSPreviewMap;

    Ptr<CWeightMapShader>   m_pCSWeightMap;     // 가중치 쉐이더
    CStructuredBuffer*      m_pWeightMapBuffer; // 가중치 저장 버퍼
    UINT					m_iWeightWidth;		// 가중치 버퍼 가로세로 행렬 수
    UINT					m_iWeightHeight;	// 가중치 버퍼 가로세로 행렬 수
    UINT                    m_iWeightIdx;		// 증가 시킬 가중치 부위

    Ptr<CTexture>           m_pTileArrTex;  // 타일 배열 텍스쳐

    Vec2                    m_vBrushScale;
    int                     m_iBrushIdx;    // 브러쉬 인덱스
    int                     m_iBrushDir;
    float                   m_fBrushPow;
    float                   m_fBrushMod;

    LANDSCAPE_MOD           m_eMod; 	// 지형 툴모드에서 상태값

    bool                    m_bLoad;
    bool                    m_bWeightLoad;

    Ptr<CTexture>           m_WeightLoadTex;

public:
    virtual void finalupdate() override;
    virtual void render() override;
    

public:
    void SetWeightLoadTex(Ptr<CTexture> pTex) { m_WeightLoadTex = pTex; }

    Ptr<CTexture> GetHeightMap() { return m_pHeightMap; }
    void SetHeightMap(Ptr<CTexture> pTex) { m_pHeightMap = pTex; }

    Ptr<CTexture> GetWeightMap() { return m_pWeightTex; }
    void SetWeightMap(Ptr<CTexture> pTex) { m_pWeightTex = pTex; }

    Ptr<CHeightMapShader>& GetHeightMapShader() { return m_pCSHeightMap; }
    Ptr<CWeightMapShader>& GetWeightMapShader() { return m_pCSWeightMap; }

    void SetFaceCount(UINT _iX, UINT _iZ)  { m_iXFaceCount = _iX; m_iZFaceCount = _iZ; }
    void Create();

    Vec2 GetBrushScale() { return m_vBrushScale; }
    void SetBrushScale(Vec2 _v) { m_vBrushScale = _v; }

    int GetBrushIdx() { return m_iBrushIdx; }
    void SetBrushIdx(int _i) { m_iBrushIdx=_i; }

    int GetBrushDir() { return m_iBrushDir; }
	void SetBrushDir(int _i) { m_iBrushDir = _i; }

    float GetBrushPow() { return m_fBrushPow; }
    void SetBrushPow(float _f) { m_fBrushPow = _f; }

    UINT GetWeightIndex() { return m_iWeightIdx; }
    void SetWeightIndex(UINT _i) { m_iWeightIdx = _i; }

    void SetLoad(bool _b) { m_bLoad = _b; }
    void SetWeightLoad(bool _b) { m_bWeightLoad = _b; }

    UINT GetFaceXCount() { return m_iXFaceCount; }
    UINT GetFaceZCount() { return m_iZFaceCount; }

    void WeightExecute();
    void WeightLoad();
    void HeightLoad();

private:
    void Raycasting();
    

private:
    void CreateShaderMaterial();
    void CreateMesh();
    void CreateTexture();

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;

    virtual void SaveJsonFile(Json::Value& root) override;
    virtual void LoadJsonFromFile(Json::Value& root) override;
public:
    CLONE(CLandScape);
    CLandScape();
    ~CLandScape();
};

 