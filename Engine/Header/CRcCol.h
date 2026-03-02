#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcCol : public CVIBuffer
{
protected:
	explicit CRcCol();
	explicit CRcCol(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcCol(const CRcCol& rhs);
	virtual ~CRcCol();
public: //Buffer 그릴 준비
	virtual HRESULT Ready_Buffer();
	virtual _int Update_Component(const _float& fTimeDelta);
	virtual void Render_Buffer();
	void Update_VertexTwist(const _float fAngle) override;
public:
	void Add_VertexRotation(_int iVertexIndex, _float fAngle) override;
public:
	static CRcCol* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone(); //component 타입의 clone을 반환
private:
	virtual void Free();
};

END