#include "CRcTex.h"

CRcTex::CRcTex()
{
}

CRcTex::CRcTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CRcTex::CRcTex(const CRcTex& rhs)
	: CVIBuffer(rhs)
{
}

CRcTex::~CRcTex()
{
}

HRESULT CRcTex::Ready_Buffer(_float fUScale, _float fVScale)
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 4;
	m_dwTriCnt = 2;
	m_dwFVF = FVF_TEX;
	//인덱스 버퍼의 사이즈 설정
	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXTEX* pVertex = NULL;
	//&pVertex : 버텍스 버퍼에 저장된 버텍스 중 첫번째 버텍스의 주소 얻어오기
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	//좌상
	pVertex[0].vPosition = { -1.f, 1.f, 0.f };
	pVertex[0].vTexUV = { 0.f , 0.f };
	//우상
	pVertex[1].vPosition = { 1.f, 1.f, 0.f };
	pVertex[1].vTexUV = { fUScale, 0.f };
	//우하
	pVertex[2].vPosition = { 1.f, -1.f, 0.f };
	pVertex[2].vTexUV = { fUScale, fVScale };
	//좌하
	pVertex[3].vPosition = { -1.f, -1.f, 0.f };
	pVertex[3].vTexUV = { 0.f, fVScale };
	m_pVB->Unlock();

	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);
	//오른쪽 위
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;
	//왼쪽 아래
	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;
	m_pIB->Unlock();
	return S_OK;
}

void CRcTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CRcTex* CRcTex::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	_float fUScale, _float fVScale)
{
	CRcTex* pRcTex = new CRcTex(pGraphicDev);

	if (FAILED(pRcTex->Ready_Buffer(fUScale, fVScale)))
	{
		Safe_Release(pRcTex);
		MSG_BOX("RcTex Create Failed");
		return nullptr;
	}

	return pRcTex;
}

CComponent* CRcTex::Clone()
{
	return new CRcTex(*this);
}

void CRcTex::Free()
{
	CVIBuffer::Free();
}
