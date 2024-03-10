#pragma once
#include <Engine/CSingleton.h>
#include "ER_Character.h"

class ER_CharacterMgr :
    public CSingleton<ER_CharacterMgr>
{
    SINGLE(ER_CharacterMgr);

private:
    map<wstring, ER_Character*> m_mapCharacters;  // 캐릭터 오브젝트풀

private:
    int Save();
    int Load();

    int SaveCharacterData(ER_Character* _Character, FILE* _File);
    ER_Character* LoadCharacterData(FILE* _File);


public:
    void init();
    void tick();

    int SpawnCharacter(const wstring& _key);

    friend class CharacterDataUI;
};