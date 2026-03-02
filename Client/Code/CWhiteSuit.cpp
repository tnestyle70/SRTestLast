#include "pch.h"
#include "CWhiteSuit.h"

CWhiteSuit::CWhiteSuit(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEnemy(pGraphicDev)
{
}

CWhiteSuit::~CWhiteSuit()
{
}

HRESULT CWhiteSuit::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CWhiteSuit::Update_GameObject(const _float& fTimeDelta)
{
	//플레이어 위치 찾아서 방향 + 위치 업데이트
	_int iExit = CEnemy::Update_GameObject(fTimeDelta);
	
	return iExit;
}

void CWhiteSuit::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CEnemy::LateUpdate_GameObject(fTimeDelta);
}

void CWhiteSuit::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	//라이팅 설정 끄기
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//텍스쳐 설정
	m_pTexture->Set_Texture(0);
	m_pRcTexture->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//CEnemy::Render_GameObject();
}

HRESULT CWhiteSuit::Add_Component()
{
	CComponent* pComponent = nullptr;

	//rctexture
	m_pRcTexture = CRcTex::Create(m_pGraphicDev, 1.f, 1.f);
	//pComponent = m_pRcTexture = static_cast<CRcTex*>(
	//	CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
	//if (!pComponent)
	//	return E_FAIL;
	//m_mapComponent[ID_STATIC].insert({ L"Com_RcTex", pComponent });

	//transform
	pComponent = m_pTransform = static_cast<CTransform*>(
		CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Trans"));
	if (!pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Trans", pComponent });

	//collider
	pComponent = m_pCollider = static_cast<CCollider*>(
		CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Collider"));
	if (!pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	//texture
	pComponent = m_pTexture = static_cast<CTexture*>(
		CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_WhiteSuit"));
	if (!pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

CWhiteSuit* CWhiteSuit::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWhiteSuit* pWhiteSuit = new CWhiteSuit(pGraphicDev);
	
	if (FAILED(pWhiteSuit->Ready_GameObject()))
	{
		Safe_Release(pWhiteSuit);
		MSG_BOX("pWhiteSuit Create Failed");
		return nullptr;
	}

	return pWhiteSuit;
}

void CWhiteSuit::Free()
{
	CEnemy::Free();
}
