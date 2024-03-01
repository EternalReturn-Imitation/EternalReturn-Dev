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
    ItemDataListColumnID_Recipe,
    ItemDataListColumnID_Stats
};

ItemDataUI::ItemDataUI()
    : UI("##ItemDataUI")
    , m_bOpenTab{ true,true }
    , m_pCurItem(nullptr)
    , m_pCurStatsEditItem(nullptr)
    , m_iDragItemIdx(0)
    , m_iDropItemIdx(0)
    , m_bItemPopup(false)
    , m_iDeleteItemIdx(0)
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
        ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_SizingFixedFit;
}

ItemDataUI::~ItemDataUI()
{
    m_vecItem = nullptr;
    m_umapRecipe = nullptr;
}

void ItemDataUI::init()
{
    
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
    if (!m_vecItem || !m_umapRecipe)
        return 0;

    render_menubar();

    render_Tabs();

    SwapItem();

    ItemPopUp();

    return 0;
}

void ItemDataUI::RegistItemMgr()
{
    m_vecItem = &ER_ItemMgr::GetInst()->m_vecItem;
    m_umapRecipe = &ER_ItemMgr::GetInst()->m_umapRecipe;

    m_pEmptyItemSlotTex = CResMgr::GetInst()->FindRes<CTexture>(L"Ico_ItemGradebg_Empty.png");

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    size_t ItemCnt = (*m_vecItem).size();
    m_vecItemName.resize(ItemCnt);

    for (size_t i = 0; i < ItemCnt; ++i)
        m_vecItemName[i] = converter.to_bytes((*m_vecItem)[i]->GetItemName()).c_str();
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

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Add NewItem.."))
            {
                ER_Item* NewItem = new ER_Item;
                NewItem->m_eItemCode = (*m_vecItem).size();
                (*m_vecItem).push_back(NewItem);

                m_vecItemName.push_back(string());
            }

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

    static ImVector<float> ItemHeight;
    float rowHeight = 41.f;
    

    if (ImGui::BeginTable("##ItemDataList", 8, ItemDataUIFlags, ImVec2(0, 0)))
    {
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_NoReorder, 20.f, ItemDataListColumnID_ID);
        ImGui::TableSetupColumn("Texture", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_NoReorder, 68.f, ItemDataListColumnID_Texture);
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoReorder, 100.f, ItemDataListColumnID_Name);
        ImGui::TableSetupColumn("Grade", ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_WidthFixed, 100.f, ItemDataListColumnID_Grade);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_WidthFixed, 100.f, ItemDataListColumnID_Type);
        ImGui::TableSetupColumn("Slot", ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_WidthFixed, 100.f, ItemDataListColumnID_Slot);
        ImGui::TableSetupColumn("Recipe", ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_WidthFixed, 250.f, ItemDataListColumnID_Recipe);
        ImGui::TableSetupColumn("Stats", ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoSort, 100.f, ItemDataListColumnID_Stats);
        ImGui::TableSetupScrollFreeze(1, 1);

        ImGui::TableHeadersRow();

        ImGui::PushButtonRepeat(true);

        // Show data
        // Demonstrate using clipper for large vertical lists
        // ImGuiListClipper clipper;
        // clipper.Begin((*m_vecItem).size());
        
        if(ItemHeight.size() < (*m_vecItem).size())
            ItemHeight.resize((*m_vecItem).size());

        // while (clipper.Step())
        // {
        //    for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
        //        // Without clipper
        //    {
        for (int row_n = 0; row_n < (*m_vecItem).size(); ++row_n)
        {
                m_pCurItem = (*m_vecItem)[row_n];

                int ID = m_pCurItem->GetCode();
                ImGui::PushID(ID);
                ImGui::TableNextRow(ImGuiTableRowFlags_Headers, 0.0f);

                int ItemGrade = m_pCurItem->GetGrade();
                ImVec4 RowColor = {};
                switch (ItemGrade)
                {
                case 0:
                    RowColor = { 0.1f, 0.1f, 0.1f, 0.6f };  // 일반
                    break;
                case 1:
                    RowColor = { 0.1f, 0.7f, 0.1f, 0.4f };  // 고급
                    break;
                case 2:
                    RowColor = { 0.1f, 0.1f, 0.7f, 0.4f };  // 희귀
                    break;
                case 3:
                    RowColor = { 0.7f, 0.0f, 0.7f, 0.4f };  // 영웅
                    break;
                }

                ImU32 row_bg_color = ImGui::GetColorU32(RowColor);
                ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, row_bg_color);



                // For the demo purpose we can select among different type of items submitted in the first column
                ImGui::TableSetColumnIndex((UINT)ItemDataListColumnID_ID);
                ImGui::PushItemWidth(-FLT_MIN);
                char id[5] = {};
                sprintf(id, "%03d", ID);
        
                const bool item_is_selected = false;
                ImGui::Selectable(id, &item_is_selected, 0, ImVec2(0.f, ItemHeight[row_n]));

                if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Right))
                {
                    m_iDeleteItemIdx = row_n;
                }
                else if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Right))
                {
                    if (m_iDeleteItemIdx == row_n)
                        m_bItemPopup = true;
                    else
                        m_bItemPopup = false;
                }

                if (ImGui::BeginDragDropSource())
                {
                    ImGui::SetDragDropPayload("##ItemSwapPayload", this, sizeof(ER_Item*));
                    ImGui::Text("%d", row_n);

                    m_DragItem = m_pCurItem;
                    m_iDragItemIdx = row_n;

                    ImGui::EndDragDropSource();
                }


                // 드래그 시작 후, 드랍의 후보인 경우
                if (ImGui::BeginDragDropTarget())
                {
                    // 해당 노드에서 마우스 뗀 경우, 지정한 PayLoad 키값이 일치한 경우
                    const ImGuiPayload* pPayLoad = ImGui::AcceptDragDropPayload("##ItemSwapPayload");
                    if (pPayLoad)
                    {
                        m_DropItem = m_pCurItem;
                        m_iDropItemIdx = row_n;
                    }

                    ImGui::EndDragDropTarget();
                }

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
                    const map<wstring, Ptr<CRes>>& mapTex = CResMgr::GetInst()->GetResources(RES_TYPE::TEXTURE);

                    ListUI* pListUI = (ListUI*)ImGuiMgr::GetInst()->FindUI("##List");
                    pListUI->Reset("ItemIcon List", ImVec2(300.f, 500.f));
                    
                    wstring KeyItemIcon = L"ItemIcon";
                    int KeyLen = KeyItemIcon.length();

                    for (const auto& pair : mapTex)
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
                char szbuffer[32] = {};
                strcpy(szbuffer, m_vecItemName[row_n].c_str());

                ImGui::InputText("##ItemName", szbuffer, sizeof(szbuffer));

                float NameSlotWidth = ImGui::GetItemRectSize().x;

                // UTF-8 ->wchar_t
                std::string utf8String = szbuffer;
                std::wstring ItemName = converter.from_bytes(utf8String);

                if (m_vecItemName[row_n].c_str() != ItemName)
                {
                    m_pCurItem->SetItemName(ItemName);
                    m_vecItemName[row_n] = utf8String;
                }

                // Item Grade
                ImGui::TableSetColumnIndex((UINT)ItemDataListColumnID_Grade);
                ImGui::PushItemWidth(-FLT_MIN);
                const char* Grade[] = { u8"일반",u8"고급",u8"희귀",u8"영웅" };
                int CurItemGrade = m_pCurItem->GetGrade();
                ImGui::Combo("##ItemGrade", &CurItemGrade, Grade, 4);
                m_pCurItem->SetItemGrade(CurItemGrade);

                // Item Type
                ImGui::TableSetColumnIndex((UINT)ItemDataListColumnID_Type);
                ImGui::PushItemWidth(-FLT_MIN);
                const char* Types[] = { u8"장비",u8"소비",u8"재료" };
                int CurItemType = m_pCurItem->GetType();
                ImGui::Combo("##ItemType", &CurItemType, Types, 3);
                m_pCurItem->SetItemType(CurItemType);

                // Item Slot
                ImGui::TableSetColumnIndex((UINT)ItemDataListColumnID_Slot);
                ImGui::PushItemWidth(-FLT_MIN);
                ImGui::BeginDisabled((UINT)ER_ITEM_TYPE::EQUIPMENT != CurItemType);
                const char* Slots[] = { u8"장착불가",u8"무기",u8"머리",u8"옷",u8"팔",u8"다리"};
                int CurItemSlot = m_pCurItem->GetSlot();
                ImGui::Combo("##ItemSlot", &CurItemSlot, Slots, 6);
                m_pCurItem->SetItemSlot(CurItemSlot);
                ImGui::EndDisabled();

                // Item Recipe
                ImGui::TableSetColumnIndex((UINT)ItemDataListColumnID_Recipe);
                ImGui::SetNextItemWidth(NameSlotWidth);
                ImGui::BeginDisabled((UINT)ER_ITEM_GRADE::COMMON == CurItemGrade);
                ER_RECIPE CurItemRecipe = m_pCurItem->GetRecipe();
                int Ingr1 = CurItemRecipe.ingredient_1;
                int Ingr2 = CurItemRecipe.ingredient_2;

                if (ImGui::BeginCombo("##CurItemIngr1", m_vecItemName[Ingr1].c_str(), 0))
                {
                    for (int n = 0; n < m_vecItemName.size(); n++)
                    {
                        const bool is_selected = (Ingr1 == n);
                        if (ImGui::Selectable(m_vecItemName[n].c_str(), is_selected))
                        {
                            if (Ingr2 < n)
                            {
                                m_pCurItem->m_uniRecipe.ingredient_1 = Ingr2;
                                m_pCurItem->m_uniRecipe.ingredient_2 = n;
                            }
                            else
                                m_pCurItem->m_uniRecipe.ingredient_1 = n;
                        }
                    }
                    ImGui::EndCombo();
                }
                
                ImGui::SameLine();

                ImGui::SetNextItemWidth(NameSlotWidth);
                if (ImGui::BeginCombo("##CurItemIngr2", m_vecItemName[Ingr2].c_str(), 0))
                {
                    for (int n = 0; n < m_vecItemName.size(); n++)
                    {
                        const bool is_selected = (Ingr2 == n);
                        if (ImGui::Selectable(m_vecItemName[n].c_str(), is_selected))
                        {
                            if (n < Ingr1)
                            {
                                m_pCurItem->m_uniRecipe.ingredient_1 = n;
                                m_pCurItem->m_uniRecipe.ingredient_1 = Ingr1;
                            }
                            else
                                m_pCurItem->m_uniRecipe.ingredient_2 = n;
                        }
                    }
                    ImGui::EndCombo();
                }

                ImGui::EndDisabled();

                // stats
                ImGui::TableSetColumnIndex((UINT)ItemDataListColumnID_Stats);
                ImGui::BeginDisabled((UINT)ER_ITEM_TYPE::INGREDIENT == CurItemType);
                if (ImGui::Button("Edit##ItemStatsEditButton"))
                {
                    m_pCurStatsEditItem = m_pCurItem;
                    m_bItemStatsEditWin = true;
                }
                ImGui::EndDisabled();

                ImGui::SameLine();
                
                ImGui::BeginGroup();
                Print_Stats(m_pCurItem->m_tItemStats);
                ImGui::EndGroup();

                float NextItemSize = ImGui::GetItemRectSize().y;
                ItemHeight[row_n] = NextItemSize <= rowHeight ? rowHeight : NextItemSize;

                ImGui::PopID();
            }

        ImGui::PopButtonRepeat();

        m_pCurItem = nullptr;

        ImGui::EndTable();
    }
   
    render_ItemStatEdit();
}

