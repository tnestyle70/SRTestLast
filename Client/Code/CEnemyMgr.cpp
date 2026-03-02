#include "pch.h"
#include "CEnemyMgr.h"

IMPLEMENT_SINGLETON(CEnemyMgr)

CEnemyMgr::CEnemyMgr()
{
}

CEnemyMgr::CEnemyMgr(LPDIRECT3DDEVICE9 pGraphicDev)
{
}

CEnemyMgr::~CEnemyMgr()
{
}

HRESULT CEnemyMgr::Ready_EnemyMgr()
{
	return E_NOTIMPL;
}

_int CEnemyMgr::Update(const _float& fTimeDelta)
{
	CTransform* pPlayerTrans = m_pPlayer->Get_Transform();
	
	for (auto& pEnemy : m_vecEnemies)
	{
		//플레이어의 Transform Injection
		pEnemy->SetPlayerTrans(pPlayerTrans);
		pEnemy->Update_GameObject(fTimeDelta);
	}

	return 0;
}

void CEnemyMgr::LateUpdate(const _float& fTimeDelta)
{
	for (auto& pEnemy : m_vecEnemies)
	{
		pEnemy->LateUpdate_GameObject(fTimeDelta);
	}
}

void CEnemyMgr::Render()
{
	for (auto& pEnemy : m_vecEnemies)
	{
		pEnemy->Render_GameObject();
	}
}

void CEnemyMgr::AddEnemy(CEnemy* pEnemy)
{
	if (FAILED(pEnemy->Ready_GameObject()))
	{
		MSG_BOX("Enemy Create Failed");
		return;
	}
	
	m_vecEnemies.push_back(pEnemy);
}

void CEnemyMgr::Free()
{
	for_each(m_vecEnemies.begin(), m_vecEnemies.end(), Safe_Release<CEnemy*>);
}
