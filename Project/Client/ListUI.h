#pragma once
#include "UI.h"

class ListUI :
    public UI
{
private:
    vector<string>          m_vecStrData; 
    int                     m_iSelectedIdx;

    UI*                     m_SelectInst;
    UI_DELEGATE_1           m_SelectDelegate;



public:
    virtual void finaltick() override;
    virtual int render_update() override;

public:
    void Reset(const string& _strName, ImVec2 _vSize);
    void AddItem(const string& _strItem) {  m_vecStrData.push_back(_strItem); }
    void Clear() { m_vecStrData.clear(); m_iSelectedIdx = -1; }

    void AddDynamic_Select(UI* _Inst, UI_DELEGATE_1 _Func)
    {
        m_SelectInst = _Inst;
        m_SelectDelegate = _Func;
    }


public:
    ListUI();
    ~ListUI();
};