void ItemDataUI::render_ItemRecipeTable()
{
    ImGui::Text("Recipe");
}

void ItemDataUI::render_ItemStatEdit()
{
    if (!m_bItemStatsEditWin || !m_pCurStatsEditItem)
    {
        m_pCurStatsEditItem = nullptr;
        return;
    }

    string id = "Number";
    id += std::to_string(m_pCurStatsEditItem->GetCode());
    id += "_Stats_Edit";

    ImGui::SetNextWindowSize(ImVec2(220.f, 300.f));

    if (!ImGui::Begin(id.c_str(), &m_bItemStatsEditWin, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::End();
        return;
    }

    float windowWidth = ImGui::GetWindowWidth();
    float InputSlotWidth = 60.f;

    ImGui::Text(u8"공격력 ");
    ImGui::SameLine();
    ImGui::SetCursorPosX(windowWidth - InputSlotWidth);
    ImGui::InputScalar("##AttackPower", ImGuiDataType_S16, &m_pCurStatsEditItem->m_tItemStats.iAttackPower);

    ImGui::Text(u8"레벨당 공격력 ");
    ImGui::SameLine();
    ImGui::SetCursorPosX(windowWidth - InputSlotWidth);
    ImGui::InputScalar("##AttackPowerPerLevel", ImGuiDataType_S16, &m_pCurStatsEditItem->m_tItemStats.iAttackPowerPerLevel);

    ImGui::Text(u8"스킬 증폭 ");
    ImGui::SameLine();
    ImGui::SetCursorPosX(windowWidth - InputSlotWidth);
    ImGui::InputScalar("##SkillAmplification", ImGuiDataType_S16, &m_pCurStatsEditItem->m_tItemStats.iSkillAmplification);

    ImGui::Text(u8"레벨당 스킬 증폭 ");
    ImGui::SameLine();
    ImGui::SetCursorPosX(windowWidth - InputSlotWidth);
    ImGui::InputScalar("##SkillAmplificationPerLevel", ImGuiDataType_S16, &m_pCurStatsEditItem->m_tItemStats.iSkillAmplificationPerLevel);

    ImGui::Text(u8"체력 ");
    ImGui::SameLine();
    ImGui::SetCursorPosX(windowWidth - InputSlotWidth);
    ImGui::InputScalar("##MaxHP", ImGuiDataType_S16, &m_pCurStatsEditItem->m_tItemStats.iMaxHP);

    ImGui::Text(u8"레벨당 체력 ");
    ImGui::SameLine();
    ImGui::SetCursorPosX(windowWidth - InputSlotWidth);
    ImGui::InputScalar("##MaxHPPerLevel", ImGuiDataType_S16, &m_pCurStatsEditItem->m_tItemStats.iMaxHPPerLevel);

    ImGui::Text(u8"체력 재생 ");
    ImGui::SameLine();
    ImGui::SetCursorPosX(windowWidth - InputSlotWidth);
    ImGui::InputFloat("##HPRegen", &m_pCurStatsEditItem->m_tItemStats.fHPRegen, 0.f, 0.f, "%.2f");

    ImGui::Text(u8"스테미너 ");
    ImGui::SameLine();
    ImGui::SetCursorPosX(windowWidth - InputSlotWidth);
    ImGui::InputScalar("##MaxSP", ImGuiDataType_S16, &m_pCurStatsEditItem->m_tItemStats.iMaxSP);

    ImGui::Text(u8"레벨당 스테미너 ");
    ImGui::SameLine();
    ImGui::SetCursorPosX(windowWidth - InputSlotWidth);
    ImGui::InputScalar("##MaxSPPerLevel", ImGuiDataType_S16, &m_pCurStatsEditItem->m_tItemStats.iMaxSPPerLevel);

    ImGui::Text(u8"스테미나 재생 ");
    ImGui::SameLine();
    ImGui::SetCursorPosX(windowWidth - InputSlotWidth);
    ImGui::InputFloat("##SPRegen", &m_pCurStatsEditItem->m_tItemStats.fSPRegen, 0.f, 0.f, "%.2f");

    ImGui::Text(u8"방어력 ");
    ImGui::SameLine();
    ImGui::SetCursorPosX(windowWidth - InputSlotWidth);
    ImGui::InputScalar("##Defense", ImGuiDataType_S16, &m_pCurStatsEditItem->m_tItemStats.iDefense);

    ImGui::Text(u8"공격 속도 ");
    ImGui::SameLine();
    ImGui::SetCursorPosX(windowWidth - InputSlotWidth);
    ImGui::InputFloat("##AttackSpeed", &m_pCurStatsEditItem->m_tItemStats.fAttackSpeed, 0.f, 0.f, "%.2f");

    ImGui::Text(u8"치명타 확률 ");
    ImGui::SameLine();
    ImGui::SetCursorPosX(windowWidth - InputSlotWidth);
    ImGui::InputFloat("##CriticalStrikeChance", &m_pCurStatsEditItem->m_tItemStats.fCriticalStrikeChance, 0.f, 0.f, "%.2f");

    ImGui::Text(u8"치명타 추가 데미지 ");
    ImGui::SameLine();
    ImGui::SetCursorPosX(windowWidth - InputSlotWidth);
    ImGui::InputFloat("##CriticalStrikeDamage", &m_pCurStatsEditItem->m_tItemStats.fCriticalStrikeDamage, 0.f, 0.f, "%.2f");

    ImGui::Text(u8"이동 속도 ");
    ImGui::SameLine();
    ImGui::SetCursorPosX(windowWidth - InputSlotWidth);
    ImGui::InputFloat("##MovementSpeed", &m_pCurStatsEditItem->m_tItemStats.fMovementSpeed, 0.f, 0.f, "%.2f");

    ImGui::Text(u8"시야 ");
    ImGui::SameLine();
    ImGui::SetCursorPosX(windowWidth - InputSlotWidth);
    ImGui::InputFloat("##VisionRange", &m_pCurStatsEditItem->m_tItemStats.fVisionRange, 0.f, 0.f, "%.2f");

    ImGui::Text(u8"쿨타임 감소 ");
    ImGui::SameLine();
    ImGui::SetCursorPosX(windowWidth - InputSlotWidth);
    ImGui::InputFloat("##CooldownReduction", &m_pCurStatsEditItem->m_tItemStats.fCooldownReduction, 0.f, 0.f, "%.2f");

    ImGui::Text(u8"모든 피해 흡혈 ");
    ImGui::SameLine();
    ImGui::SetCursorPosX(windowWidth - InputSlotWidth);
    ImGui::InputFloat("##Omnisyphon", &m_pCurStatsEditItem->m_tItemStats.fOmnisyphon, 0.f, 0.f, "%.2f");

    ImGui::End();
}

void ItemDataUI::Print_Stats(const ER_tStats& _stats)
{
    if (0 != _stats.iAttackPower) ImGui::Text(u8"공격력 : + %d", _stats.iAttackPower);
    if (0 != _stats.iAttackPowerPerLevel) ImGui::Text(u8"레벨당 공격력 : + %d", _stats.iAttackPowerPerLevel);
    if (0 != _stats.iSkillAmplification) ImGui::Text(u8"스킬 증폭 : + %d", _stats.iSkillAmplification);
    if (0 != _stats.iSkillAmplificationPerLevel) ImGui::Text(u8"레벨당 스킬 증폭 : + %d", _stats.iSkillAmplificationPerLevel);
    if (0 != _stats.iMaxHP) ImGui::Text(u8"체력 : + %d", _stats.iMaxHP);
    if (0 != _stats.iMaxHPPerLevel) ImGui::Text(u8"레벨당 체력 : + %d", _stats.iMaxHPPerLevel);
    if (0 != _stats.fHPRegen) ImGui::Text(u8"체력 재생 : + %.0f %%", _stats.fHPRegen * 100);
    if (0 != _stats.iMaxSP) ImGui::Text(u8"스테미너 : + %d", _stats.iMaxSP);
    if (0 != _stats.iMaxSPPerLevel) ImGui::Text(u8"레벨당 스테미너 : + %d", _stats.iMaxSPPerLevel);
    if (0 != _stats.fSPRegen) ImGui::Text(u8"스테미나 재생 : + %.0f %%", _stats.fSPRegen * 100);
    if (0 != _stats.iDefense) ImGui::Text(u8"방어력 : + %d", _stats.iDefense);
    if (0 != _stats.fAttackSpeed) ImGui::Text(u8"공격 속도 : + %.0f %%", _stats.fAttackSpeed * 100);
    if (0 != _stats.fCriticalStrikeChance) ImGui::Text(u8"치명타 확률 : + %.0f %%", _stats.fCriticalStrikeChance * 100);
    if (0 != _stats.fCriticalStrikeDamage) ImGui::Text(u8"치명타 추가 데미지 : + %0.f %%", _stats.fCriticalStrikeDamage * 100);
    if (0 != _stats.fMovementSpeed) ImGui::Text(u8"이동 속도 : + %.2f", _stats.fMovementSpeed);
    if (0 != _stats.fVisionRange) ImGui::Text(u8"시야 : + %.1f", _stats.fVisionRange);
    if (0 != _stats.fCooldownReduction) ImGui::Text(u8"쿨타임 감소 : + %.0f %%", _stats.fCooldownReduction * 100);
    if (0 != _stats.fOmnisyphon) ImGui::Text(u8"모든 피해 흡혈 : + %.0f %%", _stats.fOmnisyphon * 100);
}

void ItemDataUI::SwapItem()
{
    if ((m_DragItem && m_DropItem) || (m_DragItem && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left)))
    {
        if (!m_DragItem || !m_DropItem)
        {
            m_DragItem = nullptr;
            m_DropItem = nullptr;
            return;
        }

        (*m_vecItem)[m_iDragItemIdx] = m_DropItem;
        (*m_vecItem)[m_iDropItemIdx] = m_DragItem;

        UINT ItemCode = m_DragItem->m_eItemCode;
        m_DragItem->m_eItemCode = m_DropItem->m_eItemCode;
        m_DropItem->m_eItemCode = ItemCode;

        m_DragItem = nullptr;
        m_DropItem = nullptr;

        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        size_t ItemCnt = (*m_vecItem).size();

        for (size_t i = 0; i < ItemCnt; ++i)
            m_vecItemName[i] = converter.to_bytes((*m_vecItem)[i]->GetItemName()).c_str();
    }
}

