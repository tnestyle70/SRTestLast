#include "pch.h"
#include "CStage.h"
#include "CCameraMgr.h"
#include "CCollisionMgr.h"
#include "CRoomMgr.h"
#include "CEnemyMgr.h"
#include "CRoom.h"
#include "CRoomStage1.h"
#include "CRoomStage2.h"
#include "CWall.h"
#include "CTexture.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene()
{
	if (FAILED(Ready_Prototype()))
		return E_FAIL;
	if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
		return E_FAIL;
	if(FAILED(Ready_GameLogic_Layer(L"GameLogic_Layer")))
		return E_FAIL;
	if (FAILED(Ready_UI_Layer(L"UI_Layer")))
		return E_FAIL;
	if (FAILED(Ready_Camera()))
		return E_FAIL;

	return S_OK;
}

_int CStage::Update_Scene(const _float& fTimeDelta)
{
	_int iExit = CScene::Update_Scene(fTimeDelta);

	CCameraMgr::GetInstance()->Update(fTimeDelta);

	return iExit;
}

void CStage::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
}

void CStage::Render_Scene()
{
	CScene::Render_Scene();

	CCollisionMgr::GetInstance()->RenderDebug();
}

HRESULT CStage::Ready_Environment_Layer(const _tchar* pLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	if (!pLayer)
		return E_FAIL;

	//플레이어 추가
	CGameObject* pPlayer = CPlayer::Create(m_pGraphicDev);
	if (!pPlayer)
		return E_FAIL;
	if (FAILED(pLayer->Add_GameObject(L"Player", pPlayer)))
		return E_FAIL;
	m_pPlayer = dynamic_cast<CPlayer*>(pPlayer);

	//CRoomMgr 쪽에서 방들 쭉 초기화 - Scene을 넘겨줘서 초기화
	CRoomMgr::GetInstance()->Ready_RoomMgr(this);

	CRoom* pRoom = new CRoom(m_pGraphicDev);

	pRoom->SetPlayer(m_pPlayer);
	//pRoom->CreateRoomObjects(m_pGraphicDev, pLayer);

	//roomStage1 초기화
	CRoomStage1* pRoomStage1 = new CRoomStage1(m_pGraphicDev);
	pRoomStage1->Ready_Room(pLayer);
	
	//roomStage2 초기화
	CRoomStage2* pRoomStage2 = new CRoomStage2(m_pGraphicDev);
	pRoomStage2->Ready_Room(pLayer);

	//CEnemyMgr 초기화
	CEnemyMgr::GetInstance()->SetPlayer(m_pPlayer);

	//white suit 생성

	m_mapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_GameLogic_Layer(const _tchar* pLayerTag)
{
	return S_OK;
}

HRESULT CStage::Ready_Prototype()
{
	//CTriCol의 prototype 준비, prototype은 component를 반환
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TriCol", Engine::CTriCol::Create(
		m_pGraphicDev))))
		return E_FAIL;
	//CRcCol의 prototype 준비
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RcCol", Engine::CRcCol::Create(
		m_pGraphicDev))))
		return E_FAIL;
	//RcTex의 prototype 준비 - UVScale을 개별적으로 설정해주기 위해서 prototype을 사용하지 않기
	//if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RcTex", Engine::CRcTex::Create(
	//	m_pGraphicDev))))
	//	return E_FAIL;
	//CTransform의 prototype 준비
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Trans", Engine::CTransform::Create(
		m_pGraphicDev))))
		return E_FAIL;
	//CCollider의 prototype 준비
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Collider", Engine::CCollider::Create(
		m_pGraphicDev))))
		return E_FAIL;

	//Texture들의 prototype 준비
	//if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_FloorTex", Engine::CTexture::Create(
	//	m_pGraphicDev ,L"../../Image/Floor1.png"))))
	//	return E_FAIL;
	//if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CeilingTex", Engine::CTexture::Create(
	//	m_pGraphicDev,L"../../Image/Ceiling1.png"))))
	//	return E_FAIL;
	//if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_WallTex", Engine::CTexture::Create(
	//	m_pGraphicDev,L"../../Image/Wall1.png"))))
	//	return E_FAIL;

	if (FAILED(Ready_Texture()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage::Ready_Camera()
{
	//플레이어 transform을 카메라와 연결
	CTransform* pPlayerTransform = m_pPlayer->Get_Transform();
	if (!pPlayerTransform)
		return E_FAIL;
	//카메라 타겟 설정
	CCameraMgr::GetInstance()->Set_Target(pPlayerTransform);

	return S_OK;
}

HRESULT CStage::Ready_Texture()
{
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_WallTex",
		Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Wall1.png", 1))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_CeilingTex",
		Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Floor1.png", 1))))
		return E_FAIL;
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_FloorTex",
		Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Ceiling1.png", 1))))
		return E_FAIL;
	//총알 텍스쳐
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Bullet",
		Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Bullet_%d.png", 4))))
		return E_FAIL;
	//보스
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Boss",
		Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Boss_%d.png", 8))))
		return E_FAIL;
	//white suit monster
	if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_WhiteSuit",
		CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/WhiteSuit/WhiteSuit_%d.png", 5))))
		return E_FAIL;
	
	return S_OK;
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage* pStage = new CStage(pGraphicDev);
	if (FAILED(pStage->Ready_Scene()))
	{
		Safe_Release(pStage);
		MSG_BOX("Stage Create Failed");
		return nullptr;
	}
	return pStage;
}

void CStage::Free()
{
	CScene::Free();
}
