#pragma once

#include <Engine\CGameObject.h>




class CGameObjectEx :
    public CGameObject
{
private:

public:
    virtual void finaltick() override;
};

