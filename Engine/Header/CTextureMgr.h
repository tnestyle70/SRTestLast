#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CTextureMgr : public CBase
{
	DECLARE_SINGLETON(CTextureMgr)
private:
	explicit CTextureMgr();
	virtual ~CTextureMgr();
private:
	virtual void Free();
};

END	