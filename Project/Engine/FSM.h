#pragma once

class CGameObject;
class CScript;

typedef void (CScript::* SCRIPT_DELEGATE)(tFSMData);

class FSMState
{
private:
	tFSMData Data;

	CScript* script;

	SCRIPT_DELEGATE m_EnterFunc;
	SCRIPT_DELEGATE m_UpdateFunc;
	SCRIPT_DELEGATE m_ExitFunc;

public:
	void OnStateEnter()
	{
		if (script)
		{
			if (m_EnterFunc)
				(script->*m_EnterFunc)(Data);
		}
	}
	void OnStateUpdate()
	{
		if (script)
		{
			if (m_UpdateFunc)
				(script->*m_UpdateFunc)(Data);
		}
	}
	void OnStateExit()
	{
		if (script)
		{
			if (m_ExitFunc)
				(script->*m_ExitFunc)(Data);
		}
	}

	void SetStateEnter(SCRIPT_DELEGATE _delegate) { m_EnterFunc = _delegate; }
	void SetStateUpdate(SCRIPT_DELEGATE _delegate) { m_UpdateFunc = _delegate; }
	void SetStateExit(SCRIPT_DELEGATE _delegate) { m_ExitFunc = _delegate; }

	void SetData(const tFSMData& _data) { Data = _data; }
	const tFSMData& GetData() { return Data; }

	FSMState(CScript* _script) : script(_script)
		, Data{}
		, m_EnterFunc(nullptr)
		, m_UpdateFunc(nullptr)
		, m_ExitFunc(nullptr)
	{}
};

class FSM
{
public:
	FSM(FSMState* initState)
	{
		m_pCurState = initState;
	}

private:
	FSMState* m_pCurState;

public:
	void ChangeState(FSMState* _nextState)
	{
		if (_nextState == m_pCurState)
			return;

		if (m_pCurState != nullptr)
			m_pCurState->OnStateExit();

		m_pCurState = _nextState;
		m_pCurState->OnStateEnter();
	}

	void UpdateState()
	{
		if (m_pCurState != nullptr)
			m_pCurState->OnStateUpdate();
	}
};

