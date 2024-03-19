#pragma once
#include <Engine\CSingleton.h>

class ER_UIMgr
	: public CSingleton<ER_UIMgr>
{
	SINGLE(ER_UIMgr);
private:


public:
	void init();
	void progress();
	void tick();
};

