#include "pch.h"
#include "CLevelSaveLoad.h"

#include <Engine\CPathMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CScript.h>

#include <Script\CScriptMgr.h>
#include <Engine\CSQLMgr.h>

vector<int> CLevelSaveLoad::m_vLevelID;

int CLevelSaveLoad::SaveLevel(const wstring& _LevelPath, CLevel* _Level)
{
	if (_Level->GetState() != LEVEL_STATE::STOP)
		return E_FAIL;

	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	strPath += _LevelPath;

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, strPath.c_str(), L"wb");

	if (nullptr == pFile)	
		return E_FAIL;

	// ���� �̸� ����
	SaveWString(_Level->GetName(), pFile);


	// ������ ���̾���� ����
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = _Level->GetLayer(i);

		// ���̾� �̸� ����
		SaveWString(pLayer->GetName(), pFile);

		// ���̾��� ���ӿ�����Ʈ�� ����
		const vector<CGameObject*>& vecParent = pLayer->GetParentObject();

		// ������Ʈ ���� ����
		size_t objCount = vecParent.size();
		fwrite(&objCount, sizeof(size_t), 1, pFile);
		
		// �� ���ӿ�����Ʈ
		for (size_t i = 0; i < objCount; ++i)
		{
			SaveGameObject(vecParent[i], pFile);
		}
	}

	fclose(pFile);


	return S_OK;
}

int CLevelSaveLoad::SaveGameObject(CGameObject* _Object, FILE* _File)
{
	// �̸�
	SaveWString(_Object->GetName(), _File);
	
	// ������Ʈ
	for (UINT i = 0; i <= (UINT)COMPONENT_TYPE::END; ++i)
	{		
		if (i == (UINT)COMPONENT_TYPE::END)
		{
			// ������Ʈ Ÿ�� ����
			fwrite(&i, sizeof(UINT), 1, _File);
			break;
		}

		CComponent* Com = _Object->GetComponent((COMPONENT_TYPE)i);
		if (nullptr == Com)
			continue;

		// ������Ʈ Ÿ�� ����
		fwrite(&i, sizeof(UINT), 1, _File);

		// ������Ʈ ���� ����
		Com->SaveToLevelFile(_File);
	}

	// ��ũ��Ʈ	
	const vector<CScript*>& vecScript = _Object->GetScripts();
	size_t ScriptCount = vecScript.size();
	fwrite(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		wstring ScriptName = CScriptMgr::GetScriptName(vecScript[i]);
		SaveWString(ScriptName, _File);
		vecScript[i]->SaveToLevelFile(_File);
	}


	// �ڽ� ������Ʈ
	const vector<CGameObject*>& vecChild = _Object->GetChild();
	size_t ChildCount = vecChild.size();
	fwrite(&ChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ChildCount; ++i)
	{
		SaveGameObject(vecChild[i], _File);		
	}

	return 0;
}

int CLevelSaveLoad::SaveLevelToDB(CLevel* _Level)
{
	if (_Level->GetState() != LEVEL_STATE::STOP)
		return E_FAIL;

	//���� ������ ����
	CSQLMgr::GetInst()->DeleteAllRecordToAllTable();

	//���� ����
	int levelId = CSQLMgr::GetInst()->InsertToLevel(_Level->GetName());

	// ������ ���̾� ����
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = _Level->GetLayer(i);

		// ���̾� ����
		int layerId = CSQLMgr::GetInst()->InsertToLayer(levelId, pLayer->GetName(), i);

		// ���̾��� ���ӿ�����Ʈ�� ����
		const vector<CGameObject*>& vecParent = pLayer->GetParentObject();

		// �� ���ӿ�����Ʈ
		for (size_t i = 0; i < vecParent.size(); ++i)
		{
			SaveGameObjectToDB(layerId, vecParent[i], -1);
		}
	}

	return 0;
}

