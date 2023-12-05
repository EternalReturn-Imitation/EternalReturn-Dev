#include "pch.h"
#include "ListUI.h"

#include <Engine\CKeyMgr.h>

ListUI::ListUI()
    : UI("##List")
    , m_iSelectedIdx(0)
{ 
}

ListUI::~ListUI()
{
}

void ListUI::finaltick()
{
    UI::finaltick();

    // Esc 눌리면 비활성화
    if (KEY_TAP(KEY::ESC))
    {
        SetActive(false);

        // 모든 UI 포커스 해제
        ImGui::SetWindowFocus(nullptr); 
    }
}

int ListUI::render_update()
{    
    // 최근 UI 의 작업영역 사이즈를 알아낸다.
    ImVec2 ListUIContentSize = ImGui::GetContentRegionAvail();
        
    if (ImGui::BeginListBox("##list", ListUIContentSize))
    {
        for (int i = 0; i < m_vecStrData.size(); i++)
        {
            const bool is_selected = (m_iSelectedIdx == i);

            // 리스트 아이템 출력, 반환값은 클릭 True/ False
            if (ImGui::Selectable(m_vecStrData[i].c_str(), is_selected))
            {
                m_iSelectedIdx = i;
            }

            // 특정 아이템(인덱스 i) 위에서 더블클릭이 발생하면
            if (ImGui::IsItemHovered() && 
                ImGui::IsMouseDoubleClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
            {
                SetActive(false);                
                ImGui::SetWindowFocus(nullptr);

                // Select Delegate 가 등록되어있다면 호출해 준다.
                if (m_SelectInst && m_SelectDelegate)
                {
                    (m_SelectInst->*m_SelectDelegate)((DWORD_PTR)m_vecStrData[i].c_str());
                }
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }
       

    return TRUE;
}

void ListUI::Reset(const string& _strName, ImVec2 _vSize)
{
    Clear();

    SetName(_strName);

    SetSize(_vSize.x, _vSize.y);

    SetActive(true);

    // 위치 조정
    HWND hWnd = ImGuiMgr::GetInst()->GetMainHwnd();

    RECT rt = {};
    //GetClientRect(hWnd, &rt);
    GetWindowRect(hWnd, &rt);

    float CenterX = (float)((rt.left + rt.right) / 2);
    float CenterY = (float)((rt.bottom + rt.top) / 2);

    SetPopupPos(ImVec2(CenterX - _vSize.x / 2.f, CenterY - _vSize.y / 2.f));
}
