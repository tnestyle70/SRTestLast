#include "pch.h"
#include "CRoomMgr.h"

IMPLEMENT_SINGLETON(CRoomMgr)

CRoomMgr::CRoomMgr()
{
}

CRoomMgr::CRoomMgr(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
}

CRoomMgr::~CRoomMgr()
{
}

_int CRoomMgr::Update(const _float& fTimeDelta)
{
	//현재 room을 업데이트
	m_pCurrentRoom->Update(fTimeDelta);

	return 0;
}

void CRoomMgr::Late_Update(const _float& fTimeDelta)
{
	m_pCurrentRoom->LateUpdate(fTimeDelta);
}

void CRoomMgr::Render()
{
	m_pCurrentRoom->Render();
}

HRESULT CRoomMgr::Ready_RoomMgr(CScene* pScene)
{
	//Room 배치를 위한 5 X 5 사이즈의 그리드 생성
	m_vecRoomGrid.resize(5, vector<_int>(5, -1)); //-1은 빈 공간
	//Room 배치
	int roomID = 0;
	//시작 방 배치
	m_vecRoomGrid[2][2] = roomID;
	CreateRoom(eRoomType::ROOM_START, _vec3(0, 0, 0), roomID++);
	//전투1 방 배치
	m_vecRoomGrid[2][3] = roomID;
	CreateRoom(eRoomType::ROOM_COMBAT1, _vec3(20, 0, 0), roomID++);

	m_iCurrentRoomID = 0;
	m_pCurrentRoom = m_mapRooms[0];

	return S_OK;
}

void CRoomMgr::Change_Room()
{
	//현재 룸을 비활성화 시키고 ID++
	if (m_pCurrentRoom) //room 활성화 false
	{
		m_pCurrentRoom->SetRoomState(false);
	}
	//방 이동 후 해당 방 활성화 시키기
	m_iCurrentRoomID++;
	m_pCurrentRoom = m_mapRooms[m_iCurrentRoomID];
	m_pCurrentRoom->SetRoomState(true);
}

CRoom* CRoomMgr::CreateRoom(eRoomType type, _vec3 vCenterPos, _int roomID)
{
	CRoom* pRoom = CRoom::Create(m_pGraphicDev);
	pRoom->Ready_Room(vCenterPos, type);
	pRoom->SetRoomID(roomID);
	//roomID에 생성한 room을 대입시켜주기
	m_mapRooms[roomID] = pRoom;
	return pRoom;
}

void CRoomMgr::Free()
{
}
