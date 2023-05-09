#pragma once
#include "CComponent.h"



#include "Ptr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CAnimation3D.h"

class CStructuredBuffer;

class CAnimator3D :
    public CComponent
{
private:
    map<wstring, CAnimation3D*> m_mapAnim;
    CAnimation3D*               m_pCurAnim;
    CAnimation3D*               m_pPrevAnim;

    bool                        m_bRepeat;
    bool                        m_bPlay;

    const vector<tMTBone>*      m_pVecBones;
    const vector<tMTAnimClip>*  m_pVecClip;

    vector<float>				m_vecClipUpdateTime;
    vector<Matrix>				m_vecFinalBoneMat; // 텍스쳐에 전달할 최종 행렬정보

    CStructuredBuffer*          m_pBoneFinalMatBuffer;  // 특정 프레임의 최종 행렬
    bool						m_bFinalMatUpdate; // 최종행렬 연산 수행여부

    CStructuredBuffer*          m_pSocketBuffer;   
    float                       m_BoneIndex;

    CStructuredBuffer*          m_pSocketBuffer2;   
    float                       m_BoneIndex2;

public:
    virtual void finalupdate() override;
    virtual void UpdateData() override;

public:
    void SetBones(const vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; m_vecFinalBoneMat.resize(m_pVecBones->size()); }
    void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip);    
    void SetClipTime(int _iClipIdx, float _fTime) { m_vecClipUpdateTime[_iClipIdx] = _fTime; }

    
    CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMatBuffer; }
    UINT GetBoneCount() { return (UINT)m_pVecBones->size(); }
    const vector<tMTBone>*& GetBoneInfo() { return m_pVecBones; }
    void ClearData();

    CStructuredBuffer* GetSocket() { return m_pSocketBuffer; }
    CStructuredBuffer* GetSocket2() { return m_pSocketBuffer2; }
    
    CAnimation3D* CreateAnim(const wstring& _strName,int _ClipIdx, float _fStart, float _fEnd, int _AnimFrame);
    CAnimation3D* FindAnim(const wstring& _strName);
    void Play(const wstring& _strName, bool _bRepeat);
    map<wstring, CAnimation3D*>& GetAnimList() { return m_mapAnim; }
    const vector<tMTAnimClip>*& GetAnimClip() { return m_pVecClip; }

    CAnimation3D* GetCurAnim() { return m_pCurAnim; }
    CAnimation3D* GetPrevAnim() { return m_pPrevAnim; }

    bool IsRepeat() { return m_bRepeat; }
    void SetRepeat(bool _b) { m_bRepeat = _b; }
    
    bool IsPlay() { return m_bPlay; }
    void SetPlay(bool _b) { m_bPlay = _b; }

    float GetBoneIdx() { return m_BoneIndex; }
    void SetBoneIdx(float _f) { m_BoneIndex = _f; }

    float GetBoneIdx2() { return m_BoneIndex2; }
    void SetBoneIdx2(float _f) { m_BoneIndex2 = _f; }

private:
    void check_mesh(Ptr<CMesh> _pMesh);

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;

    virtual void SaveJsonFile(Json::Value& root) override;
    virtual void LoadJsonFromFile(Json::Value& root) override;

    // 애니메이션의 정보를 모아놓은 애니메이터를 따로 저장
    int Save(const wstring& _strFileName); 
    int Load(const wstring& _strFileName);

    CLONE(CAnimator3D);

public:
    CAnimator3D();
    CAnimator3D(const CAnimator3D& _origin);
    ~CAnimator3D();
};


