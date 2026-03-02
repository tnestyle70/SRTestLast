#include "pch.h"
#include "CRoomStage1.h"

const CRoom::WallData CRoomStage1::s_Room1WallTable[] =
{
	{L"Proto_FloorTex", _vec3(0.f, -5.f, -5.f), _vec3(5.f, 5.f, 5.f), _vec3(0.f, 90.f, 90.f) , 10.f, 10.f},
	{L"Proto_CeilingTex", _vec3(0.f, 5.f, -5.f), _vec3(5.f, 5.f, 5.f), _vec3(0.f, 90.f, 90.f) , 5.f, 5.f},
	{L"Proto_WallTex", _vec3(0.f, 0.f, 0.f), _vec3(5.f, 5.f, 5.f), _vec3(0.f, 0.f, 0.f) , 5.f, 5.f},
	{L"Proto_WallTex", _vec3(0.f, 0.f, -10.f), _vec3(5.f, 5.f, 5.f), _vec3(0.f, 0.f, 0.f) , 5.f, 5.f},
	{L"Proto_WallTex", _vec3(5.f, 0.f, -5.f), _vec3(5.f, 5.f, 5.f), _vec3(0.f, 90.f, 0.f) , 5.f, 5.f},
	{L"Proto_WallTex", _vec3(-5.f, 0.f, -5.f), _vec3(5.f, 5.f, 5.f), _vec3(0.f, 90.f, 0.f) , 5.f, 5.f}
};

const _int CRoomStage1::s_Room1WallCount = sizeof(s_Room1WallTable) / sizeof(s_Room1WallTable[0]);

CRoomStage1::CRoomStage1(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CRoomStage1::~CRoomStage1()
{
}

HRESULT CRoomStage1::Ready_Room(CLayer* pLayer)
{
	BuildRoom(m_pGraphicDev, pLayer, s_Room1WallTable, s_Room1WallCount);

	return S_OK;
}
