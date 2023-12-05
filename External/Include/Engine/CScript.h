#pragma once
#include "CComponent.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

#include "CResMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "CGameObject.h"
#include "components.h"


enum class SCRIPT_PARAM
{
    INT,
    FLOAT,
    VEC2,
    VEC4,
};

struct tScriptParam
{
    SCRIPT_PARAM    eParam;
    void*           pData;    
    string          strDesc;
};

class CCollider2D;

class CScript :
    public CComponent
{
private:  
    UINT                    m_iScriptType;
    vector<tScriptParam>    m_vecParam;


public:
    void Destroy() { DestroyObject(GetOwner()); }
    void SetLifeSpan(float _Time) { GetOwner()->SetLifeSpan(_Time); }
    UINT GetScriptType() { return m_iScriptType; }
    const vector<tScriptParam>& GetScritpParam() { return m_vecParam; }

public:   
    virtual void finaltick() final {};
    virtual void BeginOverlap(CCollider2D* _Other) {}
    virtual void OnOverlap(CCollider2D* _Other) {}
    virtual void EndOverlap(CCollider2D* _Other) {}

public:
    virtual void SaveToLevelFile(FILE* _File) override {}
    virtual void LoadFromLevelFile(FILE* _FILE) override {}

protected:
    void AddScriptParam(SCRIPT_PARAM eParam, void* _pData, const string& _Desc);

public:
    CScript(UINT _iScriptType);
    ~CScript();
};

