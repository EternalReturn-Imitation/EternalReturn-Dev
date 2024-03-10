#include "pch.h"
#include "DebugLogUI.h"

DebugLogUI::DebugLogUI()
    : UI("##DebugLogUI")
    , dbgcontext(nullptr)
{
    SetName("DebugLogUI");
}

DebugLogUI::~DebugLogUI()
{
}

void DebugLogUI::init()
{
    dbgcontext = DebugContext::GetInst();
}

void DebugLogUI::tick()
{
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left, false))
        DEBUG_LOG_COMMAND("DebugLogUI", "tick", u8"마우스 클릭");
}

int DebugLogUI::render_update()
{
    if (!dbgcontext)
        return 0;

    // NextWindow Size

    UINT* dbgflags = (UINT*)dbgcontext->GetDebugFlags();

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Log events:");
    ImGui::SameLine(); ImGui::CheckboxFlags("All", dbgflags, DebugLogFlags_All);
    ImGui::SameLine(); ImGui::CheckboxFlags("Input", dbgflags, DebugLogFlags_Input);
    ImGui::SameLine(); ImGui::CheckboxFlags("Collision", dbgflags, DebugLogFlags_Collision);
    ImGui::SameLine(); ImGui::CheckboxFlags("Command", dbgflags, DebugLogFlags_Command);
    ImGui::SameLine(); ImGui::CheckboxFlags("Process", dbgflags, DebugLogFlags_Process);
    ImGui::SameLine(); ImGui::CheckboxFlags("Info", dbgflags, DebugLogFlags_Info);

    if (ImGui::SmallButton("Clear"))
    {
        dbgcontext->ClearLog();
    }

    ImGui::BeginChild("##ERlog", ImVec2(0.0f, 0.0f), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar);

    ImGuiListClipper clipper;
    const vector<string> dbglogs = dbgcontext->GetDebugLogs();

    clipper.Begin((UINT)dbglogs.size());
    while (clipper.Step())
    {
        for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
        {
            ImGui::Text(dbglogs[line_no].c_str());
        }
    }

    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);
    ImGui::EndChild();

    return 0;
}
