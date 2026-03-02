#pragma once
#include "CBase.h"
#include "CEnemy.h"
#include "CPlayer.h"
//enemy들을 관리하는 mgr의 역할을 수행
 
class CEnemyMgr : public CBase
{
	DECLARE_SINGLETON(CEnemyMgr)
public:
	explicit CEnemyMgr();
	explicit CEnemyMgr(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEnemyMgr();
public:
	virtual HRESULT Ready_EnemyMgr();
	virtual _int Update(const _float& fTimeDelta);
	virtual void LateUpdate(const _float& fTimeDelta);
	virtual void Render();
public:
	void AddEnemy(CEnemy* pEnemy);
	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
private:
	CPlayer* m_pPlayer;
	vector<CEnemy*> m_vecEnemies; //방 내부에 배치된 적들의 상태
private:
	virtual void Free();
};
