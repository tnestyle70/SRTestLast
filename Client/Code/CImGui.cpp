#include "pch.h"
#include "CImGui.h"
#include "CInputMgr.h"

// 오브젝트 타입 목록 — 여기에 뮬렛 매드잭 오브젝트 추가
static const char* s_ObjectTypes[] =
{
    // 맵 오브젝트
    "Wall_Straight",
    "Wall_Corner",
    "Wall_TShape",
    "Floor_Basic",
    "Floor_Metal",
    "Ceiling_Basic",
    // 장애물
    "Pillar",
    "Crate",
    "Barrel",
    // 적
    "Enemy_Melee",
    "Enemy_Ranged",
    "Enemy_Boss",
    // 기타
    "SpawnPoint_Player",
    "SpawnPoint_Enemy",
};

static const int s_ObjectTypeCount = IM_ARRAYSIZE(s_ObjectTypes);

IMPLEMENT_SINGLETON(CImGui)

CImGui::CImGui()
{
}

CImGui::~CImGui()
{
}

void CImGui::Init(HWND _hWnd, LPDIRECT3DDEVICE9 _pDevice)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(_hWnd);
    ImGui_ImplDX9_Init(_pDevice);
}

_int CImGui::Update()
{
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    //tab 키 누르면 editor 모드 전환
    if (CInputMgr::GetInstance()->KeyPress(VK_TAB))
    {
        m_bEditorMode = !m_bEditorMode;
        if (m_bEditorMode)
        {
            // 에디터 모드 — 마우스 해제
            ShowCursor(TRUE);
            ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
        }
        else
        {
            // 게임 모드 — 마우스 고정
            ShowCursor(FALSE);
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
        }
    }

    // 단축키
    //if (ImGui::IsKeyPressed(ImGuiKey_Escape))
    //{
    //    m_iSelectedAsset = -1;
    //    m_bPlacementMode = false;
    //}
    //if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_S))
    //    Save_Level("Level_01.dat");
    //if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_L))
    //    Load_Level("Level_01.dat");
    //if (ImGui::IsKeyPressed(ImGuiKey_Delete) && m_iSelectedObject >= 0)
    //{
    //    m_vecObjects.erase(m_vecObjects.begin() + m_iSelectedObject);
    //    m_iSelectedObject = -1;
    //}

    // 패널 렌더
    Render_MainMenuBar();
    Render_AssetBrowser();
    Render_Viewport();
    Render_Inspector();
    Render_Hierachy();

    return 0;
}

void CImGui::LateUpdate()
{
}

void CImGui::Render()
{
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void CImGui::Render_MainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save Level", "Ctrl+S"))  Save_Level("Level_01.dat");
            if (ImGui::MenuItem("Load Level", "Ctrl+L"))  Load_Level("Level_01.dat");
            if (ImGui::MenuItem("New Level"))               m_vecObjects.clear();
            ImGui::Separator();
            if (ImGui::MenuItem("Exit"))                    PostQuitMessage(0);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Delete Selected", "Del"))
            {
                if (m_iSelectedObject >= 0)
                {
                    m_vecObjects.erase(
                        m_vecObjects.begin() + m_iSelectedObject);
                    m_iSelectedObject = -1;
                }
            }
            if (ImGui::MenuItem("Clear All"))
                m_vecObjects.clear();
            ImGui::EndMenu();
        }

        // 상태 표시
        ImGui::Separator();
        ImGui::Text("Objects: %d", (int)m_vecObjects.size());
        if (m_iSelectedAsset >= 0)
        {
            ImGui::Separator();
            ImGui::TextColored(ImVec4(0, 1, 0, 1),
                "배치 모드: %s", s_ObjectTypes[m_iSelectedAsset]);
        }

        ImGui::EndMainMenuBar();
    }
}

