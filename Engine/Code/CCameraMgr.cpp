#include "CCameraMgr.h"
#include "CInputMgr.h"

IMPLEMENT_SINGLETON(CCameraMgr)

CCameraMgr::CCameraMgr()
	: m_pGraphicDev(nullptr)
	, m_pPlayerTransform(nullptr)
	, m_vOffset(0.f, 1.6f, 0.f)
	, m_fPitch(0.f)
	, m_fYaw(0.f)
	, m_fMouseSensitivity(0.002f)
	, m_vEye(0.f, 0.f, 0.f)
	, m_vAt(0.f, 0.f, 0.f)
	, m_vUp(0.f, 1.f, 0.f)
	, m_vLook(0.f, 0.f, 0.f)
	, m_vRight(0.f, 0.f, 0.f)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

CCameraMgr::~CCameraMgr()
{
}

HRESULT CCameraMgr::Ready_Camera(LPDIRECT3DDEVICE9 pGraphicDev
	, HWND hWnd)
{
	m_hWnd = hWnd;
	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();

	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(90.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return S_OK;
}

void CCameraMgr::Update(const _float fTimeDelta)
{
	if (CInputMgr::GetInstance()->KeyPress(VK_TAB))
	{
		m_bEditorMode = !m_bEditorMode;
	}
	if (!m_bEditorMode)
	{
		return;
	}

	if (!m_pPlayerTransform)
		return;

	Update_MouseInput(fTimeDelta);
	Update_CameraShake(fTimeDelta);
	Update_FOV(fTimeDelta);
	Update_FPSCamera(fTimeDelta);

	//Update를 마친 이후에 SetTransform으로 카메라를 업데이트해준다
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
}

void CCameraMgr::Update_MouseInput(const _float& fTimeDelta)
{
	POINT ptMouse, ptCenter;
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	ptCenter.x = WINCX >> 1;
	ptCenter.y = WINCY >> 1;

	_long dwMouseX = ptMouse.x - ptCenter.x;
	_long dwMouseY = ptMouse.y - ptCenter.y;

	m_fYaw += dwMouseX * m_fMouseSensitivity;
	m_fPitch -= dwMouseY * m_fMouseSensitivity;

	ClientToScreen(m_hWnd, &ptCenter);
	SetCursorPos(ptCenter.x, ptCenter.y);

	//if (0 != dwMouseX || 0 != dwMouseY)
	//{
	//	m_fYaw += dwMouseX * m_fMouseSensitivity;
	//	m_fPitch += dwMouseY * m_fMouseSensitivity;

	//	ClientToScreen(m_hWnd, &ptCenter);
	//	SetCursorPos(ptCenter.x, ptCenter.y);
	//}
}

void CCameraMgr::Update_CameraShake(const _float& fTimeDelta)
{
}

void CCameraMgr::Update_FPSCamera(const _float& fTimeDelta)
{
	_vec3 vPlayerPos;
	m_pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
	//카메라 위치
	m_vEye = vPlayerPos + m_vOffset;
	//방향 벡터 계산
	m_vLook.x = cosf(m_fPitch) * sinf(m_fYaw);
	m_vLook.y = sinf(m_fPitch);
	m_vLook.z = cosf(m_fPitch) * cosf(m_fYaw);
	D3DXVec3Normalize(&m_vLook, &m_vLook);
	//오른쪽 벡터 계산 - 플레이어 이동에 사용, up벡터와 look 벡터의 외적을 통해서 수직인 벡터 구하기
	D3DXVec3Cross(&m_vRight, &m_vUp, &m_vLook); 
	D3DXVec3Normalize(&m_vRight, &m_vRight);
	
	m_vAt = m_vEye + m_vLook;

	static int frame = 0;

	if (++frame % 60 == 0)  // 1초에 한 번
	{
		printf("=== Camera Debug ===\n");
		printf("Player Pos: %.1f, %.1f, %.1f\n", vPlayerPos.x, vPlayerPos.y, vPlayerPos.z);
		printf("Camera Eye: %.1f, %.1f, %.1f\n", m_vEye.x, m_vEye.y, m_vEye.z);
		printf("Camera At:  %.1f, %.1f, %.1f\n", m_vAt.x, m_vAt.y, m_vAt.z);
		printf("Look:       %.2f, %.2f, %.2f\n", m_vLook.x, m_vLook.y, m_vLook.z);
		printf("Pitch: %.2f, Yaw: %.2f\n", m_fPitch, m_fYaw);
	}

	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
}

void CCameraMgr::Update_FOV(const _float& fTimeDelta)
{
	//대시 시 FOV 증가
}

void CCameraMgr::Set_Target(CTransform* pTargetTransform)
{
	m_pPlayerTransform = pTargetTransform;
}

void CCameraMgr::Add_CameraShake(_float fIntensity, _float fDuration)
{
}

void CCameraMgr::Set_Dash(_bool bDash)
{
}
//플레이어 이동용 방향 벡터 제공
_vec3 CCameraMgr::GetLookVector() const
{
	return _vec3(m_vLook.x, 0.f, m_vLook.z);
}

_vec3 CCameraMgr::GetRightVector() const
{
	return _vec3(m_vRight.x, 0.f, m_vRight.z);
}

void CCameraMgr::Free()
{
}
