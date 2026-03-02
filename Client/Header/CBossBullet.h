#pragma once
#include "CProjectile.h"

class CBossBullet : public CProjectile
{
private: //여기에서 다시 graphicDev를 설정할 이유가 있나? 
	explicit CBossBullet();
	virtual ~CBossBullet();
private:
	virtual void Free();
};