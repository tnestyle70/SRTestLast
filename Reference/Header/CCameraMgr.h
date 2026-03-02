#pragma once
#include "CTransform.h"

BEGIN(Engine)

class ENGINE_DLL CCameraMgr : public CBase
{
	DECLARE_SINGLETON(CCameraMgr)
private:
	explicit CCameraMgr();
	virtual ~CCameraMgr();
public:
	HRESULT Ready_Camera(LPDIRECT3DDEVICE9 pGraphicDev, HWND hWnd);
	void Update(const _float fTimeDelta);
	void Set_Target(CTransform* pTargetTransform);
	void Add_CameraShake(_float fIntensity, _float fDuration);
	void Set_Dash(_bool bDash); //대쉬시 카메라 움직임

	const _matrix& GetViewMatrix() { return m_matView; }
	const _matrix& GetProjMatrix() { return m_matProj; }
	//카메라가 바라보는 방향 벡터(플레이어 이동에 사용)
	_vec3 GetLookVector() const;
	_vec3 GetRightVector() const;
private:
	void Update_FPSCamera(const _float& fTimeDelta);
	void Update_MouseInput(const _float& fTimeDelta);
	void Update_FOV(const _float& fTimeDelta);
	void Update_CameraShake(const _float& fTimeDelta);
private:
	bool m_bEditorMode = false;
private:
	HWND m_hWnd;
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	CTransform* m_pPlayerTransform; //카메라의 위치를 플레이어에게 고정
	//카메라 기본 회전용 변수
	_vec3 m_vOffset; //(0, 1.6f, 0) - 눈높이
	_float m_fPitch; //X축 회전
	_float m_fYaw; //Y축 회전
	_float m_fMouseSensitivity; //마우스 감도
	//카메라 행렬
	_vec3 m_vEye;
	_vec3 m_vAt;
	_vec3 m_vUp; //Y축
	_vec3 m_vLook; //Z축
	_vec3 m_vRight; //X축
	_matrix m_matView;
	_matrix m_matProj;
private:
	virtual void Free();
};

END