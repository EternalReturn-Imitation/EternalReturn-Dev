#include "pch.h"
#include "ER_CharacterMgr.h"

#include <Engine\CPathMgr.h>

#include <Script\ER_Data_CharacterScript.h>

int ER_CharacterMgr::Save()
{
    path path_content = CPathMgr::GetInst()->GetContentPath();
    path path_character = path_content.wstring() + L"Data";

    if (false == exists(path_character))
    {
        create_directory(path_character);
    }

    wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
    strFilePath += L"Data\\Character.data";

    FILE* pFile = nullptr;
    _wfopen_s(&pFile, strFilePath.c_str(), L"wb");

    // Character Cnt
    size_t iCharacterCnt = m_mapCharacters.size();
    fwrite(&iCharacterCnt, sizeof(size_t), 1, pFile);

    map<wstring, CGameObject*>::iterator iter = m_mapCharacters.begin();

    for (size_t i = 0; i < iCharacterCnt; ++i)
    {
        SaveCharacterData(iter->second, pFile);
        iter++;
    }
    
    fclose(pFile);

    return S_OK;
}

int ER_CharacterMgr::Load()
{
    wstring strPath = CPathMgr::GetInst()->GetContentPath();
    strPath += L"Data\\Character.data";

    FILE* pFile = nullptr;
    _wfopen_s(&pFile, strPath.c_str(), L"rb");

    if (nullptr == pFile)
        return S_FALSE;

    // Character Cnt
    size_t iCharacterCnt = 0;
    fread(&iCharacterCnt, sizeof(size_t), 1, pFile);

    for (size_t i = 0; i < iCharacterCnt; ++i)
    {
        LoadCharacterData(pFile);
    }

    fclose(pFile);

    return S_OK;
}

int ER_CharacterMgr::SaveCharacterData(CGameObject* _Character, FILE* _File)
{
    ER_Data_CharacterScript* CharacterContext = _Character->GetScript<ER_Data_CharacterScript>();

    // Character Key
    SaveWString(CharacterContext->m_strKey, _File);

    // Character Name
    SaveWString(CharacterContext->m_strName, _File);

    // Character Std Stats
    fwrite(&CharacterContext->m_STDStats, sizeof(ER_STD_CHAR_STATS), 1, _File);

    return 0;
}

CGameObject* ER_CharacterMgr::LoadCharacterData(FILE* _File)
{
    CGameObject* pCharacter = new CGameObject;
    pCharacter->AddComponent(new ER_Data_CharacterScript());

    ER_Data_CharacterScript* CharacterContext = pCharacter->GetScript<ER_Data_CharacterScript>();

    // Character Key
    LoadWString(CharacterContext->m_strKey, _File);

    // Character Name
    LoadWString(CharacterContext->m_strName, _File);

    // Character Std Stats
    fread(&CharacterContext->m_STDStats, sizeof(ER_STD_CHAR_STATS), 1, _File);

    // CharacterData init
    CharacterContext->init();

    m_mapCharacters.insert(make_pair(CharacterContext->m_strKey, pCharacter));

    return pCharacter;
}
