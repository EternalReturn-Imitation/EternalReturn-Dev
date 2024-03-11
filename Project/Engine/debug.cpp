#include "pch.h"
#include "debug.h"

DebugContext::DebugContext()
	: DebugLogFlags(DebugLogFlags_None)
{
}

DebugContext::~DebugContext()
{
}

void DebugContext::AddLog(const char* _log)
{
	DebugLogBuf.push_back(_log);
}

void DebugLog(const char* type, const char* obj, const char* func, const char* ex)
{
	DebugContext* dbg = DebugContext::GetInst();
	assert(dbg);

	char tmp[256] = {};
	sprintf_s(tmp, "[%s] %s::%s(). %s", type, obj, func, ex);

	dbg->AddLog(tmp);
}