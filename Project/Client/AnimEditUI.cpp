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
    , m_pCurAnimator(nullptr)
    , m_iFrameCount(0)
    , m_bPlay(true)
{
    SetName("AnimEditUI");

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    // window_flags |= ImGuiWindowFlags_NoBackground;

    SetFlags(window_flags);
    SetModal(false);
    
}

AnimEditUI::~AnimEditUI()
{
}

void AnimEditUI::tick()
{
    if (m_pCurAnimator && KEY_TAP(KEY::SPACE))
    {
        if (m_bPlay)
        {
            m_pCurAnimator->Stop();
            m_bPlay = false;
        }
        else
        {
            m_pCurAnimator->Play();
            m_bPlay = true;
        }
    }

}

void AnimEditUI::finaltick()
{
    ImGui::SetNextWindowSize(ImVec2(1280.f, 780.f));
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
            // render_CamController();
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
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save.."))
            {
                //  ER_ItemMgr::GetInst()->Save();
            }

            if (ImGui::MenuItem("Exit"))
                SetActive(false);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Control"))
        {
            if (ImGui::MenuItem("Play"))
            {
                if (m_pCurAnimator)
                {
                    m_pCurAnimator->Play();
                }
                m_bPlay = true;
            }

            if (ImGui::MenuItem("Stop"))
            {
                if (m_pCurAnimator)
                {
                    m_pCurAnimator->Stop();
                }
                m_bPlay = false;
            }

            ImGui::EndMenu();
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
    ImGui::Button("AnimClipList", ImVec2(300, 0));
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;

    ImVec2 ItemSize = ImGui::GetItemRectSize();

    map<wstring, CAnim3D*> mapAnims;
    if (m_pRenderObj)
    {
        mapAnims = m_pCurAnimator->GetAnims();
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
            // 새로운 클립 선택
            if (SelectClipIdx != ClipIdx)
            {
                SelectClipIdx = ClipIdx;

                // 기존 애니메이션이 있었는지
                CAnim3D* pPreviousAnim = m_pCurAnimator->GetCurAnim();

                if (pPreviousAnim)
                    pPreviousAnim->SetAnimClip(m_tTempAnimclip);

                // 정상적으로 애니메이션이 추가된 경우 추가한 애니메이션으로 변경해준다.
                m_pCurAnimator->SelectAnimation(iter->first, true);
                m_tTempAnimclip = m_pCurAnimator->GetCurAnim()->GetAnimClip();

                m_bPlay = true;
            }
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

    int     AnimClipCnt = 0;   // 보유 클립 갯수

    // CurAnimClip
    CAnim3D* pCurAnimation = nullptr;

    string  Origin_strCurClipName = "None";    // CurClip Anim Name
    int  Origin_startFrm = 0;
    int  Origin_EndFrm = 0;
    int  Origin_FrameLength = 0;

    double  Origin_StartTime = 0;
    double  Origin_EndTime = 0;
    double  Origin_TimeLength = 0;

   

    if (m_pRenderObj)
    {
        // Load Src
        Ptr<CMesh> pMesh = m_pRenderObj->GetObj()->MeshRender()->GetMesh();
        Ptr<CMaterial> pMaterial = m_pRenderObj->GetObj()->MeshRender()->GetMaterial(0);        // 메인 재질
        
        strMeshKey = ToString(pMesh->GetKey()).c_str();
        strMaterialKey = ToString(pMaterial->GetKey()).c_str();
        strTextureKey = ToString(pMaterial->GetTexParam(TEX_PARAM::TEX_0)->GetKey()).c_str();   // 메인재질의 diff 텍스쳐 key
        AnimClipCnt = (UINT)m_pCurAnimator->GetAnims().size();

        pCurAnimation = m_pCurAnimator->GetCurAnim();
    }

    string Modify_strCurClipName = "None";    // CurClip Anim Name
    int  Modify_startFrm = 0;
    int  Modify_EndFrm = 0;
    int  Modify_FrameLength = 0;

    float  Modify_StartTime = 0;
    float  Modify_EndTime = 0;
    float  Modify_TimeLength = 0;

    if (pCurAnimation)
    {
        tMTAnimClip tAnimClip = pCurAnimation->GetAnimClip();

        // Origin Anim Info
        
        Origin_strCurClipName   = ToString(m_tTempAnimclip.strAnimName);
        Origin_startFrm         = m_tTempAnimclip.iStartFrame;
        Origin_EndFrm           = m_tTempAnimclip.iEndFrame;
        Origin_FrameLength      = m_tTempAnimclip.iFrameLength;
        Origin_StartTime        = m_tTempAnimclip.dStartTime;
        Origin_EndTime          = m_tTempAnimclip.dEndTime;
        Origin_TimeLength       = m_tTempAnimclip.dTimeLength;

        // Modifiy Anim Info
        Modify_strCurClipName = ToString(tAnimClip.strAnimName).c_str();
        Modify_startFrm = tAnimClip.iStartFrame;
        Modify_EndFrm = tAnimClip.iEndFrame;
        Modify_FrameLength = tAnimClip.iFrameLength;
        Modify_StartTime = tAnimClip.dStartTime;
        Modify_EndTime = tAnimClip.dEndTime;
        Modify_TimeLength = tAnimClip.dTimeLength;
    }
    
    // ==================== Set Print Info =======================

    // MeshData Info
    ImGui::Button("MeshDataInfo", ImVec2(350, 0));
    ImGui::BeginGroup();
    
    print_strElement("   Mesh   Key", strMeshKey.c_str());
    print_strElement(" Material Key", strMaterialKey.c_str());
    print_strElement(" Texture  Key", strTextureKey.c_str());
    print_intElement(" AnimClip Cnt", AnimClipCnt);

    ImGui::EndGroup();

    // 현재 애니메이션 정보
    //ImGui::BeginChild("##CurAnimInfo", ImVec2(200.f, 0.f), false, window_flags);
    ImGui::Button("CurAnimInfo", ImVec2(350, 0));
    ImGui::BeginGroup();
    ImGui::Button("OriginInfo", ImVec2(350, 0));
    
    print_strElement("AnimClip Name", Origin_strCurClipName.c_str());
    print_intElement(" Start Frame ", Origin_startFrm);
    print_intElement("  End Frame  ", Origin_EndFrm);
    print_intElement(" FrameLength ", Origin_FrameLength);
    print_doubleElement(" Start  Time ", Origin_StartTime);
    print_doubleElement("  End   Time ", Origin_EndTime);
    print_doubleElement(" Time Length ", Origin_TimeLength);
    
    ImGui::EndGroup();

    {
        ImGui::BeginGroup();
        ImGui::Button("ModifyInfo", ImVec2(350, 0));

        ImGui::Button("AnimClip Name");
        ImGui::SameLine();
        
        char tmp[50] = {};
        strcpy_s(tmp, Modify_strCurClipName.c_str());
        ImGui::InputText("##AnimClipName", tmp, 50);

        print_intElement(" Start Frame ", Origin_startFrm);

        // ImGui::Button(" Start Frame ");
        // ImGui::SameLine();
        // ImGui::SetNextItemWidth(240);
        // if (ImGui::InputInt("##Modify_startFrm", &Modify_startFrm))
        // {
        //     Modify_StartTime = Modify_startFrm * Modify_TimeLength / Modify_FrameLength;
        // }
        
        ImGui::Button("  End Frame  ");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(240);
        if (ImGui::InputInt("##Modify_EndFrm", &Modify_EndFrm))
        {
            Modify_EndFrm = Modify_FrameLength < Modify_EndFrm ? Modify_FrameLength : Modify_EndFrm;
            Modify_EndTime = Modify_EndFrm * (Origin_TimeLength / Origin_FrameLength);
        }

        print_intElement(" FrameLength ", Modify_FrameLength);

        print_doubleElement(" Start  Time ", Origin_StartTime);

        // ImGui::Button(" Start  Time ");
        // ImGui::SameLine();
        // ImGui::SetNextItemWidth(240);
        // if (ImGui::InputDouble("##Modify_StartTime", &Modify_StartTime, 0.01f, 1.0f, "%.3f"))
        // {
        //     Modify_startFrm = Modify_StartTime * Modify_FrameLength / Modify_TimeLength;
        // }

        print_floatElement("  End   Time ", Modify_EndTime);

        // ImGui::Button("  End   Time ");
        // ImGui::SameLine();
        // ImGui::SetNextItemWidth(240);
        // if (ImGui::InputDouble("##Modify_EndTime", &Modify_EndTime, 0.01f, 1.0f,"%.3f"))
        // {
        //     Modify_EndTime = Modify_TimeLength < Modify_EndTime ? Modify_TimeLength : Modify_EndTime;
        //     Modify_EndFrm = Modify_EndTime * Modify_FrameLength / Modify_TimeLength;
        // }

        print_floatElement(" Time Length ", Modify_TimeLength);
    }

    if (ImGui::Button("Save Clip.."))
    {
        if(nullptr != m_pCurAnimator && m_pCurAnimator->GetCurAnim())
            ImGui::OpenPopup("Save##SaveAnimClip");
    }

    render_SavePopUp();

    if (pCurAnimation)
    {
        // Modifiy Anim Info
        tMTAnimClip AnimClip = pCurAnimation->GetAnimClip();
        
        string strName = Modify_strCurClipName;
    
        AnimClip.strAnimName = ToWString(strName);
        AnimClip.iStartFrame = Modify_startFrm;
        AnimClip.iEndFrame = Modify_EndFrm;
        AnimClip.iFrameLength = Modify_FrameLength;
        AnimClip.dStartTime = Modify_StartTime;
        AnimClip.dEndTime = Modify_EndTime;
        AnimClip.dTimeLength = Modify_TimeLength;
    
        pCurAnimation->SetAnimClip(AnimClip);
    }

    ImGui::EndGroup();

    //ImGui::EndChild();
}

void AnimEditUI::render_TimeLine()
{
    int MaxFrame = 0; 
    int iCurFrame = 0; 

    float TimeLength = 0.f;
    float fCurTime = 0.f;
    
    CAnim3D* pCurAnim = nullptr;
    if(m_pCurAnimator)
        pCurAnim = m_pCurAnimator->GetCurAnim();

    if (pCurAnim)
    {
        MaxFrame = pCurAnim->GetAnimClip().iEndFrame;
        iCurFrame = m_pCurAnimator->GetCurFrame();
        TimeLength = (float)pCurAnim->GetAnimClip().dEndTime;
        fCurTime = (float)pCurAnim->GetAnimClip().fUpdateTime;
    }

    ImGui::SetNextItemWidth(ImGui::GetWindowWidth());
    ImGui::SliderInt("Lines", &iCurFrame, 0, MaxFrame);
    
    ImGui::Text("CurFrame : %d / %d, CurTime : %.2f, %.2f", iCurFrame, MaxFrame, fCurTime, TimeLength);

    if (pCurAnim && false == m_pCurAnimator->IsPlay())
    {
        m_pCurAnimator->SetFrame(iCurFrame);
    }
}

void AnimEditUI::render_CamController()
{
    ImGui::Button("Cam Contorller", ImVec2(0, 0));
}

void AnimEditUI::render_SavePopUp()
{
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Save##SaveAnimClip", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Change List\n\n");
        ImGui::Separator();

        tMTAnimClip tAnimClip = m_pCurAnimator->GetCurAnim()->GetAnimClip();

        int ChangInfoCnt = 0;

        if (m_tTempAnimclip.strAnimName != tAnimClip.strAnimName)
        {
            ImGui::Text(ToString(m_tTempAnimclip.strAnimName).c_str());
            ImGui::SameLine();
            ImGui::Text(" -> ");
            ImGui::SameLine();
            ImGui::Text(ToString(tAnimClip.strAnimName).c_str());

            ChangInfoCnt++;
        }
        if (m_tTempAnimclip.iStartFrame != tAnimClip.iStartFrame)
        {
            ImGui::Text("StartFrm : ");
            ImGui::SameLine();
            ImGui::Text(std::to_string(m_tTempAnimclip.iStartFrame).c_str());
            ImGui::SameLine();
            ImGui::Text(" -> ");
            ImGui::SameLine();
            ImGui::Text(std::to_string(tAnimClip.iStartFrame).c_str());

            ChangInfoCnt++;
        }
        if (m_tTempAnimclip.iEndFrame != tAnimClip.iEndFrame)
        {
            ImGui::Text("EndFrm : ");
            ImGui::SameLine();
            ImGui::Text(std::to_string(m_tTempAnimclip.iEndFrame).c_str());
            ImGui::SameLine();
            ImGui::Text(" -> ");
            ImGui::SameLine();
            ImGui::Text(std::to_string(tAnimClip.iEndFrame).c_str());
            ChangInfoCnt++;
        }
        if (m_tTempAnimclip.iFrameLength != tAnimClip.iFrameLength)
        {
            ImGui::Text("FrmLength : ");
            ImGui::SameLine();
            ImGui::Text(std::to_string(m_tTempAnimclip.iFrameLength).c_str());
            ImGui::SameLine();
            ImGui::Text(" -> ");
            ImGui::SameLine();
            ImGui::Text(std::to_string(tAnimClip.iFrameLength).c_str());
            ChangInfoCnt++;
        }
        if (m_tTempAnimclip.dStartTime != tAnimClip.dStartTime)
        {
            ImGui::Text("StartTime : ");
            ImGui::SameLine();
            ImGui::Text(std::to_string(m_tTempAnimclip.dStartTime).c_str());
            ImGui::SameLine();
            ImGui::Text(" -> ");
            ImGui::SameLine();
            ImGui::Text(std::to_string(tAnimClip.dStartTime).c_str());
            ChangInfoCnt++;
        }
        if (m_tTempAnimclip.dEndTime != tAnimClip.dEndTime)
        {
            ImGui::Text("EndTime : ");
            ImGui::SameLine();
            ImGui::Text(std::to_string(m_tTempAnimclip.dEndTime).c_str());
            ImGui::SameLine();
            ImGui::Text(" -> ");
            ImGui::SameLine();
            ImGui::Text(std::to_string(tAnimClip.dEndTime).c_str());
            ChangInfoCnt++;
        }
        if (m_tTempAnimclip.dTimeLength != tAnimClip.dTimeLength)
        {
            ImGui::Text("TimeLength : ");
            ImGui::SameLine();
            ImGui::Text(std::to_string(m_tTempAnimclip.dTimeLength).c_str());
            ImGui::SameLine();
            ImGui::Text(" -> ");
            ImGui::SameLine();
            ImGui::Text(std::to_string(tAnimClip.dTimeLength).c_str());
            ChangInfoCnt++;
        }

        ImGui::Spacing();

        if (ChangInfoCnt == 0)
        {
            ImGui::SameLine();
            ImGui::Text("  No Change  ");

            if (ImGui::Button(" Close ", ImVec2(120, 0)))
            {
                ImGui::CloseCurrentPopup();
            }
        }
        else
        {
            if (ImGui::Button(" Save ", ImVec2(120, 0)))
            {
                CBone* pBone = m_pCurAnimator->GetCurAnim()->GetBone().Get();

                pBone->SetAnimClip(tAnimClip);
                pBone->Save(pBone->GetKey());

                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::SameLine();
            if (ImGui::Button(" Cancel ", ImVec2(120, 0)))
            {
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }
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
            m_pCurAnimator = nullptr;
        }

        if (nullptr == m_pRenderObj)
        {
            m_pRenderObj = new CAnimEditObj;
            m_pRenderObj->setobject(temp);
            m_pCurAnimator = m_pRenderObj->GetAnimator3D();

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
        if (nullptr == m_pCurAnimator->AddAnim(temp))
        {
            wstring errMsg = L"This Bone Already Exists.";
            MessageBox(nullptr, errMsg.c_str(), L"FAIL", MB_OK);
            return;
        }

        // temp animClip 
        // 기존 애니메이션이 있었는지
        CAnim3D* pPreviousAnim = m_pCurAnimator->GetCurAnim();
        
        if (pPreviousAnim)
            pPreviousAnim->SetAnimClip(m_tTempAnimclip);

        // 정상적으로 애니메이션이 추가된 경우 추가한 애니메이션으로 변경해준다.
        m_pCurAnimator->SelectAnimation(temp->GetKey(), true);
        m_tTempAnimclip = m_pCurAnimator->GetCurAnim()->GetAnimClip();
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

void AnimEditUI::print_floatElement(const char* _BtnTitle, float& _float, Vec2 _Btnsize)
{
    ImGui::Button(_BtnTitle);
    ImGui::SameLine();
    ImGui::Text("%.2f", _float);
}

void AnimEditUI::print_doubleElement(const char* _BtnTitle, double& _double, Vec2 _Btnsize)
{
    ImGui::Button(_BtnTitle);
    ImGui::SameLine();
    ImGui::Text("%.2f", _double);
}
