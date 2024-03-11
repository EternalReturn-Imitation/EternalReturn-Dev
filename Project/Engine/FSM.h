#pragma once

class CGameObject;
class CScript;

typedef void (CScript::* SCRIPT_DELEGATE)(void);

typedef

class FSMState
{
private:
	CScript* script;

	SCRIPT_DELEGATE m_EnterFunc;
	SCRIPT_DELEGATE m_UpdateFunc;
	SCRIPT_DELEGATE m_ExitFunc;

public:
	void OnStateEnter() { if (script && m_EnterFunc) (script->*m_EnterFunc)(); }
	void OnStateUpdate() { if (script && m_UpdateFunc) (script->*m_UpdateFunc)(); }
	void OnStateExit() { if (script && m_ExitFunc) (script->*m_ExitFunc)(); }

	FSMState(CScript* _script, SCRIPT_DELEGATE _enter, SCRIPT_DELEGATE _update, SCRIPT_DELEGATE _exit)
	{
		script = _script;
		m_EnterFunc = _enter;
		m_UpdateFunc = _update;
		m_ExitFunc = _exit;
	}
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

