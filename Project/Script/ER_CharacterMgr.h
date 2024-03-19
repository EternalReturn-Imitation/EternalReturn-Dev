#pragma once
#include <Engine/CSingleton.h>
#include <Engine/CGameObject.h>

class ER_CharacterMgr :
    public CSingleton<ER_CharacterMgr>
{
    SINGLE(ER_CharacterMgr);

private:
    map<wstring, CGameObject*> m_mapCharacters;  // 캐릭터 오브젝트풀

private:
    int Save();
    int Load();

    int SaveCharacterData(CGameObject* _Character, FILE* _File);
    CGameObject* LoadCharacterData(FILE* _File);

    CGameObject* SpawnCharacter(const wstring& _key);
public:
    void init();

    CGameObject* SpawnCharacter_Player(const wstring& _key,Vec3 _Pos);
    CGameObject* SpawnCharacter_Enemy(const wstring& _key, Vec3 _Pos);

    friend class CharacterDataUI;
};