void CImGui::Render_AssetBrowser()
{
    // 좌측 고정
    ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(180, 580), ImGuiCond_Always);
    ImGui::Begin("Asset Browser", nullptr,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    ImGui::Text("[ Map Objects ]");
    ImGui::Separator();

    // 카테고리별로 묶어서 표시
    if (ImGui::CollapsingHeader("Wall / Floor", ImGuiTreeNodeFlags_DefaultOpen))
    {
        for (int i = 0; i < 6; i++)  // Wall, Floor 타입
        {
            bool bSelected = (m_iSelectedAsset == i);
            if (ImGui::Selectable(s_ObjectTypes[i], bSelected))
            {
                m_iSelectedAsset = i;
                m_bPlacementMode = true;
            }
        }
    }

    if (ImGui::CollapsingHeader("Props"))
    {
        for (int i = 6; i < 9; i++)
        {
            bool bSelected = (m_iSelectedAsset == i);
            if (ImGui::Selectable(s_ObjectTypes[i], bSelected))
            {
                m_iSelectedAsset = i;
                m_bPlacementMode = true;
            }
        }
    }

    if (ImGui::CollapsingHeader("Enemies"))
    {
        for (int i = 9; i < 12; i++)
        {
            bool bSelected = (m_iSelectedAsset == i);
            if (ImGui::Selectable(s_ObjectTypes[i], bSelected))
            {
                m_iSelectedAsset = i;
                m_bPlacementMode = true;
            }
        }
    }

    if (ImGui::CollapsingHeader("Spawn Points"))
    {
        for (int i = 12; i < 14; i++)
        {
            bool bSelected = (m_iSelectedAsset == i);
            if (ImGui::Selectable(s_ObjectTypes[i], bSelected))
            {
                m_iSelectedAsset = i;
                m_bPlacementMode = true;
            }
        }
    }

    ImGui::Separator();
    if (m_iSelectedAsset >= 0)
    {
        ImGui::TextColored(ImVec4(0, 1, 0, 1), "Selected: %s",
            s_ObjectTypes[m_iSelectedAsset]);
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "배치 모드 ON");
        ImGui::Text("Space: 배치 / ESC: 취소");
    }

    ImGui::End();
}

void CImGui::Render_Viewport()
{
    // Viewport 컨트롤은 상단 좁게
    ImGui::SetNextWindowPos(ImVec2(200, 20), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(360, 180), ImGuiCond_Always);
    ImGui::Begin("Viewport Controls", nullptr,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    // 배치 위치 수동 입력
    static float fPos[3] = { 0.f, 0.f, 0.f };
    static float fRotY = 0.f;
    static float fScale[3] = { 1.f, 1.f, 1.f };

    ImGui::Text("[ Placement ]");
    ImGui::Separator();

    ImGui::DragFloat3("Position", fPos, 0.1f);
    ImGui::DragFloat("Rotation Y", &fRotY, 1.f, -180.f, 180.f);
    ImGui::DragFloat3("Scale", fScale, 0.1f, 0.1f, 100.f);

    ImGui::Separator();

    // 스냅 기능
    static bool  bSnap = true;
    static float fSnapGrid = 1.f;
    ImGui::Checkbox("Grid Snap", &bSnap);
    if (bSnap)
    {
        ImGui::SameLine();
        ImGui::SetNextItemWidth(80.f);
        ImGui::DragFloat("##snap", &fSnapGrid, 0.5f, 0.5f, 10.f);
        ImGui::SameLine();
        ImGui::Text("%.1f", fSnapGrid);

        // 스냅 적용
        if (bSnap)
        {
            fPos[0] = round(fPos[0] / fSnapGrid) * fSnapGrid;
            fPos[1] = round(fPos[1] / fSnapGrid) * fSnapGrid;
            fPos[2] = round(fPos[2] / fSnapGrid) * fSnapGrid;
        }
    }

    ImGui::Separator();

    // 배치 버튼
    bool bCanPlace = (m_iSelectedAsset >= 0);
    if (!bCanPlace) ImGui::BeginDisabled();

    if (ImGui::Button("배치 [Space]", ImVec2(120, 30)))
    {
        ObjectData data;
        data.strName = s_ObjectTypes[m_iSelectedAsset];
        data.iType = m_iSelectedAsset;
        data.fX = fPos[0]; data.fY = fPos[1]; data.fZ = fPos[2];
        data.fRotY = fRotY;
        data.fScaleX = fScale[0];
        data.fScaleY = fScale[1];
        data.fScaleZ = fScale[2];
        m_vecObjects.push_back(data);
    }

    if (!bCanPlace) ImGui::EndDisabled();

    ImGui::SameLine();

    if (ImGui::Button("선택 취소", ImVec2(100, 30)))
    {
        m_iSelectedAsset = -1;
        m_bPlacementMode = false;
    }

    ImGui::End();
}

void CImGui::Render_Inspector()
{
    // 우측 고정
    ImGui::SetNextWindowPos(ImVec2(1000, 20), ImGuiCond_Always);
    // ← 1280 - 200 = 1080 위치
    ImGui::SetNextWindowSize(ImVec2(200, 600), ImGuiCond_Always);
    ImGui::Begin("Inspector", nullptr,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    if (m_iSelectedObject < 0 ||
        m_iSelectedObject >= (int)m_vecObjects.size())
    {
        ImGui::Text("선택된 오브젝트 없음");
        ImGui::End();
        return;
    }

    ObjectData& obj = m_vecObjects[m_iSelectedObject];

    ImGui::Text("[ %s ]", obj.strName.c_str());
    ImGui::Separator();

    float fPos[3] = { obj.fX, obj.fY, obj.fZ };
    float fScale[3] = { obj.fScaleX, obj.fScaleY, obj.fScaleZ };

    if (ImGui::DragFloat3("Position", fPos, 0.1f))
    {
        obj.fX = fPos[0]; obj.fY = fPos[1]; obj.fZ = fPos[2];
    }
    if (ImGui::DragFloat("Rotation Y", &obj.fRotY, 1.f, -180.f, 180.f))
    {
        // 자동 반영
    }
    if (ImGui::DragFloat3("Scale", fScale, 0.1f, 0.1f, 100.f))
    {
        obj.fScaleX = fScale[0];
        obj.fScaleY = fScale[1];
        obj.fScaleZ = fScale[2];
    }

    ImGui::Separator();

    // 삭제 버튼
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.f));
    if (ImGui::Button("Delete", ImVec2(-1, 30)))
    {
        m_vecObjects.erase(m_vecObjects.begin() + m_iSelectedObject);
        m_iSelectedObject = -1;
    }
    ImGui::PopStyleColor();

    ImGui::End();
}

