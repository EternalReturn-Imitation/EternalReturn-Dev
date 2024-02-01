#include "pch.h"
#include "AnimEditUI.h"

#include <Engine/CDevice.h>
#include <Engine/CStructuredBuffer.h>

#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include <Engine/CRenderMgr.h>
#include <Engine/CMRT.h>

#include <Engine/CResMgr.h>
#include <Engine/CTexture.h>
#include <Engine/CMeshData.h>
#include <Engine/CMesh.h>

#include "ListUI.h"

#include "CEditorObjMgr.h"
#include "CAnimEditObj.h"


AnimEditUI::AnimEditUI()
    : UI("##AnimEditUI")
    , m_pRenderObj(nullptr)
    , m_iFrameCount(0)
{
    SetName("AnimEditUI");


    // 에디터 윈도우 세팅
    {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        SetFlags(window_flags);
        SetModal(false);
    }
}

AnimEditUI::~AnimEditUI()
{
}

void AnimEditUI::render_cliplistwindow()
{
    ImGui::Button("AnimClipList", ImVec2(200, 0));
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;

    ImVec2 ItemSize = ImGui::GetItemRectSize();

    int ClipCount = m_mapAnimClip.size();
    int ClipIdx = 0;
    static int SelectClipIdx = 0;

    ImGui::BeginChild("##AnimClipList", ImVec2(200.f, 600.f - ItemSize.y), false, window_flags);;

    map<wstring, tMTAnimClip>::iterator iter = m_mapAnimClip.begin();
    // 클립이 1개 이상인 경우 리스트 출력
    while (iter != m_mapAnimClip.end())
    {
        ClipIdx++;

        if (ImGui::MenuItem(string(iter->first.begin(), iter->first.end()).c_str(),"", ClipIdx == SelectClipIdx))
        {
            SelectClipIdx = ClipIdx;
            m_tMTCurAnimClip = iter->second;
        }

        iter++;
    }
    
    ImGui::EndChild();

    char buf[10];
    sprintf_s(buf, "%d / %d", SelectClipIdx, ClipCount);

    ImGui::Text(buf);

    ImGui::SameLine();
    ImGui::Button("+");
    ImGui::SameLine();
    ImGui::Button("-");
}

void AnimEditUI::render_previewwindow()
{
    ImGui::Button("Preview Window", ImVec2(600, 0));

    Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"3DAnimEditTex");

    int width = (int)pTex->Width();
    int height = (int)pTex->Height();

    ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
    ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
    ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white

    ImGui::Image((ImTextureID)pTex->GetSRV().Get(), ImVec2(600, 600), uv_min, uv_max, tint_col, border_col);
}

void AnimEditUI::render_infowindow()
{
    // 전체 애니메이션 정보
    // ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
    // ImGui::BeginChild("##MeshInfo", ImVec2(200.f, 0.f), false, window_flags);
    ImGui::BeginGroup();
    ImGui::Button("420", ImVec2(420, 0));
    ImGui::Button("425", ImVec2(425, 0));
    ImGui::Button("430", ImVec2(430, 0));
    ImGui::Button("435", ImVec2(435, 0));
    ImGui::Button("440", ImVec2(440, 0));
    ImGui::Button("445", ImVec2(445, 0));
    ImGui::Button("450", ImVec2(450, 0));

    
    // Mesh Res Name

    string strMeshKey = "None";

    if (m_pRenderObj)
    {
        Ptr<CMesh> pMesh = m_pRenderObj->GetObj()->MeshRender()->GetMesh();
        
        strMeshKey = ToString(pMesh->GetKey()).c_str();

    }

    ImGui::Button("MeshKey");
    ImGui::SameLine();
    ImGui::Spacing();
    ImGui::SameLine();
    ImGui::Text(strMeshKey.c_str());

    string strFrmCnt = "frameCnt : ";
    UINT BoneCnt = 0;
    if (nullptr != m_pSelectedMeshData)
    {
        BoneCnt = m_pSelectedMeshData->GetMesh()->GetFrameCount();
    }
    strFrmCnt += std::to_string(BoneCnt);

    ImGui::Text(strFrmCnt.c_str());
    ImGui::Text("frame value2");
    ImGui::Text("frame value3");
    ImGui::Text("frame value4");
    ImGui::Text("frame value5");
    ImGui::EndGroup();
    //ImGui::EndChild();

    // 현재 애니메이션 정보
    //ImGui::BeginChild("##CurAnimInfo", ImVec2(200.f, 0.f), false, window_flags);
    ImGui::Button("CurAnimInfo", ImVec2(0, 0));
    ImGui::BeginGroup();
    ImGui::Text("frame value1");
    ImGui::Text("frame value2");
    ImGui::Text("frame value3");
    ImGui::Text("frame value4");
    ImGui::Text("frame value5");
    ImGui::EndGroup();
    //ImGui::EndChild();

    if (ImGui::Button("Play"))
    {
        m_pRenderObj->GetAnimator3D()->Play();
    }

    if (ImGui::Button("Stop"))
    {
        m_pRenderObj->GetAnimator3D()->Stop();
    }


    if (ImGui::Button("OpenMeshData"))
    {
        const map<wstring, Ptr<CRes>>& mapMeshData = CResMgr::GetInst()->GetResources(RES_TYPE::MESHDATA);

        ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
        pListUI->Reset("MeshData List", ImVec2(300.f, 500.f));
        for (const auto& pair : mapMeshData)
        {
            pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
        }

        pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&AnimEditUI::SelectMeshData);
    }
}

