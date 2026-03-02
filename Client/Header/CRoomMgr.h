#pragma once
#include "CScene.h"
#include "CRoom.h"

class CRoomMgr
{
	DECLARE_SINGLETON(CRoomMgr)
private:
	explicit CRoomMgr();
	explicit CRoomMgr(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRoomMgr();
public: //Scene을 상속 받아서 room에서 생성한 오브젝트를 레이어 쪽에 넣어주기
	HRESULT Ready_RoomMgr(CScene* pScene);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Late_Update(const _float& fTimeDelta);
	virtual void Render();
private:
	void Change_Room();
private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	map<int, CRoom*> m_mapRooms; //RoomID->Room 객체
	CRoom* m_pCurrentRoom = nullptr;
	vector<vector<int>> m_vecRoomGrid; //roomID 저장용
	_int m_iCurrentRoomID = 0; //현재 활성화 상태의 RoomID
public:
	//Room 생성 factory
	CRoom* CreateRoom(eRoomType type, _vec3 vCenterPos, _int roomID);
private:
	virtual void Free();
};