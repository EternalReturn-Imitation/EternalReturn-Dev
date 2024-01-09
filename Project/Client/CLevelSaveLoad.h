#pragma once

class CLevel;
class CLayer;
class CGameObject;


class CLevelSaveLoad
{
public:
	static int SaveLevel(const wstring& _LevelPath, CLevel* _Level);
	static int SaveGameObject(CGameObject* _Object, FILE* _File);
	static int SaveLevelToDB(CLevel* _Level);
	static int SaveGameObjectToDB(int _layerID, CGameObject* _Object, int _parentID);

	static CLevel* LoadLevel(const wstring& _LevelPath);
	static CGameObject* LoadGameObject(FILE* _File);
	static CLevel* LoadLevelByDB();
	static CGameObject* LoadGameObjectByDB(int _layerID, int _parentID);

private:
	static vector<int> m_vLevelID;
};

