#pragma once
#include "CRoom.h"

class CRoomStage2 : public CRoom
{
public:
	explicit CRoomStage2(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRoomStage2();
public:
	HRESULT Ready_Room(CLayer* pLayer);
private:
	static const WallData s_Room2WallTable[];
	static const _int s_Room2WallCount; 
	LPDIRECT3DDEVICE9 m_pGraphicDev = nullptr;
};