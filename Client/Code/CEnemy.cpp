#include "pch.h"
#include "CEnemy.h"

CEnemy::CEnemy(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CEnemy::~CEnemy()
{
}

HRESULT CEnemy::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CEnemy::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);
	//enemymgr에서 세팅해준 playerpos 사용
	_vec3 vPlayerPos; 
	m_pPlayerTrans->Get_Info(INFO_POS, &vPlayerPos);

	m_pTransform->Chase_Target(&vPlayerPos, 5.f, fTimeDelta);

	return iExit;
}

void CEnemy::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CEnemy::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD,
		m_pTransform->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CEnemy::Free()
{
	CGameObject::Free();
}
