#include "pch.h"
#include "CRoom.h"
#include "CManagement.h"

CRoom::CRoom()
{
}

CRoom::CRoom(LPDIRECT3DDEVICE9 pGraphicDev)
{
}

CRoom::CRoom(const CRoom& rhs)
{
}

CRoom::~CRoom()
{
}

HRESULT CRoom::Ready_Room(_vec3& vCenterPos, eRoomType type)
{
	m_vCenterPos = vCenterPos;
	m_eRoomType = type;
	return S_OK;
}

_int CRoom::Update(const _float& fTimeDelta)
{
	//비활성화된 방은 업데이트하지 않음
	if (!m_bActive)
		return 0;
	//room은 wall만 업데이트하고, wall의 component인 transform을 업데이트해서 room들을 업데이트 시켜준다
	//적 업데이트
	for (auto& pEnemy : m_vecEnemies)
	{
		pEnemy->Update_GameObject(fTimeDelta);
	}
	//클리어 체크 
	if (!m_bCleared)
	{
		bool bAllDead = true;
		for (auto& pEnemy : m_vecEnemies)
		{
			if (!pEnemy->IsDead())
			{
				bAllDead = false;
			}
		}
		if (bAllDead)
			m_bCleared = true;
	}
	return 0;
}

void CRoom::LateUpdate(const _float& fTimeDelta)
{
	if (!m_bActive)
		return;
	//적 업데이트
	for (auto& pEnemy : m_vecEnemies)
	{
		pEnemy->LateUpdate_GameObject(fTimeDelta);
	}
}

void CRoom::Render()
{
	if (!m_bActive)
		return;
	//벽, 적 렌더링
	for (auto& pWall : m_vecWalls)
	{
		pWall->Render_GameObject();
	}
	for (auto& pEnemy : m_vecEnemies)
	{
		pEnemy->Render_GameObject();
	}
}

void CRoom::BuildRoom(LPDIRECT3DDEVICE9 pGraphicDev, CLayer* pLayer, const WallData* pTable, int iCnt)
{
	//CRoomStage1,2에서 호출해서 매개 변수로 받은 Layer에 WallData의 정보를 통해서 m_vecWalls에 추가해줌
	for (size_t i = 0; i < iCnt; ++i)
	{
		_tchar* szName = new _tchar[128];

		wsprintf(szName,L"Wall_%d", i);

		CGameObject* pWall = CWall::Create(pGraphicDev,
			pTable[i].pProtoName,
			pTable[i].vPos,
			pTable[i].vScale,
			pTable[i].vRot,
			pTable[i].fUScale,
			pTable[i].fVScale);
		pLayer->Add_GameObject(szName, pWall);
	}
}

void CRoom::CreateRoomObjects(LPDIRECT3DDEVICE9 pGraphicDev, CLayer* pLayer)
{
	_vec3 pos = {0.f, -5.f, -5.f};
	//몬스터 생성
	CMonster* pMonster = CMonster::Create(pGraphicDev);
	pMonster->Set_Player(m_pPlayer);
	Engine::CTransform* pMonsterTrans= pMonster->Get_Transform();
	pMonsterTrans->Set_Info(INFO_POS, pos);
	pLayer->Add_GameObject(L"Monster", pMonster);

	//바닥
	CGameObject* pFloor = CWall::Create(
		pGraphicDev, L"Proto_FloorTex",
		_vec3(0.f, -5.f, -5.f), //x-앞으로 감, y-밑으로 내려감, z-양옆으로이동
		_vec3(5.f, 5.f, 5.f),
		_vec3(0.f, 90.f, 90.f)
		,10.f, 10.f);
	pLayer->Add_GameObject(L"Floor", pFloor);
	
	//천장
	CGameObject* pCeiling = CWall::Create(
		pGraphicDev, L"Proto_CeilingTex",
		_vec3(0.f, 5.f, -5.f), //x-앞으로 감, y-밑으로 내려감, z-양옆으로이동
		_vec3(5.f, 5.f, 5.f),
		_vec3(0.f, 90.f, 90.f)
		, 5.f, 5.f);
	pLayer->Add_GameObject(L"Ceiling", pCeiling);

	//앞 벽
	CGameObject* pWallFront = CWall::Create(
		pGraphicDev, L"Proto_WallTex",
		_vec3(0.f, 0.f, 0.f),
		_vec3(5.f, 5.f, 5.f),
		_vec3(0.f, 0.f, 0.f)
		, 5.f, 5.f);
	pLayer->Add_GameObject(L"WallFront", pWallFront);

	//뒤 벽
	CGameObject* pWallBack = CWall::Create(
		pGraphicDev, L"Proto_WallTex",
		_vec3(0.f, 0.f, -10.f),
		_vec3(5.f, 5.f, 5.f),
		_vec3(0.f, 0.f, 0.f)
		, 5.f, 5.f);
	pLayer->Add_GameObject(L"WallBack", pWallBack);

	//왼쪽 벽
	CGameObject* pWallLeft = CWall::Create(
		pGraphicDev, L"Proto_WallTex",
		_vec3(5.f, 0.f, -5.f),
		_vec3(5.f, 5.f, 5.f),
		_vec3(0.f, 90.f, 0.f)
		, 5.f, 5.f);
	pLayer->Add_GameObject(L"WallLeft", pWallLeft);

	//오른쪽 벽
	CGameObject* pWallRight = CWall::Create(
		pGraphicDev, L"Proto_WallTex",
		_vec3(-5.f, 0.f, -5.f),
		_vec3(5.f, 5.f, 5.f),
		_vec3(0.f, 90.f, 0.f)
		, 5.f, 5.f);
	pLayer->Add_GameObject(L"WallRight", pWallRight);
}

CRoom* CRoom::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRoom* pRoom = new CRoom(pGraphicDev);

	//if (FAILED(pRoom->Ready_Room()))
	//{
	//	Safe_Release(pRoom);
	//	MSG_BOX("Room Create Failed");
	//	return nullptr;
	//}

	return pRoom;
}

void CRoom::Free()
{
	for (auto& pWall : m_vecWalls)
	{
		Safe_Release(pWall);
		//pWall->Release();
	}
	for (auto& pEnemy : m_vecEnemies)
	{
		Safe_Release(pEnemy);
	}
	Safe_Release(m_pGraphicDev);
}