void AnimEditUI::render_TimeLine()
{
    ImGuiStyle& style = ImGui::GetStyle();
    float child_height = ImGui::GetTextLineHeight() + style.ScrollbarSize + style.WindowPadding.y * 2.0f;
    ImGuiWindowFlags child_flags = ImGuiWindowFlags_HorizontalScrollbar;
    ImGui::BeginChild("##AnimEditTimeLine", ImVec2(0, child_height), true, child_flags);
    
    int MaxFrame = 0; 
    int iCurFrame = 0; 
    
    if (m_pRenderObj)
    {
        MaxFrame = m_tMTCurAnimClip.iFrameLength;
        iCurFrame = m_pRenderObj->GetAnimator3D()->GetCurFrameIdx();
        
    }

    static float test = 1.f;
    
    for (int frame = 0; frame < MaxFrame; frame++)
    {
        if (frame > 0)
            ImGui::SameLine();
        
        if (frame == iCurFrame)
        {
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "  ");
            //   //  ImGui::SetScrollHereX(0.5f);    // half position
            ImGui::SetScrollHereX((float)iCurFrame / (float)MaxFrame);
        }
        else
        {
            ImGui::Text("  ");
        }
    }
    
    float scroll_x = ImGui::GetScrollX();
    float scroll_max_x = ImGui::GetScrollMaxX();

    // 좌표값을 프레임으로 변환
    float SliderWidth = ImGui::GetWindowWidth();
    ImGui::EndChild();

    scroll_x *= MaxFrame / scroll_max_x;
    scroll_max_x *= MaxFrame / scroll_max_x;

    ImGui::Text("%.0f/%.0f", scroll_x, scroll_max_x);
    ImGui::Spacing();
    
    if (m_pRenderObj && false == m_pRenderObj->GetAnimator3D()->IsPlay())
    {
        m_pRenderObj->GetAnimator3D()->SetFrame(scroll_x);
    }

    // ImGui::SetNextItemWidth(SliderWidth);
    // ImGui::SliderInt("Lines", &iCurFrame, 0, MaxFrame);
}

void AnimEditUI::render_CamController()
{
    ImGui::Button("Cam Contorller", ImVec2(0, 0));
}


void AnimEditUI::tick()
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    SetPopupPos(viewport->WorkPos);
    SetSize(viewport->WorkSize.x, viewport->WorkSize.y);
}

void AnimEditUI::finaltick()
{
    // 반투명 배경
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PopStyleVar();

    UI::finaltick();
}

int AnimEditUI::render_update()
{
    ImGui::BeginGroup();
    {
        // Mesh가 보유한 Clip Tree
        {
            ImGui::BeginGroup();

            render_cliplistwindow();

            ImGui::EndGroup();
        }
    }

    ImGui::SameLine();
    // Render Preview Window
    {
        ImGui::BeginGroup();
        {
            render_previewwindow();
        }
        ImGui::EndGroup();
    }

    ImGui::SameLine();
    // Anim Info Window
    {
        ImGui::BeginGroup();
        {
            render_infowindow();

            // Preview Cam Postion Controller
            render_CamController();
        }
        ImGui::EndGroup();
    }

    ImGui::EndGroup();

    // 타임라인 스크롤
    render_TimeLine();
    

    return 0;
}

void AnimEditUI::SelectMeshData(DWORD_PTR _data)
{
    string strKey = (char*)_data;

    Ptr<CMeshData> temp = CResMgr::GetInst()->FindRes<CMeshData>(wstring(strKey.begin(), strKey.end()));
    
    if (temp->IsHaveAnim())
    {
        m_pSelectedMeshData = temp;
        const vector<tMTAnimClip>* clips = m_pSelectedMeshData->GetMesh()->GetAnimClip();

        for (size_t i = 0; i < clips->size(); ++i)
        {
            tMTAnimClip tmp = clips->at(i);
            m_mapAnimClip.insert(make_pair(tmp.strAnimName, tmp));
        }

        if (m_pRenderObj)
        {
            delete m_pRenderObj;
            m_pRenderObj = nullptr;
        }

        if (nullptr == m_pRenderObj && nullptr != m_pSelectedMeshData)
        {
            m_pRenderObj = new CAnimEditObj;
            m_pRenderObj->setobject(m_pSelectedMeshData);

            CEditorObjMgr::GetInst()->SetTexRender(m_pRenderObj);
            m_pRenderObj->GetAnimator3D()->GetCurAnimClip(m_tMTCurAnimClip);
        }
    }
    else
    {
        wstring errMsg = L"This MeshData Have Not Animation.";
        MessageBox(nullptr, errMsg.c_str(), L"FAIL", MB_OK);
        return;
    }
}