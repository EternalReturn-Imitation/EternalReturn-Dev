#include "pch.h"
#include "AnimEditUI.h"

#include <Engine/CDevice.h>
#include <Engine/CStructuredBuffer.h>

#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include <Engine/CKeyMgr.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CMRT.h>

#include <Engine/CResMgr.h>
#include <Engine/CTexture.h>
#include <Engine/CMeshData.h>
#include <Engine/CMesh.h>
#include <Engine/CBone.h>

#include <Engine/CAnim3D.h>
#include <Engine/CAnimator3D.h>

#include "ListUI.h"

#include "CEditorObjMgr.h"
#include "CAnimEditObj.h"


AnimEditUI::AnimEditUI()
    : UI("##AnimEditUI")
    , m_pRenderObj(nullptr)
    , m_pCurAnimation(nullptr)
    , m_iFrameCount(0)
    , m_bPlay(true)
{
    SetName("AnimEditUI");

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    SetFlags(window_flags);
    SetModal(false);
    
}

AnimEditUI::~AnimEditUI()
{
}

void AnimEditUI::tick()
{
    if (m_pCurAnimation && KEY_TAP(KEY::SPACE))
    {
        if (m_bPlay)
        {
            m_pCurAnimation->Stop();
            m_bPlay = false;
        }
        else
        {
            m_pCurAnimation->Play();
            m_bPlay = true;
        }
    }

}

void AnimEditUI::finaltick()
{
    render_window();

    UI::finaltick();
}

int AnimEditUI::render_update()
{

    render_menubar();

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

void AnimEditUI::render_menubar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::MenuItem("Play"))
        {
            m_pCurAnimation->Play();
            m_bPlay = true;
        }

        if (ImGui::MenuItem("Stop"))
        {
            m_pCurAnimation->Stop();
            m_bPlay = false;
        }

        if (ImGui::BeginMenu("Set.."))
        {

            if (ImGui::MenuItem("New.."))
            {
                const map<wstring, Ptr<CRes>>& mapMesh = CResMgr::GetInst()->GetResources(RES_TYPE::MESHDATA);

                ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
                pListUI->Reset("MeshData List", ImVec2(300.f, 500.f));
                for (const auto& pair : mapMesh)
                {
                    pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
                }

                pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&AnimEditUI::SelectMeshData);
            }
            ImGui::Separator();

            if (ImGui::MenuItem("Add Bone.."))
            {
                const map<wstring, Ptr<CRes>>& mapBone = CResMgr::GetInst()->GetResources(RES_TYPE::BONE);

                ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
                pListUI->Reset("Animation List", ImVec2(300.f, 500.f));
                for (const auto& pair : mapBone)
                {
                    pListUI->AddItem(string(pair.first.begin(), pair.first.end()));
                }

                pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&AnimEditUI::SelectBone);
            }
            
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

