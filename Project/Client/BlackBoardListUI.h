#pragma once
#include "UI.h"

#include <Engine\CBehaviorTree.h>

class BlackBoardListUI :
    public UI
{
private:
    enum BBListShowFlag
    {
        BBListShowFlag_None         = 0,
        BBListShowFlag_int          = 1 << 0,
        BBListShowFlag_float        = 1 << 1,
        BBListShowFlag_GameObject   = 1 << 2,
        BBListShowFlag_string       = 1 << 3,
        BBListShowFlag_wstring      = 1 << 4,
        BBListShowFlag_All          = BBListShowFlag_int | BBListShowFlag_float | BBListShowFlag_GameObject | BBListShowFlag_string | BBListShowFlag_wstring
    };

private:
    BB* m_pTargetBB;
    UINT m_eFlags;
    unordered_map<string, CGameObject*> m_mapGameObject;
    CGameObject* m_pCreateObject;
    bool m_bCreateDataWindow;

private:
    void render_ManuBar();
    void render_TabBar(ImGuiTabBarFlags _flags);
    void render_Table(ImGuiTableFlags _flags);
    void render_CreateDataWindow();

    void SelectObject(DWORD_PTR _Key);

public:
    virtual void init() override;
    virtual void tick() override;
    virtual int render_update() override;

public:
    void    SetBlackBoard(BB* _Target);
    BB*     GetBlackBoard() { return m_pTargetBB; }
    string  GetDataStr(BB::tBBData* _Data, const char* _DataType);
    bool    IsShowType(const char* _type);

public:
    BlackBoardListUI();
    ~BlackBoardListUI();
};

