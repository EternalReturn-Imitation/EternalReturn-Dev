#pragma once
#include "UI.h"

#include "ER_Item.h"
#include "ER_ItemMgr.h"

enum class ItemDataUItab
{
    ITEMDATA,
    RECIPE,
    END,
};

class ItemDataUI :
    public UI
{
private:
    bool m_bOpenTab[(UINT)ItemDataUItab::END];
    string m_bTabName[(UINT)ItemDataUItab::END];
    
    vector<ER_Item*>* m_vecItem;						// 아이템 리스트
    unordered_map<DWORD_PTR, UINT>* m_umapRecipe;

    ImGuiTableFlags ItemDataUIFlags;

    ER_Item*        m_pCurItem;

    bool            m_bItemStatsEditWin;
    ER_Item*        m_pCurStatsEditItem;
    Ptr<CTexture>   m_pEmptyItemSlotTex;

    bool           m_bItemPopup;
    int            m_iDeleteItemIdx;

    ER_Item*       m_DragItem;
    ER_Item*       m_DropItem;
    int            m_iDragItemIdx;
    int            m_iDropItemIdx;

    vector<string> m_vecItemName;

public:
    virtual void init() override;
    virtual void tick() override;
    virtual void finaltick() override;
    virtual int render_update() override;

    void RegistItemMgr();

private:
    void render_menubar();
    void render_Tabs();
    void render_ItemInfoTable();
    void render_ItemRecipeTable();
    void render_ItemStatEdit();

    void Print_Stats(const ER_tStats& _stats);

    void SwapItem();
    void ItemPopUp();
    void SelectItemIcon(DWORD_PTR _data, DWORD_PTR _target);

public:
    ItemDataUI();
    virtual ~ItemDataUI();
};

