#pragma once

#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)
//engine dll을 클래스 이름 앞에 붙이고 전처리기 시점에 ENGINE_DLL 붙은 경우에는
//따로 처리를 하도록 한다. (어떻게??)
class ENGINE_DLL CInputMgr : public CBase
{
	DECLARE_SINGLETON(CInputMgr)
private:
	explicit CInputMgr();
	virtual ~CInputMgr();
public:
	void Initialiize();
	void Update();
public:
	bool KeyPress(int vkCode) const;
	bool KeyDown(int vkCode) const;
	bool KeyUp(int vkCode) const;
private:
	bool m_bPrevKey[256];
	bool m_bCurrentKey[256];
private:
	virtual void Free();
};

END