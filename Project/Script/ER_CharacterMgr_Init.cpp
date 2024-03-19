#include "pch.h"
#include "ER_CharacterMgr.h"

#include <Engine\CPathMgr.h>

#include "ER_DataScript_Character.h"
#include "CScriptMgr.h"

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
    ER_DataScript_Character* CharacterContext = _Character->GetScript<ER_DataScript_Character>();

    // Character Key
    SaveWString(CharacterContext->m_strKey, _File);

    // Character Name
    SaveWString(CharacterContext->m_strName, _File);

    // Character Std Stats
    fwrite(&CharacterContext->m_STDStats, sizeof(ER_Initial_Stats), 1, _File);

    int SkillSize = CharacterContext->m_Skill.size();
    fwrite(&SkillSize, sizeof(int), 1, _File);

    for (int i = 0; i < SkillSize; ++i)
    {
        CharacterContext->m_Skill[i]->Save(_File);
        SaveResRef(CharacterContext->m_Skill[i]->TexSkill.Get(), _File);
    }

    return 0;
}

CGameObject* ER_CharacterMgr::LoadCharacterData(FILE* _File)
{
    CGameObject* pCharacter = new CGameObject;
    pCharacter->AddComponent(new ER_DataScript_Character());

    ER_DataScript_Character* CharacterContext = pCharacter->GetScript<ER_DataScript_Character>();

    // Character Key
    LoadWString(CharacterContext->m_strKey, _File);

    // Character Name
    LoadWString(CharacterContext->m_strName, _File);

    // Character Std Stats
    fread(&CharacterContext->m_STDStats, sizeof(ER_Initial_Stats), 1, _File);

    // CharacterData init
    pCharacter->SetName(CharacterContext->m_strKey);
    CharacterContext->init();

    // MeshData Set
    wstring MeshDataKey = CharacterContext->m_strKey + L".mdat";
    Ptr<CMeshData> meshdata = CResMgr::GetInst()->FindRes<CMeshData>(MeshDataKey);
    
    if(meshdata != nullptr)
        meshdata->Instantiate(pCharacter);

    wstring ActionScript = L"ER_ActionScript_";
    ActionScript += CharacterContext->m_strKey;

    pCharacter->AddComponent(CScriptMgr::GetScript(ActionScript));

    int SkillSize = CharacterContext->m_Skill.size();
    fread(&SkillSize, sizeof(int), 1, _File);
    
    for (int i = 0; i < SkillSize; ++i)
    {
        ER_SKILL* Skill = new ER_SKILL;
        
        Skill->Load(_File);
        LoadResRef(Skill->TexSkill, _File);
    
        CharacterContext->m_Skill.push_back(Skill);
    }

    m_mapCharacters.insert(make_pair(CharacterContext->m_strKey, pCharacter));

    return pCharacter;
}