void ItemDataUI::ItemPopUp()
{
    if (m_bItemPopup)
        ImGui::OpenPopup("##OutLinerObjMenu");

    if (ImGui::BeginPopup("##OutLinerObjMenu"))
    {
        m_bItemPopup = false;
        ImGui::Text("menu");
        ImGui::Separator();

        if (ImGui::MenuItem("Delete..##GameObjDelete"))
        {
            // 아이템 제거
            delete (*m_vecItem)[m_iDeleteItemIdx];
            (*m_vecItem).erase((*m_vecItem).begin() + m_iDeleteItemIdx);
            (*m_vecItem).shrink_to_fit();

            // 아이템 코드 갱신
            size_t ItemCnt = (*m_vecItem).size();
            for (size_t i = 0; i < ItemCnt; ++i)
            {
                (*m_vecItem)[i]->m_eItemCode = i;
            }

            // utf-8 아이템 이름 갱신
            m_vecItemName.erase(m_vecItemName.begin() + m_iDeleteItemIdx);
            m_vecItemName.shrink_to_fit();

            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            for (size_t i = 0; i < ItemCnt; ++i)
                m_vecItemName[i] = converter.to_bytes((*m_vecItem)[i]->GetItemName()).c_str();

            m_bItemPopup = false;
            ImGui::CloseCurrentPopup();
        }

        if (ImGui::MenuItem("Close##CloseObjMenu"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void ItemDataUI::SelectItemIcon(DWORD_PTR _data, DWORD_PTR _target)
{
    string strKey = (char*)_data;
    ER_Item* target = (ER_Item*)_target;

    Ptr<CTexture> tex = CResMgr::GetInst()->FindRes<CTexture>(ToWString(strKey));
 
    target->SetItemTex((CTexture*)tex.Get());
}