int CLevelSaveLoad::SaveGameObjectToDB(int _layerID, CGameObject* _Object, int _parentID)
{
	//���ӿ�����Ʈ ����
	int gameObjectId = CSQLMgr::GetInst()->InsertToGameObject(_layerID, _Object->GetName(), _parentID);

	vector<int> comType;
	// ������Ʈ
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		CComponent* Com = _Object->GetComponent((COMPONENT_TYPE)i);
		if (nullptr == Com)
			continue;

		// ������Ʈ ���� ����
		Com->SaveToDB(gameObjectId, (COMPONENT_TYPE)i);

		comType.push_back(i);
	}
	wstring wComType = IntArrayToWString(comType);
	CSQLMgr::GetInst()->UpdateToGameObject(gameObjectId, wComType);

	// ��ũ��Ʈ	
	const vector<CScript*>& vecScript = _Object->GetScripts();
	size_t ScriptCount = vecScript.size();

	vector<wstring> vScriptNames;
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		wstring ScriptName = CScriptMgr::GetScriptName(vecScript[i]);
		vScriptNames.push_back(ScriptName);
		
		vecScript[i]->SaveToDB(gameObjectId,(COMPONENT_TYPE)i);
	}

	if (vScriptNames.size() > 0) {
		string scriptSaveQuery = CSQLMgr::GetInst()->CreateInsertQuery(vScriptNames, gameObjectId);

		//��ũ��Ʈ ���� ����
		sqlite3* db = CSQLMgr::GetInst()->GetDB();

		char* errMsg = nullptr;
		if (sqlite3_exec(db, scriptSaveQuery.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
			sqlite3_free(errMsg);
			assert(false);
		}
	}

	// �ڽ� ������Ʈ
	const vector<CGameObject*>& vecChild = _Object->GetChild();
	size_t ChildCount = vecChild.size();

	for (size_t i = 0; i < ChildCount; ++i)
	{
		SaveGameObjectToDB(-1, vecChild[i], gameObjectId);
	}
	return 0;
}

CLevel* CLevelSaveLoad::LoadLevel(const wstring& _LevelPath)
{
	wstring strPath = CPathMgr::GetInst()->GetContentPath();
	strPath += _LevelPath;

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, strPath.c_str(), L"rb");

	if (nullptr == pFile)
		return nullptr;

	CLevel* NewLevel = new CLevel;

	// ���� �̸�
	wstring strLevelName;
	LoadWString(strLevelName, pFile);
	NewLevel->SetName(strLevelName);


	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = NewLevel->GetLayer(i);

		// ���̾� �̸�
		wstring LayerName;
		LoadWString(LayerName, pFile);
		pLayer->SetName(LayerName);

		// ���� ������Ʈ ����
		size_t objCount = 0;
		fread(&objCount, sizeof(size_t), 1, pFile);

		// �� ���ӿ�����Ʈ
		for (size_t j = 0; j < objCount; ++j)
		{
			CGameObject* pNewObj = LoadGameObject(pFile);
			NewLevel->AddGameObject(pNewObj, i, false);
		}
	}

	fclose(pFile);

	NewLevel->ChangeState(LEVEL_STATE::STOP);

	return NewLevel;
}

CGameObject* CLevelSaveLoad::LoadGameObject(FILE* _File)
{
	CGameObject* pObject = new CGameObject;

	// �̸�
	wstring Name;
	LoadWString(Name, _File);
	pObject->SetName(Name);

	// ������Ʈ
	while (true)
	{
		UINT ComponentType = 0;
		fread(&ComponentType, sizeof(UINT), 1, _File);

		// ������Ʈ ������ ���� Ȯ��
		if ((UINT)COMPONENT_TYPE::END == ComponentType)
			break;

		CComponent* Component = nullptr;

		switch ((COMPONENT_TYPE)ComponentType)
		{
		case COMPONENT_TYPE::TRANSFORM:
			Component = new CTransform;
			break;
		case COMPONENT_TYPE::COLLIDER2D:
			Component = new CCollider2D;
			break;
		case COMPONENT_TYPE::COLLIDER3D:
			//Component = new CCollider2D;
			break;
		case COMPONENT_TYPE::ANIMATOR2D:
			Component = new CAnimator2D;
			break;
		case COMPONENT_TYPE::ANIMATOR3D:
			break;
		case COMPONENT_TYPE::LIGHT2D:
			Component = new CLight2D;
			break;
		case COMPONENT_TYPE::LIGHT3D:
			Component = new CLight3D;
			break;
		case COMPONENT_TYPE::CAMERA:
			Component = new CCamera;
			break;
		case COMPONENT_TYPE::BEHAVIORTREE:
			Component = new CBehaviorTree;
			break;
		case COMPONENT_TYPE::MESHRENDER:
			Component = new CMeshRender;
			break;
		case COMPONENT_TYPE::PARTICLESYSTEM:
			Component = new CParticleSystem;
			break;
		case COMPONENT_TYPE::TILEMAP:
			Component = new CTileMap;
			break;
		case COMPONENT_TYPE::LANDSCAPE:
			Component = nullptr;
			break;
		case COMPONENT_TYPE::SKYBOX:
			Component = new CSkyBox;
			break;
		case COMPONENT_TYPE::DECAL:	
			Component = new CDecal;
			break;
		}

		Component->LoadFromLevelFile(_File);
		pObject->AddComponent(Component);
	}


	// ��ũ��Ʈ	
	size_t ScriptCount = 0;
	fread(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		wstring ScriptName;
		LoadWString(ScriptName, _File);
		CScript* pScript = CScriptMgr::GetScript(ScriptName);
		pObject->AddComponent(pScript);
		pScript->LoadFromLevelFile(_File);
	}

	// �ڽ� ������Ʈ		
	size_t ChildCount = 0;
	fread(&ChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ChildCount; ++i)
	{
		CGameObject* ChildObject = LoadGameObject(_File);
		pObject->AddChild(ChildObject);
	}

	return pObject;
}

