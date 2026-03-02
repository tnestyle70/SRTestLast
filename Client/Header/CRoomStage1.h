#pragma once
#include "CRoom.h"

class CRoomStage1 : public CRoom
{
public:
	explicit CRoomStage1(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRoomStage1();
public:
	HRESULT Ready_Room(CLayer* pLayer);
private: //Room1의 벽 정보를 담고 있는 walltable 배열 선언
	//초기화 목록을 보고 컴파일러가 크기를 결정
	static const WallData s_Room1WallTable[];
	static const _int s_Room1WallCount;
	LPDIRECT3DDEVICE9 m_pGraphicDev = nullptr;
};