void AnimEditUI::render_cliplistwindow()
{
    ImGui::Button("AnimClipList", ImVec2(200, 0));
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;

    ImVec2 ItemSize = ImGui::GetItemRectSize();

    map<wstring, CAnim3D*> mapAnims;
    if (m_pRenderObj)
    {
        mapAnims = m_pRenderObj->GetAnimator3D()->GetAnims();
    }

    int ClipCount = (UINT)mapAnims.size();
    int ClipIdx = 0;
    static int SelectClipIdx = 0;
    
    ImGui::BeginChild("##AnimClipList", ImVec2(200.f, 600.f - ItemSize.y), false, window_flags);;

    ImGuiTreeNodeFlags NodeFlag = ImGuiTreeNodeFlags_Leaf;
        
    string strFinalName;

    map<wstring, CAnim3D*>::iterator iter = mapAnims.begin();
    // 클립이 1개 이상인 경우 리스트 출력
    while (iter != mapAnims.end())
    {
        ClipIdx++;

        //strFinalName = ToString(iter->first) + "##";
        //char szBuff[100] = {};
        //_itoa_s(ClipIdx, szBuff, 10);
        //strFinalName += szBuff;
        
        const bool is_selected = (SelectClipIdx == ClipIdx);

        if (ImGui::Selectable(ToString(iter->first).c_str(), is_selected))
        {
            SelectClipIdx = ClipIdx;
            m_pRenderObj->GetAnimator3D()->SelectAnimation(iter->first);
            m_pCurAnimation = m_pRenderObj->GetAnimator3D()->GetCurAnim();
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
    // ==================== Set Print Info =======================
    // MeshData
    string  strMeshDataKey = "None"; // MeshData Key
    string  strMeshKey = "None";    // Mesh Key
    string  strMaterialKey = "None"; // 적용된 재질 이름
    string  strTextureKey = "None"; // 적용된 텍스쳐 이름

    int  AnimClipCnt = 0;   // 보유 클립 갯수
    int KeyFrameCount = 0;  // 전체 키프레임 갯수

    // CurAnimClip
    string  strCurClipName = "None";    // CurClip Anim Name
    int  startFrm = 0;
    int  EndFrm = 0;
    int  FrameLength = 0;

    double  StartTime = 0;
    double  EndTime = 0;
    double  TimeLength = 0;

    if (m_pRenderObj)
    {
        // Load Src
        Ptr<CMesh> pMesh = m_pRenderObj->GetObj()->MeshRender()->GetMesh();
        Ptr<CMaterial> pMaterial = m_pRenderObj->GetObj()->MeshRender()->GetMaterial(0);        // 메인 재질
        CAnimator3D* pAnimComp = m_pRenderObj->GetAnimator3D();
        
        strMeshKey = ToString(pMesh->GetKey()).c_str();
        strMaterialKey = ToString(pMaterial->GetKey()).c_str();
        strTextureKey = ToString(pMaterial->GetTexParam(TEX_PARAM::TEX_0)->GetKey()).c_str();   // 메인재질의 diff 텍스쳐 key
        AnimClipCnt = (UINT)pAnimComp->GetAnims().size();
    }


    if (m_pCurAnimation)
    {
        // SetInfo from Src
        KeyFrameCount = m_pCurAnimation->GetAnimClip().iFrameLength;
        
        strCurClipName = ToString(m_pCurAnimation->GetAnimClip().strAnimName);
        startFrm      = m_pCurAnimation->GetAnimClip().iStartFrame;
        EndFrm        = m_pCurAnimation->GetAnimClip().iEndFrame;
        FrameLength   = m_pCurAnimation->GetAnimClip().iFrameLength;
        StartTime     = m_pCurAnimation->GetAnimClip().dStartTime;
        EndTime       = m_pCurAnimation->GetAnimClip().dEndTime;
        TimeLength    = m_pCurAnimation->GetAnimClip().dTimeLength;
    }
    
    // ==================== Set Print Info =======================

    // MeshData Info
    ImGui::Button("MeshDataInfo", ImVec2(450, 0));
    ImGui::BeginGroup();
    
    print_strElement("   Mesh   Key", strMeshKey.c_str());
    print_strElement(" Material Key", strMaterialKey.c_str());
    print_strElement(" Texture  Key", strTextureKey.c_str());
    print_intElement(" AnimClip Cnt", AnimClipCnt);
    print_intElement("AllKeyFrm Cnt", KeyFrameCount);

    ImGui::EndGroup();

    // 현재 애니메이션 정보
    //ImGui::BeginChild("##CurAnimInfo", ImVec2(200.f, 0.f), false, window_flags);
    ImGui::Button("CurAnimInfo", ImVec2(450, 0));
    ImGui::BeginGroup();
    ImGui::Button("OriginInfo", ImVec2(220, 0));
    
    print_strElement("AnimClip Name", strCurClipName.c_str());
    print_intElement(" Start Frame ", startFrm);
    print_intElement("  End Frame  ", EndFrm);
    print_intElement(" FrameLength ", FrameLength);
    print_doubleElement(" Start  Time ", StartTime);
    print_doubleElement("  End   Time ", EndTime);
    print_doubleElement(" Time Length ", TimeLength);


    ImGui::EndGroup();

    //ImGui::EndChild();

}

void AnimEditUI::render_TimeLine()
{
    ImGuiStyle& style = ImGui::GetStyle();
    float child_height = ImGui::GetTextLineHeight() + style.ScrollbarSize + style.WindowPadding.y * 2.0f;
    ImGuiWindowFlags child_flags = ImGuiWindowFlags_HorizontalScrollbar;
    ImGui::BeginChild("##AnimEditTimeLine", ImVec2(0, child_height), true, child_flags);
    
    int MaxFrame = 0; 
    int iCurFrame = 0; 
    
    if (m_pCurAnimation)
    {
        MaxFrame = m_pCurAnimation->GetAnimClip().iFrameLength;
        iCurFrame = m_pCurAnimation->GetCurFrameIdx();
    }

    static float test = 1.f;
    
    // 좌표값을 프레임으로 변환
    float SliderWidth = ImGui::GetWindowWidth();
    ImGui::EndChild();

    ImGui::SetNextItemWidth(SliderWidth);
    ImGui::SliderInt("Lines", &iCurFrame, 0, MaxFrame);
    ImGui::Text("%.0f/%.0f", iCurFrame, MaxFrame);
    
    if (m_pCurAnimation && false == m_pRenderObj->GetAnimator3D()->IsPlay())
    {
        m_pRenderObj->GetAnimator3D()->SetFrame(iCurFrame);
    }
}

void AnimEditUI::render_CamController()
{
    ImGui::Button("Cam Contorller", ImVec2(0, 0));
}

void AnimEditUI::render_window()
{
    // 에디터 윈도우 세팅
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    }

    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    SetPopupPos(viewport->WorkPos);
    SetSize(viewport->WorkSize.x, viewport->WorkSize.y);

    // 반투명 배경
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PopStyleVar(3);
}

void AnimEditUI::SelectMeshData(DWORD_PTR _data)
{
    string strKey = (char*)_data;

    Ptr<CMeshData> temp = CResMgr::GetInst()->FindRes<CMeshData>(wstring(strKey.begin(), strKey.end()));

    if (nullptr != temp)
    {
        if (m_pRenderObj)
        {
            delete m_pRenderObj;
            m_pRenderObj = nullptr;
            m_pCurAnimation = nullptr;
        }

        if (nullptr == m_pRenderObj)
        {
            m_pRenderObj = new CAnimEditObj;
            m_pRenderObj->setobject(temp);

            CEditorObjMgr::GetInst()->SetTexRender(m_pRenderObj);
        }
    }
    else
    {
        wstring errMsg = L"This MeshData Select Fail.";
        MessageBox(nullptr, errMsg.c_str(), L"FAIL", MB_OK);
        return;
    }
}

void AnimEditUI::SelectBone(DWORD_PTR _data)
{
    string strKey = (char*)_data;

    Ptr<CBone> temp = CResMgr::GetInst()->FindRes<CBone>(wstring(strKey.begin(), strKey.end()));
    
    if (nullptr == m_pRenderObj)
    {
        wstring errMsg = L"No RenderObj. Select RenderObj First.";
        MessageBox(nullptr, errMsg.c_str(), L"FAIL", MB_OK);
        return;
    }

    if (nullptr != temp)
    {
        // 중복 검사
        CAnimator3D* pAnimComp = m_pRenderObj->GetAnimator3D();
        if (nullptr == pAnimComp->AddAnim(temp))
        {
            wstring errMsg = L"This Bone Already Exists.";
            MessageBox(nullptr, errMsg.c_str(), L"FAIL", MB_OK);
            return;
        }

        // 정상적으로 애니메이션이 추가된 경우 추가한 애니메이션으로 변경해준다.
        m_pCurAnimation = pAnimComp->SelectAnimation(temp->GetKey());
    }
    else
    {
        wstring errMsg = L"Can't Find Bond Data.";
        MessageBox(nullptr, errMsg.c_str(), L"FAIL", MB_OK);
        return;
    }
}

void AnimEditUI::print_strElement(const char* _BtnTitle, const char* _str, Vec2 _Btnsize)
{
    ImGui::Button(_BtnTitle);
    ImGui::SameLine();
    ImGui::Text(_str);
}

void AnimEditUI::print_intElement(const char* _BtnTitle, int& _int, Vec2 _Btnsize)
{
    ImGui::Button(_BtnTitle);
    ImGui::SameLine();
    ImGui::Text("%d", _int);
}

void AnimEditUI::print_doubleElement(const char* _BtnTitle, double& _double, Vec2 _Btnsize)
{
    ImGui::Button(_BtnTitle);
    ImGui::SameLine();
    ImGui::Text("%.2f", _double);
}