CLevel* CLevelSaveLoad::LoadLevelByDB()
{
	m_vLevelID.clear();
	m_vLevelID.shrink_to_fit();

	sqlite3* db = CSQLMgr::GetInst()->GetDB();

	const char* szQuery = "SELECT ID FROM LEVEL";
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			int id = sqlite3_column_int(stmt, 0);
			m_vLevelID.push_back(id);
		}
		sqlite3_finalize(stmt);
	}
	else {
		assert(false);
	}

	CLevel* NewLevel = new CLevel;

	CSQLMgr::GetInst()->SelectFromLevel(m_vLevelID[0], NewLevel);

	LoadLayerByDB(m_vLevelID[0], NewLevel);

	NewLevel->ChangeState(LEVEL_STATE::STOP);

	return NewLevel;
}

void CLevelSaveLoad::LoadLayerByDB(int _levelID, CLevel*& _level)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();
	int levelID = _levelID;

	const char* szQuery = "SELECT ID, Layer_Name, Layer_Idx FROM Layer WHERE Level_ID = ?";
	sqlite3_stmt* stmt;

	// ���� �غ�
	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		sqlite3_bind_int(stmt, 1, levelID);

		while (sqlite3_step(stmt) == SQLITE_ROW) {
			int layer_ID = (sqlite3_column_int(stmt, 0));
			const wchar_t* layerName = reinterpret_cast<const wchar_t*>(sqlite3_column_text16(stmt, 1));
			int layer_Idx = (sqlite3_column_int(stmt, 2));

			CLayer* pLayer = _level->GetLayer(layer_Idx);
			pLayer->SetName(layerName);

			//���⼭ ���ӿ�����Ʈ�� �����ϰ� ���̾ �־���
			LoadGameObjectByDB(layer_ID, layer_Idx, _level);
		}

		sqlite3_finalize(stmt);
	}
	else {
		assert(false);
	}
}

void CLevelSaveLoad::LoadGameObjectByDB(int _layerID, int layer_Idx, CLevel*& _level)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();
	int layerID = _layerID;

	const char* szQuery = "SELECT ID, GameObject_Name, Parent_ID, Component_Type_Array FROM GAMEOBJECT WHERE Layer_ID = ?";
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		sqlite3_bind_int(stmt, 1, layerID);

		while (sqlite3_step(stmt) == SQLITE_ROW) {
			int gameObject_ID = (sqlite3_column_int(stmt, 0));
			const wchar_t* gameObjectName = reinterpret_cast<const wchar_t*>(sqlite3_column_text16(stmt, 1));
			int parentID = (sqlite3_column_int(stmt, 2));
			const wchar_t* comTypeArr = reinterpret_cast<const wchar_t*>(sqlite3_column_text16(stmt, 3));

			vector<int> vComTypeArr = WStringToIntArray(comTypeArr);

			CGameObject* pNewObject = LoadComponentByDB(gameObject_ID, gameObjectName, parentID, vComTypeArr);
			_level->AddGameObject(pNewObject, _layerID, false);
		}
		sqlite3_finalize(stmt);
	}
	else {
		assert(false);
	}
}

