#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer :
    public CEntity
{
private:
    vector<CGameObject*>    m_vecParentObj; // 레이어에 속해있는 부모오브젝트
    vector<CGameObject*>    m_vecObject;    // 계층에 상관없이 해당 레이어에 속해있는 모든 오브젝트
    int                     m_iLayerIdx;    // 레이어 번호 (0~31)


public:    
    void begin();
    void tick();
    void finaltick(); 


public:
    int GetLayerIndex() { return m_iLayerIdx; }
    const vector<CGameObject*>& GetParentObject() { return m_vecParentObj; }
    const vector<CGameObject*>& GetObjects() { return m_vecObject; }


private:
    // Layer 에 부모오브젝트 입력
    // _bMove : 자식 오브젝트들이 부모 오브젝트를 따라서 이동 할 것인지 체크
    void AddGameObject(CGameObject* _Object, bool _bMove);
    void RegisterObject(CGameObject* _Object){ m_vecObject.push_back(_Object); }    
    void RemoveFromParentList(CGameObject* _Obj);
    void AddParentList(CGameObject* _Obj);

    CLONE(CLayer)
public:
    CLayer();
    ~CLayer();

    friend class CLevel;
    friend class CGameObject;
    friend class CEventMgr;
};

