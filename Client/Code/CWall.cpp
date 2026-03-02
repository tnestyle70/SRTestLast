#include "pch.h"
#include "CWall.h"

CWall::CWall(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CWall::CWall(const CWall& rhs)
	:CGameObject(rhs)
{	
}

CWall::~CWall()
{
}

HRESULT CWall::Ready_GameObject(const _tchar* pProtoTexKey, _float fUScale, _float fVScale)
{
	if (FAILED(Add_Component(pProtoTexKey, fUScale, fVScale)))
		return E_FAIL;
	//서로 다른 U, V Scale 때문에 Create를 다르게 한다
	m_pRectCom = CRcTex::Create(m_pGraphicDev, fUScale, fVScale);

	//m_pTexture = dynamic_cast<CTexture*>(
	//	CProtoMgr::GetInstance()->Clone_Prototype(pProtoTexKey));
	//m_pTexture = CTexture::Create(m_pGraphicDev, pPath);

	return S_OK;
}

_int CWall::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	//m_pTransCom->Update_Component(fTimeDelta);

	return iExit;
}

void CWall::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CWall::Render_GameObject()
{
	m_pGraphicDev->SetTransform(
		D3DTS_WORLD, m_pTransCom->Get_World());
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	//텍스쳐 반복 래핑 설정
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	//CRcCol 컴포넌트 클래스를 사용해서 사각형 Buffer를 출력
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//텍스쳐 설정
	m_pTexture->Set_Texture(0);
	m_pRectCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CWall::Add_Component(const _tchar* pProtoTexKey, _float fUScale, _float fVScale)
{
	//Wall을 출력할 사각형 컴포넌트
	Engine::CComponent* pComponent = nullptr;

	//CRcTex - Component
	//pComponent = m_pRectCom = dynamic_cast<CRcTex*>(
	//	CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
	//if (nullptr == pComponent)
	//	return E_FAIL;
	////정적 컴포넌트에 Proto_RcCol을 insert해주기
	//m_mapComponent[ID_STATIC].insert({ L"Proto_RcTex", pComponent });

	//Transform 컴포넌트 추가
	pComponent = m_pTransCom = static_cast<CTransform*>(
		CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Trans"));
	if (nullptr == pComponent)
		return E_FAIL;
	//정적 컴포넌트에 Proto_Transform을 insert해주기
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Trans", pComponent });

	//매개변수로 받은 protokey를 사용해서 clone을 하고 map component 쪽에 insert를 해준다
	pComponent = m_pTexture = static_cast<CTexture*>(
		CProtoMgr::GetInstance()->Clone_Prototype(pProtoTexKey));
	if (!pComponent)
		return E_FAIL;
	//m_pTexture = CTexture::Create(m_pGraphicDev, L"../Image/Floor1.png");
	//Component를 insert를 할 경우에는 Com_Texture이런 식으로 component로서 추가를 해야 함
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent});

	pComponent = m_pCollider = static_cast<CCollider*>(
		CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Collider"));
	m_pCollider->SetOwner(this);
	m_pCollider->SetSize(_vec3(10.f, 10.f, 10.f));
	m_pCollider->SetOffset(_vec3(0.f, 1.f, 0.f));
	CCollisionMgr::GetInstance()->AddCollider(m_pCollider);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	//Collider 설정
	//CCollider* pCollider = CCollider::Create(m_pGraphicDev,
	//	_vec3(10.f, 10.f, 10.f),
	//	_vec3(0.f, 1.f, 0.f));
	//m_mapComponent[ID_STATIC].insert({ L"Com_Collider", pCollider });

	return S_OK;
}

CWall* CWall::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pProtoTexKey,
	_vec3 vPos, _vec3 vScale, _vec3 vAngle,
	_float fUScale, _float fVScale)
{
	CWall* pWall = new CWall(pGraphicDev);

	if (FAILED(pWall->Ready_GameObject(pProtoTexKey, fUScale, fVScale)))
	{
		Safe_Release(pWall);
		MSG_BOX("pWall Create Failed");
		return nullptr;
	}

	pWall->m_pTransCom->Set_Info(INFO_POS, vPos);
	pWall->m_pTransCom->Set_Scale(vScale);
	pWall->m_pTransCom->Set_Angle(vAngle);

	pWall->m_pTransCom->Update_Component(0.f);

	return pWall;
}

void CWall::Free()
{
	CGameObject::Free();
}
