#include "pch.h"
#include "CBackGround.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CBackGround::CBackGround(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

CBackGround::~CBackGround()
{
}

HRESULT CBackGround::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CBackGround::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CBackGround::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CBackGround::Render_GameObject()
{
	m_pBufferCom->Render_Buffer();
}

HRESULT CBackGround::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	// TriCol
	//pComponent = m_pBufferCom = dynamic_cast<CTriCol*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_TriCol"));

	//RcCol
	pComponent = m_pBufferCom = static_cast<CRcCol*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcCol"));

	if (nullptr == pComponent)
		return E_FAIL;
	
	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	return S_OK;
}

CBackGround* CBackGround::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBackGround* pBackGround = new CBackGround(pGraphicDev);

	if (FAILED(pBackGround->Ready_GameObject()))
	{
		Safe_Release(pBackGround);
		MSG_BOX("pBackGround Create Failed");
		return nullptr;
	}

	return pBackGround;
}

void CBackGround::Free()
{
	CGameObject::Free();
}