CGameObject* CLevelSaveLoad::LoadComponentByDB(int _gameObjectID, const wstring _gameObjectName, int _parentID, vector<int> _comTypeArr)
{
	sqlite3* db = CSQLMgr::GetInst()->GetDB();

	CGameObject* pNewGameObject = new CGameObject;
	pNewGameObject->SetName(_gameObjectName);

	for (int i = 0; i < _comTypeArr.size(); i++) {

		CComponent* Component = nullptr;

		switch ((COMPONENT_TYPE)_comTypeArr[i])
		{
		case COMPONENT_TYPE::TRANSFORM:
			Component = new CTransform;
			break;
		case COMPONENT_TYPE::COLLIDER2D:
			Component = new CCollider2D;
			break;
		case COMPONENT_TYPE::COLLIDER3D:
			//Component = new CCollider2D;
			break;
		case COMPONENT_TYPE::ANIMATOR2D:
			Component = new CAnimator2D;
			break;
		case COMPONENT_TYPE::ANIMATOR3D:
			break;
		case COMPONENT_TYPE::LIGHT2D:
			Component = new CLight2D;
			break;
		case COMPONENT_TYPE::LIGHT3D:
			Component = new CLight3D;
			break;
		case COMPONENT_TYPE::CAMERA:
			Component = new CCamera;
			break;
		case COMPONENT_TYPE::MESHRENDER:
			Component = new CMeshRender;
			break;
		case COMPONENT_TYPE::PARTICLESYSTEM:
			Component = new CParticleSystem;
			break;
		case COMPONENT_TYPE::TILEMAP:
			Component = new CTileMap;
			break;
		case COMPONENT_TYPE::LANDSCAPE:
			Component = nullptr;
			break;
		case COMPONENT_TYPE::SKYBOX:
			Component = new CSkyBox;
			break;
		case COMPONENT_TYPE::DECAL:
			Component = new CDecal;
			break;
		}

		Component->LoadFromDB(_gameObjectID);
		pNewGameObject->AddComponent(Component);
	}

	//��ũ��Ʈ �ε� �ڵ� ¥��
	const char* sSzQuery = "SELECT Script_Name FROM SCRIPT WHERE GameObject_ID = ?";
	sqlite3_stmt* sStmt;

	// ���� �غ�
	if (sqlite3_prepare_v2(db, sSzQuery, -1, &sStmt, NULL) == SQLITE_OK) {
		// Level_ID ���ε�
		sqlite3_bind_int(sStmt, 1, _gameObjectID);

		// ���� ���� �� ��� ó��
		while (sqlite3_step(sStmt) == SQLITE_ROW) {
			// ����� ����ϴ�.
			const char* scriptName = reinterpret_cast<const char*>(sqlite3_column_text(sStmt, 0));
			CScript* pScript = CScriptMgr::GetScript(ToWString(scriptName));
			pNewGameObject->AddComponent(pScript);
			pScript->LoadFromDB(_gameObjectID);
		}

		// ������Ʈ��Ʈ ����
		sqlite3_finalize(sStmt);
	}
	else {
		// ���� �غ� �������� ����� ó��
		assert(false);
	}


	//�ڽĿ�����Ʈ
	int parentID = _gameObjectID;

	// ���� ���ڿ� �غ�
	const char* szQuery = "SELECT ID, GameObject_Name, Component_Type_Array FROM GAMEOBJECT WHERE Parent_ID = ?";
	sqlite3_stmt* stmt;

	// ���� �غ�
	if (sqlite3_prepare_v2(db, szQuery, -1, &stmt, NULL) == SQLITE_OK) {
		// Level_ID ���ε�
		sqlite3_bind_int(stmt, 1, parentID);

		// ���� ���� �� ��� ó��
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			// ����� ����ϴ�.
			int gameObject_ID = (sqlite3_column_int(stmt, 0));
			const wchar_t* gameObjectName = reinterpret_cast<const wchar_t*>(sqlite3_column_text16(stmt, 1));
			const wchar_t* comTypeArr = reinterpret_cast<const wchar_t*>(sqlite3_column_text16(stmt, 2));

			vector<int> vComTypeArr = WStringToIntArray(comTypeArr);

			CGameObject* pChildObject = LoadComponentByDB(gameObject_ID, gameObjectName, parentID, vComTypeArr);
			pNewGameObject->AddChild(pChildObject);
		}

		// ������Ʈ��Ʈ ����
		sqlite3_finalize(stmt);
	}
	else {
		// ���� �غ� �������� ����� ó��
		assert(false);
	}


	return pNewGameObject;
}
