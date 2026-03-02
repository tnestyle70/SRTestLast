#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer : public CComponent
{
protected:
	explicit CVIBuffer();
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer();

public:
	virtual HRESULT		Ready_Buffer();
	virtual void		Render_Buffer();
	virtual void Update_VertexTwist(const _float fAngle) {};
	virtual void Add_VertexRotation(_int iVertexIndex, _float fAngle) {};
protected:
	LPDIRECT3DVERTEXBUFFER9			m_pVB;
	LPDIRECT3DINDEXBUFFER9 m_pIB;

	_ulong			m_dwVtxSize;
	_ulong			m_dwVtxCnt;
	_ulong			m_dwTriCnt;
	_ulong			m_dwFVF;
	//Index Buffer 추가(vertex buffer 재사용)
	_ulong m_dwIdxSize;
	D3DFORMAT m_IdxFmt;
	VTXCOL* m_pOriginVertex = nullptr;
	_float m_fVertexAngle[4]; //정점 4개
	//_float m_fAngle;
public:
	virtual void	Free();
};

END
