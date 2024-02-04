#pragma once
#include "CRes.h"
#include "CFBXLoader.h"

class CStructuredBuffer;

class CBone :
    public CRes
{
private:
    vector<tMTAnimClip> m_vecAnimClip;
    vector<tMTBone>     m_vecBones;
    CStructuredBuffer*  m_pBoneFrameData;    // 전체 본 프레임 정보 ( 크기, 이동, 회전 ) (프레임 갯수만큼)
    CStructuredBuffer*  m_pBoneOffset;       // 각 뼈의 offset 행렬 ( 각 뼈의 위치를 되돌리는 행렬) (1행짜리)

    UINT    m_iFrmCnt;

public:
    const vector<tMTAnimClip>* GetAnimClip() { return &m_vecAnimClip; }
    const vector<tMTBone>* GetBones() { return &m_vecBones; }
    UINT GetBoneCount() { return (UINT)m_vecBones.size(); }
    UINT GetFrameCount() { return m_iFrmCnt; }

    CStructuredBuffer* GetBoneFrameDataBuffer() { return m_pBoneFrameData; }    // 전체 본 프레임 정보
    CStructuredBuffer* GetBoneOffsetBuffer() { return m_pBoneOffset; }  // 각 뼈의 offset 행렬

public:
    static CBone* CreateFromContatiner(CFBXLoader& _loader);
    static CBone* LoadFbx(const wstring& _strPath);

private:
    virtual int Load(const wstring& _strFilePath);

public:
    virtual int Save(const wstring& _strRelativePath);

public:
    CBone(bool _bEngine = false);
    ~CBone();
};

