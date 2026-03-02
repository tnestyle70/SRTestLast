#include "pch.h"
#include "CPlayer.h"
#include "CInputMgr.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CPlayer::CPlayer(const CPlayer& rhs)
	:CGameObject(rhs)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	// ===== 플레이어 위치 (방 안쪽) =====
	
	_vec3 vPos, vScale;
	vPos = { 0.f, 2.f, -5.f };
	vScale = { 1.f, 1.f, 1.f };
	m_pTransCom->Set_Info(INFO_POS, vPos);
	m_pTransCom->Set_Scale(vScale);
	m_pTransCom->Update_Component(0.f);

	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	//_vec3 vDir(0.f, 0.f, 0.f);

	//if (CInputMgr::GetInstance()->KeyPress('W'))
	//	vDir.x -= 1.f;
	//if (CInputMgr::GetInstance()->KeyPress('S'))
	//	vDir.x += 1.f;
	//if (CInputMgr::GetInstance()->KeyPress('A'))
	//	vDir.y -= 1.f;
	//if (CInputMgr::GetInstance()->KeyPress('D'))
	//	vDir.y += 1.f;
	//if (CInputMgr::GetInstance()->KeyPress('Q'))
	//	vDir.z -= 1.f;
	//if (CInputMgr::GetInstance()->KeyPress('E'))
	//	vDir.z += 1.f;
	
	//_vec3 vCamDir;
	//vCamDir = CCameraMgr::GetInstance()->GetLookVector();
	//D3DXVec3Normalize(&vCamDir, &vCamDir);
	//D3DXVec3Normalize(&vDir, &vDir);
	//vDir = vDir + vCamDir;
	//m_pTransCom->Move_Pos(D3DXVec3Normalize(&vDir, &vDir), 10.f, fTimeDelta);

	//m_pTransCom->Update_Component(fTimeDelta);
	
	//FPS 게임에 앞뒤 양옆 이동을 구현해야 하기 때문에 look, right만 사용 x, z축
	//y up축은 날기용으로 사용
	_vec3 vLook, vRight;
	m_pTransCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransCom->Get_Info(INFO_RIGHT, &vRight);
	_vec3 vCamRight, vCamLook;
	vCamRight = CCameraMgr::GetInstance()->GetRightVector();
	vCamLook = CCameraMgr::GetInstance()->GetLookVector();
	D3DXVec3Normalize(&vCamRight, &vCamRight);
	D3DXVec3Normalize(&vCamLook, &vCamLook);
	if (CInputMgr::GetInstance()->KeyPress('A')) //왼쪽
		m_pTransCom->Move_Pos(&vCamRight, -10.f, fTimeDelta);
	if (CInputMgr::GetInstance()->KeyPress('D')) //오른쪽
		m_pTransCom->Move_Pos(&vCamRight, 10.f, fTimeDelta);
	if (CInputMgr::GetInstance()->KeyPress('W')) //앞
		m_pTransCom->Move_Pos(&vCamLook, 10.f, fTimeDelta);
	if (CInputMgr::GetInstance()->KeyPress('S')) //뒤
		m_pTransCom->Move_Pos(&vCamLook, -10.f, fTimeDelta);

	m_pTransCom->Update_Component(fTimeDelta);

	//m_iFrameCounter++;
	//if (m_iFrameCounter >= m_iMaxFrame)
	//{
	//	printf("Real Pos: %.2f, %.2f, %.2f\n",
	//		pos.x, pos.y, pos.z);
	//	m_iFrameCounter = 0;
	//}

	return iExit;
}

void CPlayer::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CPlayer::Render_GameObject()
{
	m_pGraphicDev->SetTransform(
		D3DTS_WORLD, m_pTransCom->Get_World());
	
	//CRcCol 컴포넌트 클래스를 사용해서 사각형 Buffer를 출력
	m_pRectBufferComponent->Render_Buffer();
}

HRESULT CPlayer::Add_Component()
{
	//CPlayer를 출력할 사각형 컴포넌트를 만들어낸다
	Engine::CComponent* pComponent = nullptr;
	//CRcCol - Component
	pComponent = m_pRectBufferComponent = static_cast<CRcCol*>(
		CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcCol"));
	if (nullptr == pComponent)
		return E_FAIL;
	//정적 컴포넌트에 Proto_RcCol을 insert해주기
	m_mapComponent[ID_STATIC].insert({ L"Com_RcCol", pComponent });

	//Transform 컴포넌트 추가
	pComponent = m_pTransCom = static_cast<CTransform*>(
		CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Trans"));
	if (nullptr == pComponent)
		return E_FAIL;
	//정적 컴포넌트에 Proto_Transform을 insert해주기
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Trans", pComponent });

	//Collider 컴포넌트 추가
	pComponent = m_pCollider = static_cast<CCollider*>(
		CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Collider"));
	if (!pComponent)
		return E_FAIL;
	m_pCollider->SetOwner(this);
	m_pCollider->SetSize(_vec3(1.f, 2.f, 1.f));
	m_pCollider->SetOffset(_vec3(0.f, 1.f, 0.f));
	CCollisionMgr::GetInstance()->AddCollider(m_pCollider);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });
	//Collider 컴포넌트 추가
	//CCollider* pCollider = CCollider::Create(m_pGraphicDev,
	//	_vec3(1.f, 2.f, 1.f), //크기
	//	_vec3(0.f, 1.f, 0.f)); //오프셋
	////정적 컴포넌트에 추가
	//m_mapComponent[ID_STATIC].insert({ L"Com_Collider", pCollider });

	return S_OK;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	//플레이어 복사 생성, 몬스터나 벽, 바닥 같은 객체들 같은 경우에는 복사 생성이 반드시 필요
	CPlayer* pPlayer = new CPlayer(pGraphicDev);

	if (FAILED(pPlayer->Ready_GameObject()))
	{
		Safe_Release(pPlayer);
		MSG_BOX("pPlayer Create Failed");
		return nullptr;
	}

	return pPlayer;
}

void CPlayer::Free()
{
	CGameObject::Free();
}
