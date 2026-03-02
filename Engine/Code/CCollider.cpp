#include "CCollider.h"
#include "CCameraMgr.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)	
{
}

CCollider::CCollider(const CCollider& rhs)
	: CComponent(rhs)
{
}

CCollider::~CCollider()
{
}

HRESULT CCollider::Ready_Collider()
{
	return S_OK;
}

_int CCollider::Update_Component(const _float& fTimeDelta)
{
	//업데이트는 따로 갱신하지 않음?
	return 0;
}

void CCollider::LateUpdate_Component()
{
	DebugRender();
}

void CCollider::DebugRender()
{
	_vec3 vCenter = GetWorldCenter();
	_vec3 vHalf = m_vSize * 0.5f;

	printf("Center: (%.2f, %.2f, %.2f)\n", vCenter.x, vCenter.y, vCenter.z);
	printf("Half Size: (%.2f, %.2f, %.2f)\n", vHalf.x, vHalf.y, vHalf.z);

	// 초록색으로 설정
	VTXCOL vCorner[8] = {
		{{vCenter.x - vHalf.x, vCenter.y - vHalf.y, vCenter.z - vHalf.z}, D3DXCOLOR(0.f, 1.f, 0.f, 1.f)},
		{{vCenter.x + vHalf.x, vCenter.y - vHalf.y, vCenter.z - vHalf.z}, D3DXCOLOR(0.f, 1.f, 0.f, 1.f)},
		{{vCenter.x + vHalf.x, vCenter.y + vHalf.y, vCenter.z - vHalf.z}, D3DXCOLOR(0.f, 1.f, 0.f, 1.f)},
		{{vCenter.x - vHalf.x, vCenter.y + vHalf.y, vCenter.z - vHalf.z}, D3DXCOLOR(0.f, 1.f, 0.f, 1.f)},
		{{vCenter.x - vHalf.x, vCenter.y - vHalf.y, vCenter.z + vHalf.z}, D3DXCOLOR(0.f, 1.f, 0.f, 1.f)},
		{{vCenter.x + vHalf.x, vCenter.y - vHalf.y, vCenter.z + vHalf.z}, D3DXCOLOR(0.f, 1.f, 0.f, 1.f)},
		{{vCenter.x + vHalf.x, vCenter.y + vHalf.y, vCenter.z + vHalf.z}, D3DXCOLOR(0.f, 1.f, 0.f, 1.f)},
		{{vCenter.x - vHalf.x, vCenter.y + vHalf.y, vCenter.z + vHalf.z}, D3DXCOLOR(0.f, 1.f, 0.f, 1.f)}
	};

	// 꼭지점 몇 개만 출력
	printf("Corner[0]: (%.2f, %.2f, %.2f)\n", vCorner[0].vPosition.x, vCorner[0].vPosition.y, vCorner[0].vPosition.z);
	printf("Corner[7]: (%.2f, %.2f, %.2f)\n", vCorner[7].vPosition.x, vCorner[7].vPosition.y, vCorner[7].vPosition.z);

	WORD  indices[24] = {
		0,1, 1,2, 2,3, 3,0,  // 앞면
		4,5, 5,6, 6,7, 7,4,  // 뒷면
		0,4, 1,5, 2,6, 3,7   // 연결선
	};

	_matrix I; D3DXMatrixIdentity(&I);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &I);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_COLORVERTEX, TRUE);
	m_pGraphicDev->SetFVF(FVF_COL);  // 색상 추가!

	HRESULT hr = m_pGraphicDev->DrawIndexedPrimitiveUP(
		D3DPT_LINELIST,
		0, 8,
		12,
		indices,
		D3DFMT_INDEX16,              // ★ WORD와 일치
		vCorner,
		sizeof(VTXCOL)
	);
	printf("DebugRender hr = 0x%08X\n", hr);

	// 최소 복구
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

bool CCollider::CheckCollision(CCollider* pOther)
{
	_vec3 vOwnerCenter = GetWorldCenter();
	_vec3 vOtherCenter = pOther->GetWorldCenter();

	_vec3 vOwnerHalf = m_vSize * 0.5f;
	_vec3 vOtherHalf = pOther->GetSize() * 0.5f;

	//AABB 충돌 검사 - center - center와 size + size 두 개를 비교
	if (abs(vOtherCenter.x - vOwnerCenter.x) >
		vOwnerHalf.x + vOtherHalf.x)
		return false;
	if (abs(vOtherCenter.y - vOwnerCenter.y) >
		vOwnerHalf.y + vOtherHalf.y)
		return false;
	if (abs(vOtherCenter.z - vOwnerCenter.z) >
		vOwnerHalf.z + vOtherHalf.z)
		return false;

	//검증 후 return true
	return true;
}

_vec3 CCollider::GetWorldCenter()
{
	CTransform* pTrans = dynamic_cast<CTransform*>(
		m_pOwner->Get_Component(ID_DYNAMIC, L"Com_Trans"));
	_vec3 vOwnerPos;
	pTrans->Get_Info(INFO_POS, &vOwnerPos);
	//오프셋과 owner의 offset을 더한 값을 return
	return vOwnerPos + m_vOffset;
}

CCollider* CCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCollider* pCollider = new CCollider(pGraphicDev);
	
	if (FAILED(pCollider->Ready_Collider()))
	{
		Safe_Release(pCollider);
		MSG_BOX("Transform Create Failed");
		return nullptr;
	}

	return pCollider;
}

CComponent* CCollider::Clone()
{
	return new CCollider(*this);
}

void CCollider::Free()
{
}
