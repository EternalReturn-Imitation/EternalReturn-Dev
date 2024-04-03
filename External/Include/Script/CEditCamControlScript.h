#pragma once
#include <Engine\CScript.h>

class CEditCamControlScript :
    public CScript
{
private:
    float       m_fCamSpeed;

    bool       b_RBTNPressed;

public:
    virtual void tick() override;

private:
    void Camera2DMove();
    void Camera3DMove();

public:
    bool GetRBTNPressed() { return b_RBTNPressed; }
    void SetRBTNPressed(bool _pressed) { b_RBTNPressed = _pressed; }

    CLONE(CEditCamControlScript);
public:
    CEditCamControlScript();
    ~CEditCamControlScript();
};

