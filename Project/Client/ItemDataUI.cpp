#include "pch.h"
#include "ItemDataUI.h"

#include <Engine/CResMgr.h>
#include <Engine/CTexture.h>

#include "ListUI.h"

enum MyItemColumnID
{
    ItemDataListColumnID_ID,
    ItemDataListColumnID_Texture,
    ItemDataListColumnID_Name,
    ItemDataListColumnID_Grade,
    ItemDataListColumnID_Type,
    ItemDataListColumnID_Slot,
    ItemDataListColumnID_Stats
};

ItemDataUI::ItemDataUI()
    : UI("##ItemDataUI")
    , m_bOpenTab{ true,true }
    , m_pCurItem(nullptr)
{
    SetName("ItemDataUI");

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    SetFlags(window_flags);
    SetModal(false);

    m_bTabName[(UINT)ItemDataUItab::ITEMDATA] = "ItemDataList";
    m_bTabName[(UINT)ItemDataUItab::RECIPE] = "ItemRecipeList";

    ItemDataUIFlags =
        ImGuiTableFlags_Resizable |         // Size 조정
        ImGuiTableFlags_Reorderable |       // 순서 변경
        //ImGuiTableFlags_Sortable |           // Item 정렬
        ImGuiTableFlags_Borders |
        ImGuiTableFlags_ScrollX |
        ImGuiSliderFlags_NoInput;

    m_vecItem = &ER_ItemMgr::GetInst()->m_vecItem;
    m_umapRecipe = &ER_ItemMgr::GetInst()->m_umapRecipe;
}

ItemDataUI::~ItemDataUI()
{
    m_vecItem = nullptr;
    m_umapRecipe = nullptr;
}

void ItemDataUI::init()
{
    m_pEmptyItemSlotTex = CResMgr::GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_Empty.png");
}

void ItemDataUI::tick()
{
}

void ItemDataUI::finaltick()
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

    UI::finaltick();
}

int ItemDataUI::render_update()
{
    render_menubar();

    render_Tabs();

    return 0;
}

