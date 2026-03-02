#include "pch.h"
#include "CProjectile.h"
#include "CCollisionMgr.h"
#include "CTimerMgr.h"

CProjectile::CProjectile(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CProjectile::CProjectile(const CProjectile& rhs)
	: CGameObject(rhs)
{
}

CProjectile::~CProjectile()
{
}

HRESULT CProjectile::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CProjectile::Update_GameObject()
{
	if (m_bDead)
		return 1;
	float dt = CTimerMgr::GetInstance()->Get_TimeDelta(L"Timer_FPS60");
	//지정된 방향으로 이동

	

	return _int();
}

void CProjectile::LateUpdate_GameObject()
{
}

void CProjectile::Render_GameObject()
{
}

HRESULT CProjectile::Add_Component()
{
	//transform, collider, buffer 연결
	Engine::CComponent* pComponent = nullptr;
	//Transform
	pComponent = m_pTrans = static_cast<CTransform*>(
		CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Trans"));
	if (!pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Trans", pComponent });
	//Collider
	pComponent = m_pCollider = static_cast<CCollider*>(
		CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Collider"));
	if (!pComponent)
		return E_FAIL;
	//Collider 세팅해주기
	m_pCollider->SetOwner(this);
	//임시 객체는 const로 받아야 함
	m_pCollider->SetSize(_vec3(1.f, 2.f, 1.f));
	m_pCollider->SetOffset(_vec3(0.f, 1.f, 0.f));
	CCollisionMgr::GetInstance()->AddCollider(m_pCollider);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });
	
	return S_OK;
}

void CProjectile::Fire(const _vec3& vPos, const _vec3& vDir)
{
}

void CProjectile::Free()
{

}
