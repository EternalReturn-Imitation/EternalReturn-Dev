#pragma once
#include "CSingleton.h"

enum DebugLogFlags_
{
    // Event types

    DebugLogFlags_None = 0,
    DebugLogFlags_Input = 1 << 0,         // 입력 로그 : 마우스, 키보드 입력 시
    DebugLogFlags_Collision = 1 << 1,     // 충돌 로그 : 객체 충돌 판정 ( 충돌 시작, 충돌 종료 )
    DebugLogFlags_Command = 1 << 2,       // 명령 로그 : 동작 실행 ( 이동, 아이템 조합, 스킬 사용 등 )
    DebugLogFlags_Process = 1 << 3,       // 진행 로그 : 함수 동작 과정 ( 애니메이션 재생, 포인터 등록/해제 등 )
    DebugLogFlags_Info = 1 << 4,          // 정보 로그 : 게임 정보 변화/출력 내용 ( 데미지 발생, 스탯 증가/감소 등 )
    DebugLogFlags_All = DebugLogFlags_Input | DebugLogFlags_Collision | DebugLogFlags_Command | DebugLogFlags_Process | DebugLogFlags_Info
};

class DebugContext
    : public CSingleton<DebugContext>
{
    SINGLE(DebugContext);
private:
    DebugLogFlags_  DebugLogFlags;
    vector<string>  DebugLogBuf;

public:
    const vector<string>& GetDebugLogs() { return DebugLogBuf; }

    void AddLog(const char* _log);
    void ClearLog() { if (!DebugLogBuf.empty()) DebugLogBuf.clear(); }

    DebugLogFlags_* GetDebugFlags() { return &DebugLogFlags; }
    void SetDebugFlags(DebugLogFlags_ _flags) { DebugLogFlags = _flags; }
};

void DebugLog(const char* type, const char* obj, const char* func, const char* ex);

#define DEBUG_LOG(type,obj,func,ex)      DebugLog(type,obj,func,ex)
#define DEBUG_LOG_INPUT(obj,func,ex)     do { if (*(DebugContext::GetInst()->GetDebugFlags()) & DebugLogFlags_Input) DEBUG_LOG("Input",obj,func,ex); } while (0)
#define DEBUG_LOG_COLLISION(obj,func,ex) do { if (*(DebugContext::GetInst()->GetDebugFlags()) & DebugLogFlags_Collision)    DEBUG_LOG("Collision",obj,func,ex); } while (0)
#define DEBUG_LOG_COMMAND(obj,func,ex)   do { if (*(DebugContext::GetInst()->GetDebugFlags()) & DebugLogFlags_Command)    DEBUG_LOG("Command",obj,func,ex); } while (0)
#define DEBUG_LOG_PROCESS(obj,func,ex)   do { if (*(DebugContext::GetInst()->GetDebugFlags()) & DebugLogFlags_Process)      DEBUG_LOG("Process",obj,func,ex); } while (0)
#define DEBUG_LOG_INFO(obj,func,ex)      do { if (*(DebugContext::GetInst()->GetDebugFlags()) & DebugLogFlags_Info)  DEBUG_LOG("Info",obj,func,ex); } while (0)