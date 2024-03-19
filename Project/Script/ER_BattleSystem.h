#pragma once
#include <Engine\CSingleton.h>

class ER_BattleSystem :
    public CSingleton<ER_BattleSystem>
{
    SINGLE(ER_BattleSystem);
private:
    

public:
    void init();

public:
    void Battle(CGameObject* Attacker, CGameObject* Target, int Dmg);

};

