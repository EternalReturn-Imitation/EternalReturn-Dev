#include "pch.h"
#include "BlackBoardListUI.h"

#include <Engine\CBehaviorTreeMgr.h>

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>

#include "ListUI.h"

BlackBoardListUI::BlackBoardListUI()
    : UI("##BlackBoardList")
    , m_pTargetBB(nullptr)
    , m_eFlags(BBListShowFlag_None)
    , m_bCreateDataWindow(false)
    , m_pCreateObject(nullptr)
{
	SetName("BlackBoardList");
}

BlackBoardListUI::~BlackBoardListUI()
{
}

void BlackBoardListUI::render_ManuBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Create New Data")) m_bCreateDataWindow = true;
                
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void BlackBoardListUI::render_TabBar(ImGuiTabBarFlags _flags)
{
    if (ImGui::BeginTabBar("MyTabBar", _flags))
    {
        if (ImGui::BeginTabItem("All"))
        {
            m_eFlags = BBListShowFlag_All;
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("int"))
        {
            m_eFlags = BBListShowFlag_int;
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("float"))
        {
            m_eFlags = BBListShowFlag_float;
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("GameObject"))
        {
            m_eFlags = BBListShowFlag_GameObject;
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("string"))
        {
            m_eFlags = BBListShowFlag_string;
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("wstring"))
        {
            m_eFlags = BBListShowFlag_wstring;
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

void BlackBoardListUI::render_Table(ImGuiTableFlags _flags)
{
    const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;

    if (ImGui::BeginTable("BBTable", 3, _flags))
    {
        ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_NoHide);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 12.0f);
        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed, TEXT_BASE_WIDTH * 18.0f);
        ImGui::TableHeadersRow();

        const char* DataKey = nullptr;
        const char* DataType = nullptr;
        string DataValue;

        unordered_map<string, BB::tBBData*> BBList = m_pTargetBB->GetBBList();

        for (auto iter : BBList)
        {
            DataKey = iter.second->strKey.c_str();
            DataType = iter.second->strDataType;

            if (IsShowType(DataType))
            {
                DataValue = GetDataStr(iter.second, DataType);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();

                ImGui::TreeNodeEx(DataKey, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);

                if (ImGui::BeginPopupContextItem())
                {
                    if (ImGui::MenuItem("Delete"))
                    {
                        m_pTargetBB->DeleteBBData(DataKey);
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Open Node");

                ImGui::TableNextColumn();
                ImGui::Text(DataType);
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(DataValue.c_str());
            }
        }
        ImGui::EndTable();
    }
}

void BlackBoardListUI::render_CreateDataWindow()
{
    if (m_pTargetBB == NULL || m_bCreateDataWindow == false)
    {
        m_bCreateDataWindow = false;
        return;
    }

    // ImGui::OpenPopup("CreateDataWindow");
    if (ImGui::Begin("CreateDataWindow", NULL, ImGuiWindowFlags_Modal | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking))
    {
        ImGui::BeginGroup();
        ImGui::Button("Type##ButtonTitle");
        ImGui::SameLine();

        static const char* TypeList[5] = { "int","float","GameObject","string","wstring" };
        static int CurrentItem = 0;

        static int Value_int = 0;
        static float Value_float = 0.f;
        static char stringtmp[32] = {};
        
        ImGui::Combo("##CurrentDataType", &CurrentItem, TypeList, 5);

        // ImGui::InputText("##stringData", Key.c_str(), IM_ARRAYSIZE(Key));
        static char InputBBKey[32] = "New Data Key";
        ImGui::Button("Key##ButtonTitle");
        ImGui::SameLine();
        ImGui::InputText("##NewBBKey", InputBBKey, ImGuiInputTextFlags_EscapeClearsAll);

        ImGui::Button(TypeList[CurrentItem]);
        ImGui::SameLine();

        switch (CurrentItem)
        {
        case 0:
            ImGui::InputInt("##IntData", &Value_int, 1);
            break;
        case 1:
            ImGui::InputFloat("##FloatData", &Value_float);
            break;
        case 2:
        {
            static string CurObjName;
            ImGui::Text(" : ");
            ImGui::SameLine();

            if (m_pCreateObject != nullptr)
            {
                CurObjName.assign(m_pCreateObject->GetName().begin(), m_pCreateObject->GetName().end());
            }
            else
            {
                CurObjName = "empty";
            }

            ImGui::Text(CurObjName.c_str());
            
            ImGui::SameLine();
            
            if (ImGui::Button("...##OpenCurLevelObjList"))
            {
                m_mapGameObject.clear();

                CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

                ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
                pListUI->Reset("GameObject List", ImVec2(300.f, 500.f));

                for (UINT i = 0; i < (UINT)MAX_LAYER; ++i)
                {
                    CLayer* pLayer = pCurLevel->GetLayer(i);

                    const vector<CGameObject*>& vecParentObj = pLayer->GetParentObject();

                    for (size_t i = 0; i < vecParentObj.size(); ++i)
                    {
                        wstring tmp = vecParentObj[i]->GetName();
                        pListUI->AddItem(string(tmp.begin(), tmp.end()));
                        m_mapGameObject.insert(make_pair(string(tmp.begin(), tmp.end()), vecParentObj[i]));
                    }
                }

                // 항목 선택시 호출받을 델리게이트 등록
                pListUI->AddDynamic_Select(this, (UI_DELEGATE_1)&BlackBoardListUI::SelectObject);
            };
            
        }
            break;
        case 3:
            ImGui::InputText("##stringData", stringtmp, IM_ARRAYSIZE(stringtmp));
            break;
        case 4:
            ImGui::InputText("##stringData", stringtmp, IM_ARRAYSIZE(stringtmp));
            break;
        }
        ImGui::EndGroup();

        if (ImGui::Button("Create##BBDataCreateBtn"))
        {
            switch (CurrentItem)
            {
            case 0:
                m_pTargetBB->AddBBData(InputBBKey, Value_int);
                break;
            case 1:
                m_pTargetBB->AddBBData(InputBBKey, Value_float);
                break;
            case 2:
                m_pTargetBB->AddBBData(InputBBKey, m_pCreateObject);
                break;
            case 3:
                m_pTargetBB->AddBBData(InputBBKey, stringtmp);
                break;
            case 4:
                string stmp = stringtmp;
                wstring wtmp;
                m_pTargetBB->AddBBData(InputBBKey, wtmp.assign(stmp.begin(), stmp.end()));
                break;
            }

            CurrentItem = 0;
            strcpy_s(InputBBKey, "New Data Key");
            Value_int = 0;
            Value_float = 0.f;
            static CGameObject* Value_GOPtr = nullptr;
            m_pCreateObject = nullptr;
            m_mapGameObject.clear();
            
            m_bCreateDataWindow = false;
            ImGui::CloseCurrentPopup();
        };
        ImGui::SameLine();

        if (ImGui::Button("Close##CreateWinCloseBtn"))
        {
            CurrentItem = 0;
            strcpy_s(InputBBKey, "New Data Key");
            Value_int = 0;
            Value_float = 0.f;
            m_pCreateObject = nullptr;
            m_mapGameObject.clear();

            m_bCreateDataWindow = false;
            ImGui::CloseCurrentPopup();
        };
        ImGui::End();

        // ImGui::EndPopup();
    }
}

void BlackBoardListUI::SelectObject(DWORD_PTR _Key)
{
    m_pCreateObject = m_mapGameObject.find((char*)_Key)->second;
}

void BlackBoardListUI::init()
{
	SetBlackBoard(nullptr);
}

void BlackBoardListUI::tick()
{
}

int BlackBoardListUI::render_update()
{
    // 메뉴
    render_ManuBar();

    // 메뉴바 메뉴
    render_CreateDataWindow();

    // BB 타겟설정이 되어있지 않은 경우
    if (m_pTargetBB == nullptr)
        return 0;

    // 테이블 플래그
    static ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    static ImGuiTableFlags Tableflags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;
    
    // 탭 바
    render_TabBar(tab_bar_flags);

    // 테이블
    render_Table(Tableflags);

    return 0;
}

void BlackBoardListUI::SetBlackBoard(BB* _Target)
{
	m_pTargetBB = _Target;
}

string BlackBoardListUI::GetDataStr(BB::tBBData* _Data, const char* _DataType)
{
    string res;

    if (_DataType == "int")
    {
        res = std::to_string(*(int*)_Data->pDataPtr);
    }
    else if (_DataType == "float")
    {
        char fstr[10] = {};
        float f = *((float*)_Data->pDataPtr);

        sprintf_s(fstr, "%.3f", f);
        res = fstr;
    }
    else if (_DataType == "GameObject")
    {
        if (_Data->pDataPtr == 0)
        {
            res = "nullptr";
        }
        else
        {
            wstring ObjName = ((CGameObject*)_Data->pDataPtr)->GetName();
            res.assign(ObjName.begin(), ObjName.end());
        }
    }
    else if (_DataType == "string")
    {
        res = _Data->strData;
    }
    else if (_DataType == "wstring")
    {
        res = _Data->strData;
    }

    return res;
}

bool BlackBoardListUI::IsShowType(const char* _type)
{
    if (_type == "int" && m_eFlags & BBListShowFlag_int)
        return true;
    else if (_type == "float" && m_eFlags & BBListShowFlag_float)
        return true;
    else if (_type == "GameObject" && m_eFlags & BBListShowFlag_GameObject)
        return true;
    else if (_type == "string" && m_eFlags & BBListShowFlag_string)
        return true;
    else if (_type == "wstring" && m_eFlags & BBListShowFlag_wstring)
        return true;

    return false;
}

