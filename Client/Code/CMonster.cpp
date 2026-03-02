#include "pch.h"
#include "CMonster.h"
#include "CProjectile.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CMonster::CMonster(const CMonster& rhs)
	:CGameObject(rhs)
{
}

CMonster::~CMonster()
{
}

HRESULT CMonster::Ready_GameObject()
{
	//필요한 Component들을 추가
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CMonster::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	CTransform* pPlayerTransform = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Transform();
	_vec3 vPlayerPos;
	pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);

	//플레이어 쪽의 위치를 받아와서 플레이어를 따라다니게 만들기
	//_vec3 playerPos = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Pos();
	
	m_iFrameCounter++;
	if (m_iFrameCounter >= m_iMaxFrame)
	{
		printf("Reference Pos: %.2f, %.2f, %.2f\n",
			vPlayerPos.x, vPlayerPos.y, vPlayerPos.z);
		m_iFrameCounter = 0;
	}

	//플레이어를 기준으로 하는 방향 벡터 구하기
	//방향 초기화
	_vec3 vPos;
	m_pTransformComponent->Get_Info(INFO_POS, &vPos);
	_vec3 dir = D3DXVECTOR3(0.f, 1.f, 0.f);
	dir = vPlayerPos - vPos;
	float dist = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
	if (dist > 1.f)
	{
		//방향 벡터 정규화
		D3DXVec3Normalize(&dir, &dir);
		//구한 방향 기준으로 몬스터 이동
		vPos += dir * m_fSpeed * fTimeDelta;
	}

	m_fBulletTimer += fTimeDelta;
	if (m_fBulletTimer >= m_fBulletInterval)
	{
		FireBullet(dir);
		m_fBulletTimer = 0.f;
	}

	m_pTransformComponent->Set_Info(INFO_POS, vPos);
	m_pTransformComponent->Update_Component(fTimeDelta);

	//자동 회전
	m_pTriBufferComponent->Update_Component(fTimeDelta);

	//3D 회전각 계산
	//Yaw(Y축 회전)
	//m_vAngle.y = atan2f(dir.x, dir.z);
	////Pitch(X축 회전)
	//float fXZLength = sqrtf(dir.x * dir.x + dir.z * dir.z);
	//m_vAngle.x = atan2f(-dir.y, fXZLength);
	//Roll(Z축 회전)
	//m_vAngle.z = atan2f(m_vDir.x, m_vDir.y);

	//_matrix matScale, matRotX, matRotY, matRotZ, matTrans;

	//D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	////D3DXMatrixRotationX(&matRotX, m_vAngle.x);
	////D3DXMatrixRotationY(&matRotY, m_vAngle.y);
	////D3DXMatrixRotationZ(&matRotZ, m_vAngle.z);
	//D3DXMatrixTranslation(&matTrans, m_vPos.x, m_vPos.y, m_vPos.z);

	//m_matWorld = matScale * matRotZ * matTrans;

	return iExit;
}

void CMonster::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CMonster::Render_GameObject()
{
	//update에서 계산한 몬스터와 플레이어의 위치를 반영한 matworld로 삼각형의 렌더링을 한다
	m_pGraphicDev->SetTransform(D3DTS_WORLD,
		m_pTransformComponent->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTriBufferComponent->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CMonster::Add_Component()
{
	//CTriCol 추가
	Engine::CComponent* pComponent = nullptr;
	//RTriCol
	pComponent = m_pTriBufferComponent = static_cast<CTriCol*>(
		CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_TriCol"));
	if (nullptr == pComponent)
		return E_FAIL;
	//정적 컴포넌트 맵에 추가
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_TriCol", pComponent });

	//transform 컴포넌트 추가
	pComponent = m_pTransformComponent = static_cast<CTransform*>(
		CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Trans"));
	if (!pComponent)
		return E_FAIL;
	//정적 컴포넌트 게임 오브젝트인 CMonster의 맵에 추가
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Trans", pComponent });

	//Collider 컴포넌트 추가
	pComponent = m_pColliderCom = static_cast<CCollider*>(
	CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Collider"));
	if (!pComponent)
		return E_FAIL;
	//Collider 세팅
	m_pColliderCom->SetOwner(this);
	//임시 객체는 const로 받아야 함
	m_pColliderCom->SetSize(_vec3(1.f, 2.f, 1.f));
	m_pColliderCom->SetOffset(_vec3(0.f, 1.f, 0.f));
	CCollisionMgr::GetInstance()->AddCollider(m_pColliderCom);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pComponent });

	//CCollider* pCollider = CCollider::Create(m_pGraphicDev,
	//	_vec3(1.f, 2.f, 1.f), //크기
	//	_vec3(0.f, 1.f, 0.f)); //오프셋
	////Owner 설정
	//pCollider->SetOwner(this);
	//m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", pCollider });

	return S_OK;
}

void CMonster::FireBullet(const _vec3& vDir)
{
	//update에서 얻어온 방향으로 총알 발사
	CProjectile* pProjectile = new CProjectile(m_pGraphicDev);

}

CMonster* CMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonster* pMonster = new CMonster(pGraphicDev);
	if (FAILED(pMonster->Ready_GameObject()))
	{
		Safe_Release(pMonster);
		MSG_BOX("pMonster Create Failed");
		return nullptr;
	}
	return pMonster;
}

void CMonster::Free()
{
	CGameObject::Free();
}
