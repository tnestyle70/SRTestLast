#include "CVIBuffer.h"

CVIBuffer::CVIBuffer() : m_pVB(nullptr), m_pIB(nullptr)
, m_dwVtxSize(0)
, m_dwVtxCnt(0)
, m_dwTriCnt(0)
, m_dwIdxSize(0)
, m_IdxFmt(D3DFMT_INDEX16)
{
}

CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev), m_pVB(nullptr), m_pIB(nullptr)
	, m_dwVtxSize(0)
	, m_dwVtxCnt(0)
	, m_dwTriCnt(0)
	, m_dwIdxSize(0)
	, m_dwFVF(0)
	, m_IdxFmt(D3DFMT_INDEX16)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	:CComponent(rhs), m_pVB(rhs.m_pVB), m_pIB(rhs.m_pIB)
	, m_dwVtxSize(rhs.m_dwVtxSize)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwTriCnt(rhs.m_dwTriCnt)
	, m_dwFVF(rhs.m_dwFVF)
	, m_dwIdxSize(rhs.m_dwIdxSize)
	, m_IdxFmt(rhs.m_IdxFmt)
	, m_pOriginVertex(nullptr)
{
	m_pVB->AddRef();
	if(m_dwIdxSize > 0)
		m_pIB->AddRef();
	if (rhs.m_pOriginVertex != nullptr)
	{
		m_pOriginVertex = rhs.m_pOriginVertex;
	}
	// ===== m_fVertexAngle 배열 복사 =====
	for (int i = 0; i < 4; ++i)
	{
		m_fVertexAngle[i] = rhs.m_fVertexAngle[i];
	}
}

CVIBuffer::~CVIBuffer()
{
}

HRESULT CVIBuffer::Ready_Buffer()
{
	// D3DPOOL_DEFAULT : 그래픽 메모리
	// D3DPOOL_MANAGED : 그래픽 메모리에 저장, 메인 메모리 백업
	// D3DPOOL_SYSTEMMEM : 메인 메모리에 저장
	// D3DPOOL_SCRATCH : 메인 메모리에 저장(DX 장치로 접근 불가)

	//VB는 무조건 생성
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(m_dwVtxCnt * m_dwVtxSize,	// 버텍스 버퍼의 크기
												0,		// 0인 경우 정적 버퍼, D3DUSAGE_DYNAMIC인 경우 동적 버퍼
												m_dwFVF, // 버텍스 속성
												D3DPOOL_MANAGED, // 정적 버퍼인 경우 MANAGED
												&m_pVB,			// 버텍스 버퍼 객체 생성
												NULL)))			// 공유할 일 없기 때문에 null
												return E_FAIL;
	//Index Buffer를 사용하기 위해서 index buffer를 생성
	//index buffer는 조건을 줘서 생성 -> 삼각형은 index buffer를 생성하지 않음
	if (m_dwIdxSize > 0)
	{
		if (FAILED(m_pGraphicDev->CreateIndexBuffer(
			m_dwIdxSize * m_dwTriCnt * 3,
			0,
			m_IdxFmt,
			D3DPOOL_MANAGED,
			&m_pIB,
			NULL)))
			return E_FAIL;
	}

	return S_OK;
}

void CVIBuffer::Render_Buffer()
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);

	m_pGraphicDev->SetFVF(m_dwFVF);

	if (m_pIB)
	{
		m_pGraphicDev->SetIndices(m_pIB);
		m_pGraphicDev->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,  // Primitive 타입
			0,                   // BaseVertexIndex
			0,                   // MinVertexIndex
			m_dwVtxCnt,          // NumVertices (정점 개수)
			0,                   // StartIndex
			m_dwTriCnt);         // PrimitiveCount
	}
	else
	{
		m_pGraphicDev->SetIndices(nullptr);
		//index buffer 없이 그릴 경우에는 DrawPrimitive 호출
		m_pGraphicDev->DrawPrimitive(
			D3DPT_TRIANGLELIST,
			0, m_dwTriCnt);
	}
}

void CVIBuffer::Free()
{
	Safe_Release(m_pVB);
	if(m_pIB)
		Safe_Release(m_pIB);

	CComponent::Free();
}
