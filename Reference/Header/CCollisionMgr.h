#pragma once
#include "CCollider.h"

class ENGINE_DLL CCollisionMgr
{
	DECLARE_SINGLETON(CCollisionMgr);
private:
	explicit CCollisionMgr();
	virtual ~CCollisionMgr();
public:
	void CheckCollision();
	void RenderDebug();
public:
	void AddCollider(CCollider* pCollider) {
		m_vecColliders.push_back(pCollider);
	}
private:
	vector<CCollider*> m_vecColliders;
private:
	virtual void Free();
};
	