void ItemDataUI::render_menubar()
{
    bool Active = IsActive();

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save.."))
            {
                ER_ItemMgr::GetInst()->Save();
            }

            if (ImGui::MenuItem("Exit") && Active)
                SetActive(false);

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void ItemDataUI::render_Tabs()
{
    if (ImGui::BeginTabBar("##ItemDatatabs", ImGuiTabBarFlags_FittingPolicyDefault_))
    {
        for (int tab_n = 0; tab_n < (UINT)ItemDataUItab::END; ++tab_n)
        {
            // Submit Tabs

            bool visible = ImGui::BeginTabItem(m_bTabName[tab_n].c_str(), &m_bOpenTab[tab_n], 0);

            // Cancel attempt to close when unsaved add to save queue so we can display a popup.

            if ((UINT)ItemDataUItab::ITEMDATA == tab_n && visible)
            {
                render_ItemInfoTable();
            }
            else if ((UINT)ItemDataUItab::RECIPE == tab_n && visible)
            {
                render_ItemRecipeTable();
            }
            
            if(visible)
                ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
}

void ItemDataUI::render_ItemInfoTable()
{
    static ImVector<int> selection;

    if (ImGui::BeginTable("##ItemDataList", 7, ItemDataUIFlags, ImVec2(0, 0)))
    {
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHide | ImGuiTableColumnFlags_NoReorder, 0.0f, ItemDataListColumnID_ID);
        ImGui::TableSetupColumn("Texture", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoReorder, 0.0f, ItemDataListColumnID_Texture);
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoReorder, 0.0f, ItemDataListColumnID_Name);
        ImGui::TableSetupColumn("Grade", ImGuiTableColumnFlags_None | ImGuiTableColumnFlags_WidthFixed, 0.0f, ItemDataListColumnID_Grade);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_None | ImGuiTableColumnFlags_WidthFixed, 0.0f, ItemDataListColumnID_Type);
        ImGui::TableSetupColumn("Slot", ImGuiTableColumnFlags_None | ImGuiTableColumnFlags_WidthFixed, 0.0f, ItemDataListColumnID_Slot);
        ImGui::TableSetupColumn("Stats", ImGuiTableColumnFlags_None | ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoSort, 0.0f, ItemDataListColumnID_Stats);
        ImGui::TableSetupScrollFreeze(1, 1);

        ImGui::TableHeadersRow();

        ImGui::PushButtonRepeat(true);

        // Show data

        // Demonstrate using clipper for large vertical lists
        ImGuiListClipper clipper;
        clipper.Begin((*m_vecItem).size());
        while (clipper.Step())
        {
            for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
                // Without clipper
            {

                m_pCurItem = (*m_vecItem)[row_n];

                const bool item_is_selected = selection.contains(m_pCurItem->GetCode());
                ImGui::PushID(m_pCurItem->GetCode());
                ImGui::TableNextRow(ImGuiTableRowFlags_None, 0.0f);

                // For the demo purpose we can select among different type of items submitted in the first column
                ImGui::TableSetColumnIndex((UINT)ItemDataListColumnID_ID);
                ImGui::PushItemWidth(-FLT_MIN);
                int CurItemCode = m_pCurItem->GetCode();
                ImGui::Text("%d", CurItemCode);

                // Item Icon
                ImGui::TableSetColumnIndex((UINT)ItemDataListColumnID_Texture);
                ImGui::PushItemWidth(-FLT_MIN);
                Ptr<CTexture> pTex = (CTexture*)m_pCurItem->GetItemTex().Get();
                
                ImVec2 size = ImVec2(66, 41);
                ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
                ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
                ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
                ImVec4 border_col = ImVec4(0.0f, 0.0f, 0.0f, 0.5f); // 50% opaque white

                ImTextureID TextureID;

                if (nullptr != pTex)
                    TextureID = (ImTextureID)pTex->GetSRV().Get();
                else
                    TextureID = (ImTextureID)m_pEmptyItemSlotTex->GetSRV().Get();

                if (ImGui::ImageButton("##ItemTexIcon", TextureID, size, uv_min, uv_max, border_col, tint_col))
                {
                    const map<wstring, Ptr<CRes>>& mapMesh = CResMgr::GetInst()->GetResources(RES_TYPE::TEXTURE);

                    ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
                    pListUI->Reset("ItemIcon List", ImVec2(300.f, 500.f));
                    
                    wstring KeyItemIcon = L"ItemIcon";
                    int KeyLen = KeyItemIcon.length();

                    for (const auto& pair : mapMesh)
                    {
                        if(KeyItemIcon == pair.first.substr(0, KeyLen))
                            pListUI->AddItem(ToString(pair.first));
                    }

                    pListUI->AddDynamic_Select_with_target(this, (UI_DELEGATE_2)&ItemDataUI::SelectItemIcon,(DWORD_PTR)m_pCurItem);
                }

                // Item Name
                ImGui::TableSetColumnIndex((UINT)ItemDataListColumnID_Name);
                ImGui::PushItemWidth(-FLT_MIN);

                // wchar_t -> UTF-8
                std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
                char szbuffer[64] = {};
                strcpy(szbuffer, converter.to_bytes(m_pCurItem->GetItemName()).c_str());

                ImGui::InputText("##ItemName", szbuffer, sizeof(szbuffer));

                // UTF-8 ->wchar_t
                std::string utf8String = szbuffer;
                std::wstring ItemName = converter.from_bytes(utf8String);

                if (m_pCurItem->GetItemName() != ItemName)
                    m_pCurItem->SetItemName(ItemName);

                // Item Grade
                ImGui::TableSetColumnIndex((UINT)ItemDataListColumnID_Grade);
                ImGui::PushItemWidth(-FLT_MIN);
                const char* Grade[] = { u8"미지정",u8"일반",u8"고급",u8"희귀",u8"영웅" };
                int CurItemGrade = m_pCurItem->GetGrade();
                ImGui::Combo("##ItemGrade", &CurItemGrade, Grade, 5);
                m_pCurItem->SetItemGrade(CurItemGrade);

                // Item Type
                ImGui::TableSetColumnIndex((UINT)ItemDataListColumnID_Type);
                ImGui::PushItemWidth(-FLT_MIN);
                const char* Types[] = { u8"미지정",u8"장비",u8"소비",u8"재료" };
                int CurItemType = m_pCurItem->GetType();
                ImGui::Combo("##ItemType", &CurItemType, Types, 4);
                m_pCurItem->SetItemType(CurItemType);

                // Item Slot
                ImGui::TableSetColumnIndex((UINT)ItemDataListColumnID_Slot);
                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::BeginDisabled((UINT)ER_ITEM_TYPE::EQUIPMENT != CurItemType);
                const char* Slots[] = { u8"장착불가",u8"무기",u8"머리",u8"옷",u8"팔",u8"다리",u8"미지정" };
                int CurItemSlot = m_pCurItem->GetSlot();
                ImGui::Combo("##ItemSlot", &CurItemSlot, Slots, 7);
                m_pCurItem->SetItemSlot(CurItemSlot);
                ImGui::EndDisabled();

                // if(ImGuiSetColumnIndex((UINT)ItemDataListColumnID_Stats))



                ImGui::PopID();
            }
        }
        ImGui::PopButtonRepeat();

        m_pCurItem = nullptr;

        ImGui::EndTable();
    }
}

void ItemDataUI::render_ItemRecipeTable()
{
    ImGui::Text("Recipe");
}

void ItemDataUI::SelectItemIcon(DWORD_PTR _data, DWORD_PTR _target)
{
    string strKey = (char*)_data;
    ER_Item* target = (ER_Item*)_target;

    Ptr<CTexture> tex = CResMgr::GetInst()->FindRes<CTexture>(ToWString(strKey));
 
    target->SetItemTex(tex);
}
