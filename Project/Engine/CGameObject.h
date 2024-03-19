#pragma once
#include "CEntity.h"


class CComponent;
class CTransform;
class CMeshRender;
class CParticleSystem;
class CCollider2D;
class CAnimator2D;
class CAnimator3D;
class CLight2D;
class CLight3D;
class CCamera;
class CText;
class CRenderComponent;
class CSkyBox;
class CTileMap;
class CDecal;
class CLandScape;
class CScript;
class CBehaviorTree;
class CCollider3D;
class CFindPath;
class CUIComponent;
class CUI_Button;

#define GET_COMPONENT(Type, TYPE) C##Type* Type() const { return (C##Type*)m_arrCom[(UINT)COMPONENT_TYPE::TYPE]; }



class CGameObject :
    public CEntity
{
private:
    CComponent*         m_arrCom[(UINT)COMPONENT_TYPE::END];
    CRenderComponent*   m_RenderCom;
    CText* m_TextCom;


    CUIComponent*       m_UICom;
    vector<CScript*>    m_vecScript;

    CGameObject* m_Parent;
    vector<CGameObject*>    m_vecChild;

    int                     m_iLayerIdx; // 소속된 레이어 인덱스값
    bool                    m_bDead;
    float                   m_LifeTime;
    float                   m_CurLifeTime;
    bool                    m_bLifeSpan;
    bool                    m_bEnable;

public:
    // 레벨이 시작될 때 호출 or 시작 된 레벨에 합류할 때 호출
    // 생성자
    virtual void begin();

    virtual void tick();
    virtual void finaltick();
    void finaltick_module();
    void render();
    void render_shadowmap();

public:
    void AddComponent(CComponent* _Component);
    void AddChild(CGameObject* _Object);

    CComponent* GetComponent(COMPONENT_TYPE _ComType) { return m_arrCom[(UINT)_ComType]; }
    const vector<CGameObject*>& GetChild() { return m_vecChild; }

    CGameObject* GetParent() const { return m_Parent; }

    GET_COMPONENT(Transform, TRANSFORM);
    GET_COMPONENT(MeshRender, MESHRENDER);
    GET_COMPONENT(ParticleSystem, PARTICLESYSTEM);
    GET_COMPONENT(Camera, CAMERA);
    GET_COMPONENT(Text, TEXTCOMP);
    GET_COMPONENT(Collider2D, COLLIDER2D);
    GET_COMPONENT(Light2D, LIGHT2D);
    GET_COMPONENT(TileMap, TILEMAP);
    GET_COMPONENT(Animator2D, ANIMATOR2D);
    GET_COMPONENT(Animator3D, ANIMATOR3D);
    GET_COMPONENT(BehaviorTree, BEHAVIORTREE);
    GET_COMPONENT(Light3D, LIGHT3D);
    GET_COMPONENT(SkyBox, SKYBOX);
    GET_COMPONENT(Decal, DECAL);
    GET_COMPONENT(LandScape, LANDSCAPE);
    GET_COMPONENT(Collider3D, COLLIDER3D);
    GET_COMPONENT(FindPath, FINDPATH);
    GET_COMPONENT(UI_Button, UI_BUTTON);

    CRenderComponent* GetRenderComponent() const { return m_RenderCom; }
    CText* GetTextComponent() const { return m_TextCom; }
    CUIComponent* GetUIComponent() const { return m_UICom; }

    int GetLayerIndex() { return m_iLayerIdx; }

    template<typename T>
    T* GetScript();

    const vector<CScript*>& GetScripts() { return m_vecScript; }

    void SetLifeSpan(float _fTime)
    {
        if (m_bLifeSpan)
            return;

        m_LifeTime = _fTime;
        m_bLifeSpan = true;
    }

    bool IsDead() { return m_bDead; }
    bool IsAncestor(CGameObject* _Target);
    bool IsEnable() { return m_bEnable; }

    void SetEnable(bool _b) { m_bEnable = _b; }

private:
    void DisconnectFromParent();
    void ChangeToChildType();
    void AddParentList();


    CLONE(CGameObject)
public:
    CGameObject();
    CGameObject(const CGameObject& _Other);
    ~CGameObject();

    friend class CLayer;
    friend class CEventMgr;
};

template<typename T>
inline T* CGameObject::GetScript()
{
    for (size_t i = 0; i < m_vecScript.size(); ++i)
    {
        T* pScript = dynamic_cast<T*> (m_vecScript[i]);
        if (nullptr != pScript)
            return pScript;
    }

    return nullptr;
}
