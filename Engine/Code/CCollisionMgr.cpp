#include "CCollisionMgr.h"

IMPLEMENT_SINGLETON(CCollisionMgr)

CCollisionMgr::CCollisionMgr()
{
}

CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::CheckCollision()
{
}

void CCollisionMgr::RenderDebug()
{
	for (auto& pCollider : m_vecColliders)
	{
		pCollider->DebugRender();
	}
}

void CCollisionMgr::Free()
{
}

