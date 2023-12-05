#pragma once
#include "CEntity.h"

#include "CGameObject.h"

class CTransform;
class CMeshRender;

#define GET_OTHER_COMPONENT(Type) C##Type* Type() {return m_pOwner->Type();}




class CComponent :
    public CEntity
{
private:
    CGameObject*         m_pOwner;
    const COMPONENT_TYPE m_Type;

public:
    COMPONENT_TYPE GetType() { return m_Type; }
    CGameObject* GetOwner() { return m_pOwner ; }

public:
    virtual void begin() {}
    virtual void tick() {}
    virtual void finaltick() = 0;
    virtual CComponent* Clone() = 0;

public:
    virtual void SaveToLevelFile(FILE* _File) = 0;
    virtual void LoadFromLevelFile(FILE* _FILE) = 0;

public:
    GET_OTHER_COMPONENT(Transform);
    GET_OTHER_COMPONENT(MeshRender);
    GET_OTHER_COMPONENT(Camera);
    GET_OTHER_COMPONENT(Collider2D);
    GET_OTHER_COMPONENT(Light2D);
    GET_OTHER_COMPONENT(TileMap);
    GET_OTHER_COMPONENT(Animator2D);

public:
    CComponent(COMPONENT_TYPE _Type);
    CComponent(const CComponent& _Other);
    virtual ~CComponent();

    friend class CGameObject;
};

