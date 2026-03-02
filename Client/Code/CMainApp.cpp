#include "pch.h"
#include "CMainApp.h"
#include "CStage.h"
#include "CLogo.h"
#include "CProtoMgr.h"
#include "CImGui.h"
#include "CInputMgr.h"

CMainApp::CMainApp()
    : m_pDeviceClass(nullptr), m_pGraphicDev(nullptr)
    , m_pManagementClass(CManagement::GetInstance())
{
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp()
{
    if (FAILED(Ready_DefaultSetting(&m_pGraphicDev)))
        return E_FAIL;

    if (FAILED(Ready_Scene(m_pGraphicDev)))
        return E_FAIL;

    return S_OK;
}

int CMainApp::Update_MainApp(const float& fTimeDelta)
{
    m_pManagementClass->Update_Scene(fTimeDelta);

    if (CInputMgr::GetInstance()->KeyDown(0))
        PostQuitMessage(0);
    
    CImGui::GetInstance()->Update();

    return 0;
}

void CMainApp::LateUpdate_MainApp(const float& fTimeDelta)
{
    m_pManagementClass->LateUpdate_Scene(fTimeDelta);

    CImGui::GetInstance()->LateUpdate();
}

void CMainApp::Render_MainApp()
{
    m_pDeviceClass->Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

    CImGui::GetInstance()->Render();

    m_pManagementClass->Render_Scene(m_pGraphicDev);
    // ImGui 렌더링
    //ImGui_ImplDX9_NewFrame();
    //ImGui_ImplWin32_NewFrame();
    //ImGui::NewFrame();

    //// UI 그리기
    //if (m_bShowEditor)
    //{
    //    ImGui::Begin("Test Window");
    //    ImGui::Text("Hello ImGui!");
    //    ImGui::End();
    //}

    //ImGui::Render();
    //ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    m_pDeviceClass->Render_End();
}

HRESULT CMainApp::Ready_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev)
{
    if (FAILED(CGraphicDev::GetInstance()->Ready_GraphicDev(g_hWnd, MODE_WIN, 1280, 720, &m_pDeviceClass)))
        return E_FAIL;

    m_pDeviceClass->AddRef();

    (*ppGraphicDev) = m_pDeviceClass->Get_GraphicDev();
    (*ppGraphicDev)->AddRef();

    (*ppGraphicDev)->SetRenderState(D3DRS_LIGHTING, FALSE);
    //카메라
    CCameraMgr::GetInstance()->Ready_Camera(m_pGraphicDev, g_hWnd);
    //IMGUI 초기화
    CImGui::GetInstance()->Init(g_hWnd, m_pGraphicDev);

    m_bShowEditor = true;

    return S_OK;
}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
    //처음 씬으로 로고를 생성해줌
    //Engine::CScene* pLogo = CLogo::Create(pGraphicDev);
    CScene* pStage = CStage::Create(pGraphicDev);
    
    if (nullptr == pStage)
        return E_FAIL;
    //managementclass의 set scene으로 pLogo를 넣어서 생성
    //씬을 변경하고 싶을 떄는 management클래스의 set_scene으로 scene을 변경해줌
    if (FAILED(m_pManagementClass->Set_Scene(pStage)))
    {
        Safe_Release(pStage);
        MSG_BOX("Stage Create Failed");
        return E_FAIL;
    }

    return S_OK;
}

CMainApp* CMainApp::Create()
{
    CMainApp* pInstance = new CMainApp;

    if (FAILED(pInstance->Ready_MainApp()))
    {
        Safe_Release(pInstance);
        MSG_BOX("MainApp Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CMainApp::Free()
{
    Safe_Release(m_pDeviceClass);
    Safe_Release(m_pGraphicDev);

    //ImGui 해제
    CImGui::GetInstance()->DestroyInstance();

    CCameraMgr::GetInstance()->DestroyInstance();
    CProtoMgr::GetInstance()->DestroyInstance();
    CFrameMgr::GetInstance()->DestroyInstance();
    CTimerMgr::GetInstance()->DestroyInstance();
    CManagement::GetInstance()->DestroyInstance();
    CGraphicDev::GetInstance()->DestroyInstance();
}
