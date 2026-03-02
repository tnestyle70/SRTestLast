#include "CTriCol.h"

CTriCol::CTriCol()
{
}

CTriCol::CTriCol(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CTriCol::CTriCol(const CTriCol& rhs)
	: CVIBuffer(rhs)
{
}

CTriCol::~CTriCol()
{
}

HRESULT CTriCol::Ready_Buffer() //삼각형 버퍼 그릴 준비
{
	for (int i = 0; i < 3; ++i)
	{
		m_fVertexAngle[i] = 0.f;
	}

	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 3;
	m_dwTriCnt = 1;
    m_dwIdxSize = 0;
	m_dwFVF = FVF_COL;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	//원본 정점 데이터를 별도 메모리에 할당
	m_pOriginVertex = new VTXCOL[m_dwVtxCnt];
	//원본 정점 데이터 채우기
	m_pOriginVertex[0].vPosition = { 0.f, 1.f, 0.f }; //위쪽
	m_pOriginVertex[0].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	m_pOriginVertex[1].vPosition = { 1.f, -1.f, 0.f }; //오른쪽 아래
	m_pOriginVertex[1].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	m_pOriginVertex[2].vPosition = { -1.f, -1.f, 0.f }; //왼쪽 아래
	m_pOriginVertex[2].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	VTXCOL* pVertex = NULL;
	// &pVertex : 버텍스 버퍼에 저장된 버텍스 중 첫번째 버텍스의 주소를 얻어옴
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	// 원본 데이터를 VB에 복사
	memcpy(pVertex, m_pOriginVertex, sizeof(VTXCOL) * m_dwVtxCnt);
	m_pVB->Unlock();
	/*
	pVertex[0].vPosition = { 0.f, 1.f, 0.f }; //위쪽
	pVertex[0].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertex[1].vPosition = { 1.f, -1.f, 0.f }; //오른쪽 아래
	pVertex[1].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVertex[2].vPosition = { -1.f, -1.f, 0.f }; //왼쪽 아래
	pVertex[2].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	m_pVB->Unlock();
	*/
	return S_OK;
}

_int CTriCol::Update_Component(const _float& fTimeDelta)
{
	VTXCOL* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, D3DLOCK_DISCARD);

	float fRotSpeed = D3DXToRadian(30.f) * fTimeDelta;

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
	{
		_vec3 vOriginal = m_pOriginVertex[i].vPosition;

		//저장된 각도 사용 (자동 증가 아님!)
		//float fAngle = m_fVertexAngle[i];

		m_fVertexAngle[i] += fRotSpeed;

		float fAngle = m_fVertexAngle[i];

		_matrix matRotX, matRotY, matRotZ;
		D3DXMatrixRotationX(&matRotX, fAngle);
		D3DXMatrixRotationY(&matRotY, fAngle);
		D3DXMatrixRotationZ(&matRotZ, fAngle);

		_matrix matRot = matRotY * matRotX * matRotZ;
		//원본 정점에 회전 적용
		D3DXVec3TransformCoord(&pVertex[i].vPosition, &vOriginal, &matRot);
	}

	m_pVB->Unlock();
	return 0;
}

void CTriCol::Render_Buffer() //ready_buffer에서 준비한 buffer render
{
	CVIBuffer::Render_Buffer();
}

CTriCol* CTriCol::Create(LPDIRECT3DDEVICE9 pGraphicDev) //처음 CTriCol 생성
{
	CTriCol* pTriCol = new CTriCol(pGraphicDev);

	if (FAILED(pTriCol->Ready_Buffer()))
	{
		Safe_Release(pTriCol);
		MSG_BOX("TriCol Create Failed");
		return nullptr;
	}

	return pTriCol;
}

CComponent* CTriCol::Clone()
{
	return new CTriCol(*this);
}

void CTriCol::Free()
{
	CVIBuffer::Free();
}