void CImGui::Render_Hierachy()
{
    // 하단 고정
    ImGui::SetNextWindowPos(ImVec2(200, 550), ImGuiCond_Always);
    // ← 720 - 140 = 580 위치
    ImGui::SetNextWindowSize(ImVec2(500, 200), ImGuiCond_Always);
    ImGui::Begin("Hierarchy", nullptr,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    ImGui::Text("배치된 오브젝트: %d개", (int)m_vecObjects.size());
    ImGui::Separator();

    // 필터 검색
    static char szFilter[64] = "";
    ImGui::InputText("Search", szFilter, IM_ARRAYSIZE(szFilter));
    ImGui::Separator();

    for (int i = 0; i < (int)m_vecObjects.size(); i++)
    {
        // 필터 적용
        if (strlen(szFilter) > 0 &&
            m_vecObjects[i].strName.find(szFilter) == string::npos)
            continue;

        char szLabel[128];
        sprintf_s(szLabel, "[%d] %s  (%.1f, %.1f, %.1f)",
            i,
            m_vecObjects[i].strName.c_str(),
            m_vecObjects[i].fX,
            m_vecObjects[i].fY,
            m_vecObjects[i].fZ);

        bool bSelected = (m_iSelectedObject == i);
        if (ImGui::Selectable(szLabel, bSelected))
            m_iSelectedObject = i;
    }

    ImGui::End();
}

void CImGui::Save_Level(const char* szPath)
{
    FILE* pFile = nullptr;
    fopen_s(&pFile, szPath, "wb");
    if (!pFile) return;

    // 오브젝트 개수 저장
    int iCount = (int)m_vecObjects.size();
    fwrite(&iCount, sizeof(int), 1, pFile);

    // 각 오브젝트 저장
    for (auto& obj : m_vecObjects)
    {
        // 이름 저장 (길이 + 문자열)
        int iLen = (int)obj.strName.length();
        fwrite(&iLen, sizeof(int), 1, pFile);
        fwrite(obj.strName.c_str(), sizeof(char), iLen, pFile);

        fwrite(&obj.iType, sizeof(int), 1, pFile);
        fwrite(&obj.fX, sizeof(float), 1, pFile);
        fwrite(&obj.fY, sizeof(float), 1, pFile);
        fwrite(&obj.fZ, sizeof(float), 1, pFile);
        fwrite(&obj.fRotY, sizeof(float), 1, pFile);
        fwrite(&obj.fScaleX, sizeof(float), 1, pFile);
        fwrite(&obj.fScaleY, sizeof(float), 1, pFile);
        fwrite(&obj.fScaleZ, sizeof(float), 1, pFile);
    }

    fclose(pFile);
    MessageBoxA(nullptr, "저장 완료!", "Save", MB_OK);
}

void CImGui::Load_Level(const char* szPath)
{
    FILE* pFile = nullptr;
    fopen_s(&pFile, szPath, "rb");
    if (!pFile) return;

    m_vecObjects.clear();

    int iCount = 0;
    fread(&iCount, sizeof(int), 1, pFile);

    for (int i = 0; i < iCount; i++)
    {
        ObjectData obj;

        // 이름 읽기
        int iLen = 0;
        fread(&iLen, sizeof(int), 1, pFile);
        char szName[128] = {};
        fread(szName, sizeof(char), iLen, pFile);
        obj.strName = szName;

        fread(&obj.iType, sizeof(int), 1, pFile);
        fread(&obj.fX, sizeof(float), 1, pFile);
        fread(&obj.fY, sizeof(float), 1, pFile);
        fread(&obj.fZ, sizeof(float), 1, pFile);
        fread(&obj.fRotY, sizeof(float), 1, pFile);
        fread(&obj.fScaleX, sizeof(float), 1, pFile);
        fread(&obj.fScaleY, sizeof(float), 1, pFile);
        fread(&obj.fScaleZ, sizeof(float), 1, pFile);

        m_vecObjects.push_back(obj);
    }

    fclose(pFile);
    MessageBoxA(nullptr, "로드 완료!", "Load", MB_OK);
}

void CImGui::Free()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}