#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTex : public CVIBuffer
{
protected:
	explicit CRcTex();
	explicit CRcTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcTex(const CRcTex& rhs);
	virtual ~CRcTex();
public:
	virtual HRESULT Ready_Buffer(_float fUScale = 1.f, _float fVScale = 1.f);
	virtual void Render_Buffer();
public:
	static CRcTex* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		_float fUScale = 1.f, _float fVScale = 1.f);
	virtual CComponent* Clone();
private:
	virtual void Free();
};

END