#pragma once

class CLevel;
class CLayer;
class CGameObject;


class CLevelSaveLoad
{
public:
	static int SaveLevel(const wstring& _LevelPath, CLevel* _Level);
	static int SaveGameObject(CGameObject* _Object, FILE* _File);
	static int SaveLevelToDB(const wstring& _LevelPath, CLevel* _Level);

	static CLevel* LoadLevel(const wstring& _LevelPath);
	static CGameObject* LoadGameObject(FILE* _File);
};

