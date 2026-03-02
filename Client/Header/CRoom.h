#pragma once
#include "CLayer.h"
#include "CProtoMgr.h"
#include "CEnemy.h"
#include "CWall.h"
#include "CMonster.h"

enum class eRoomType
{
	ROOM_START, ROOM_COMBAT1, ROOM_COMBAT2, ROOM_COMBAT3,
	ROOM_SNIPING, ROOM_BOSS ,ROOM_END
};

class CRoom
{
public:
	explicit CRoom();
	explicit CRoom(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRoom(const CRoom& rhs);
	virtual ~CRoom(); //부모쪽 virtual 가상함수 자식 소멸자 -> 부모 소멸자
public:
	HRESULT Ready_Room(_vec3& vCenterPos, eRoomType type);
	_int Update(const _float& fTimeDelta);
	void LateUpdate(const _float& fTimeDelta);
	void Render();
private: //Room에서 필요한 벽들을 가지고 있음
	CMonster* m_pMonster = nullptr;
	vector<CWall*> m_vecWalls;
	vector<CEnemy*> m_vecEnemies;
	_vec3 m_vOffset; //방 별 오프셋
	_vec3 m_vCenterPos; //방의 중심 위치, 중심을 기준으로 오프셋을 줘서 배치
	eRoomType m_eRoomType; //방 타입
	bool m_bActive = false; //활성화 여부
	bool m_bCleared = false; //클리어 여부
	_int m_iRoomID = 0;
	CPlayer* m_pPlayer = nullptr;
protected:
	struct WallData
	{
		const _tchar* pProtoName;
		_vec3 vPos;
		_vec3 vScale;
		_vec3 vRot;
		float fUScale;
		float fVScale;
	};
	void BuildRoom(LPDIRECT3DDEVICE9 pGraphicDev, CLayer* pLayer, const WallData* pTable, int iCnt = 1);
	LPDIRECT3DDEVICE9 m_pGraphicDev;
public:
	void CreateRoomObjects(LPDIRECT3DDEVICE9 pGraphicDev, CLayer* pLayer);
	void SetRoomID(_int RoomID) { m_iRoomID = RoomID; }
	void SetRoomState(bool active) { m_bActive = active; }
	bool IsCleared() { return m_bCleared; }
	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
public:
	static CRoom* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free();
};