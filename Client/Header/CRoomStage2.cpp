#include "pch.h"
#include "CRoomStage2.h"

const CRoom::WallData CRoomStage2::s_Room2WallTable[] =
{
	{L"Proto_FloorTex", _vec3(10.f, 5.f, 5.f), _vec3(5.f, 5.f, 5.f), _vec3(0.f, 90.f, 90.f) , 10.f, 10.f},
	{L"Proto_CeilingTex", _vec3(10.f, 15.f, 5.f), _vec3(5.f, 5.f, 5.f), _vec3(0.f, 90.f, 90.f) , 5.f, 5.f},
	{L"Proto_WallTex", _vec3(10.f, 10.f, 10.f), _vec3(5.f, 5.f, 5.f), _vec3(0.f, 0.f, 0.f) , 5.f, 5.f},
	{L"Proto_WallTex", _vec3(10.f, 10.f, 0.f), _vec3(5.f, 5.f, 5.f), _vec3(0.f, 0.f, 0.f) , 5.f, 5.f},
	{L"Proto_WallTex", _vec3(15.f, 10.f, 5.f), _vec3(5.f, 5.f, 5.f), _vec3(0.f, 90.f, 0.f) , 5.f, 5.f},
	{L"Proto_WallTex", _vec3(5.f, 10.f, 5.f), _vec3(5.f, 5.f, 5.f), _vec3(0.f, 90.f, 0.f) , 5.f, 5.f}
};

const _int CRoomStage2::s_Room2WallCount = sizeof(s_Room2WallTable) / sizeof(s_Room2WallTable[0]);

CRoomStage2::CRoomStage2(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}
CRoomStage2::~CRoomStage2()
{
}

HRESULT CRoomStage2::Ready_Room(CLayer* pLayer)
{
	BuildRoom(m_pGraphicDev, pLayer, s_Room2WallTable, s_Room2WallCount);

	return S_OK;
}
