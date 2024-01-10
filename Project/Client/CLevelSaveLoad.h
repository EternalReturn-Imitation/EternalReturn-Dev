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
	static void LoadLayerByDB(int _levelID, CLevel*& _level);
	static void LoadGameObjectByDB(int _layerID, int layer_Idx, CLevel*& _level);
	static CGameObject* LoadComponentByDB(int _gameObjectID, const wstring _gameObjectName, int _parentID, vector<int> _comTypeArr);

private:
	static vector<int> m_vLevelID;
};

