#include "CRcCol.h"

CRcCol::CRcCol()
{
}

CRcCol::CRcCol(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CRcCol::CRcCol(const CRcCol& rhs)
	:CVIBuffer(rhs)
{
}

CRcCol::~CRcCol()
{
}
//사각형 버퍼 그릴 준비
HRESULT CRcCol::Ready_Buffer()
{
    for (int i = 0; i < 4; ++i)
    {
        m_fVertexAngle[i] = 0.f;
    }
    // 멤버 변수 초기화
    m_dwVtxCnt = 4;
    m_dwVtxSize = sizeof(VTXCOL);
    m_dwTriCnt = 2;
    m_dwFVF = FVF_COL;
    m_dwIdxSize = sizeof(WORD);
    m_IdxFmt = D3DFMT_INDEX16;

    // Vertex/Index Buffer 생성
    if (FAILED(CVIBuffer::Ready_Buffer()))
        return E_FAIL;

    // ✅ 1. 원본 정점 데이터를 별도 메모리에 할당
    m_pOriginVertex = new VTXCOL[m_dwVtxCnt];

    // ✅ 2. 원본 정점 데이터 채우기 (메모리에)
    m_pOriginVertex[0].vPosition = { -1.f, 1.f, 0.f };   // 왼쪽 위
    m_pOriginVertex[0].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

    m_pOriginVertex[1].vPosition = { 1.f, 1.f, 0.f };    // 오른쪽 위
    m_pOriginVertex[1].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

    m_pOriginVertex[2].vPosition = { 1.f, -1.f, 0.f };   // 오른쪽 아래
    m_pOriginVertex[2].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

    m_pOriginVertex[3].vPosition = { -1.f, -1.f, 0.f };  // 왼쪽 아래
    m_pOriginVertex[3].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

    // ✅ 3. Vertex Buffer에 복사 (임시 포인터 사용)
    VTXCOL* pVertex = nullptr;
    m_pVB->Lock(0, 0, (void**)&pVertex, 0);

    // 원본 데이터를 VB에 복사
    memcpy(pVertex, m_pOriginVertex, sizeof(VTXCOL) * m_dwVtxCnt);

    m_pVB->Unlock();  // ← pVertex는 무효화되지만, m_pOriginVertex는 살아있음!

    // 인덱스 데이터 채우기
    WORD* pIndex = nullptr;
    m_pIB->Lock(0, 0, (void**)&pIndex, 0);
    //첫번째 삼각형
    pIndex[0] = 3;
    pIndex[1] = 0;
    pIndex[2] = 1;
    //두번째 삼각형
    pIndex[3] = 3;
    pIndex[4] = 1;
    pIndex[5] = 2;

    m_pIB->Unlock();

    return S_OK;
}

_int CRcCol::Update_Component(const _float& fTimeDelta)
{
	//VTXCOL* pVertex = nullptr;
	//m_pVB->Lock(0, 0, (void**)&pVertex, D3DLOCK_DISCARD);

 //   float fRotSpeed = D3DXToRadian(90.f) * fTimeDelta;

	//for (_ulong i = 0; i < m_dwVtxCnt; ++i)
	//{
	//	_vec3 vOriginal = m_pOriginVertex[i].vPosition;

	//	//저장된 각도 사용 (자동 증가 아님!)
	//	//float fAngle = m_fVertexAngle[i];

 //       m_fVertexAngle[i] += fRotSpeed;

 //       float fAngle = m_fVertexAngle[i];

 //       _matrix matRotX, matRotY, matRotZ;
 //       D3DXMatrixRotationX(&matRotX,  fAngle);
 //       D3DXMatrixRotationY(&matRotY, fAngle);
 //       D3DXMatrixRotationZ(&matRotZ, fAngle);

 //       _matrix matRot = matRotY * matRotX * matRotZ;
 //       //원본 정점에 회전 적용
 //       D3DXVec3TransformCoord(&pVertex[i].vPosition, &vOriginal, &matRot);

	//	// Y축 회전만 - x와 z의 위치값만 변경
	//	//pVertex[i].vPosition.x = vOriginal.x * cosf(fAngle) - vOriginal.z * sinf(fAngle);
	//	//pVertex[i].vPosition.z = vOriginal.x * sinf(fAngle) + vOriginal.z * cosf(fAngle);
	//	//pVertex[i].vPosition.y = vOriginal.y;
 //       //X축 회전-  y와 z 위치값 변경
 //       //pVertex[i].vPosition.x = vOriginal.x;  // X는 변하지 않음!
 //       //pVertex[i].vPosition.y = vOriginal.y * cosf(fAngle) - vOriginal.z * sinf(fAngle);
 //       //pVertex[i].vPosition.z = vOriginal.y * sinf(fAngle) + vOriginal.z * cosf(fAngle);
 //       //Z축 회전 - x와 y의 위치값 변경
 //       //pVertex[i].vPosition.x = vOriginal.x * cosf(fAngle) - vOriginal.y * sinf(fAngle);
 //       //pVertex[i].vPosition.y = vOriginal.x * sinf(fAngle) + vOriginal.y * cosf(fAngle);
 //       //pVertex[i].vPosition.z = vOriginal.z;  // Z는 변하지 않음!
	//}

	//m_pVB->Unlock();
	return 0;
}

void CRcCol::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

void CRcCol::Update_VertexTwist(const _float fAngle)
{
    //m_fVertexAngle[iVertexIndex] += fAngle;
}

void CRcCol::Add_VertexRotation(_int iVertexIndex, _float fAngle)
{
	m_fVertexAngle[iVertexIndex] += fAngle;

    while (m_fVertexAngle[iVertexIndex] > D3DX_PI * 2.f)
        m_fVertexAngle[iVertexIndex] -= D3DX_PI * 2.f;

    while (m_fVertexAngle[iVertexIndex] < 0.f)
        m_fVertexAngle[iVertexIndex] += D3DX_PI * 2.f;
}

CRcCol* CRcCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	//처음 생성해서 CRcCol의 원본을 반환
	CRcCol* pRcCol = new CRcCol(pGraphicDev);

	if (FAILED(pRcCol->Ready_Buffer()))
	{
		Safe_Release(pRcCol);
		MSG_BOX("RcCol Create Failed");
		return nullptr;
	}
	return pRcCol;
}

CComponent* CRcCol::Clone()
{
	return new CRcCol(*this);
}

void CRcCol::Free()
{
	CVIBuffer::Free();
}
