#pragma once
#include <Engine\CSingleton.h>

class ER_GameSystem
	: public CSingleton<ER_GameSystem>
{
	SINGLE(ER_GameSystem);
private:

public:
	void init();
	void progress();
	void tick();
	void render();
};

