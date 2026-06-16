// Library importing
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <functional>
#include <cstdlib>

using namespace std;
// Main

struct TaskEntry {
    string category;
    string name;
    function<void()> render;
};

inline vector<TaskEntry>& GetTasks() {
    static vector<TaskEntry> tasks;
    return tasks;
}

template <typename T>
struct AutoTask {
    AutoTask(string category, string name) {
        GetTasks().push_back({move(category), move(name), [] {
            static T instance;
            instance.Render();
        }});
    }
};

class TaskGithubLogin {
public:
    inline static AutoTask<TaskGithubLogin> _reg{"Login", "GitHub Auth Login"};

    void Render() {
        if (ImGui::Button("Run gh auth login", ImVec2(-FLT_MIN, 30))) {
            system("gh auth login");
        }
    }
};
// Classes
class TaskManualLogin {
private:
    char user[256] = "";
    char email[256] = "";
    bool save = false;
    bool cache = false;
public:
    inline static AutoTask<TaskManualLogin> _reg{"Login", "Manual Git Config"};

    void Render() {
        ImGui::InputText("Username", user, IM_ARRAYSIZE(user));
        ImGui::InputText("Email", email, IM_ARRAYSIZE(email));
        ImGui::Checkbox("Save Credentials", &save);
        if (save) {
            ImGui::Indent();
            ImGui::Checkbox("Use Cache", &cache);
            ImGui::Unindent();
        }
        if (ImGui::Button("Save Config", ImVec2(-FLT_MIN, 30))) {
            system(("git config --global user.name '" + string(user) + "'").c_str());
            system(("git config --global user.email '" + string(email) + "'").c_str());
            if (save) {
                string helper = cache ? "cache" : "manager";
                system(("git config --global credential.helper " + helper).c_str());
            }
        }
    }
};

class TaskCommitAll {
private:
    char msg[256] = "";
public:
    inline static AutoTask<TaskCommitAll> _reg{"Publish", "Commit & Push All"};

    void Render() {
        ImGui::InputText("Message##all", msg, IM_ARRAYSIZE(msg));
        if (ImGui::Button("Execute Push All", ImVec2(-FLT_MIN, 30))) {
            system("git add .");
            system(("git commit -m '" + string(msg) + "'").c_str());
            system("git push");
        }
    }
};

class TaskCommitFile {
private:
    char msg[256] = "";
    char file[256] = "";
public:
    inline static AutoTask<TaskCommitFile> _reg{"Publish", "Commit & Push File"};

    void Render() {
        ImGui::InputText("Message##file", msg, IM_ARRAYSIZE(msg));
        ImGui::InputText("File Path##file", file, IM_ARRAYSIZE(file));
        if (ImGui::Button("Execute Push File", ImVec2(-FLT_MIN, 30))) {
            system(("git add " + string(file)).c_str());
            system(("git commit -m '" + string(msg) + "'").c_str());
            system("git push");
        }
    }
};
// themes
void ApplyDarkTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 6.0f;
    style.ChildRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.PopupRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.WindowPadding = ImVec2(15, 15);
    style.FramePadding = ImVec2(10, 8);
    style.ItemSpacing = ImVec2(12, 10);

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text]           = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
    colors[ImGuiCol_WindowBg]       = ImVec4(0.09f, 0.09f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg]        = ImVec4(0.13f, 0.14f, 0.16f, 1.00f);
    colors[ImGuiCol_Border]         = ImVec4(0.22f, 0.24f, 0.27f, 1.00f);
    colors[ImGuiCol_FrameBg]        = ImVec4(0.18f, 0.20f, 0.22f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.26f, 0.29f, 1.00f);
    colors[ImGuiCol_FrameBgActive]  = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);
    colors[ImGuiCol_Header]         = ImVec4(0.18f, 0.20f, 0.22f, 1.00f);
    colors[ImGuiCol_HeaderHovered]  = ImVec4(0.24f, 0.26f, 0.29f, 1.00f);
    colors[ImGuiCol_HeaderActive]   = ImVec4(0.20f, 0.22f, 0.24f, 1.00f);
    colors[ImGuiCol_Button]         = ImVec4(0.20f, 0.44f, 0.85f, 1.00f);
    colors[ImGuiCol_ButtonHovered]  = ImVec4(0.26f, 0.53f, 0.94f, 1.00f);
    colors[ImGuiCol_ButtonActive]   = ImVec4(0.16f, 0.38f, 0.76f, 1.00f);
    colors[ImGuiCol_CheckMark]      = ImVec4(0.20f, 0.44f, 0.85f, 1.00f);
}
// main() runner *mostly contains UI*
int main() {
    if (!glfwInit()) return 1;

    GLFWwindow* window = glfwCreateWindow(1100, 700, "Automated Git GUI", nullptr, nullptr);
    if (!window) return 1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    ApplyDarkTheme();

    vector<string> categories;
    for (const auto& t : GetTasks()) {
        bool found = false;
        for (const auto& c : categories) if (c == t.category) { found = true; break; }
        if (!found) categories.push_back(t.category);
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(io.DisplaySize);

        ImGui::Begin("Git Manager", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

        float panel_width = (io.DisplaySize.x - (20.0f * categories.size()) - 20.0f) / categories.size();

        for (size_t i = 0; i < categories.size(); ++i) {
            const string& cat = categories[i];

            ImGui::BeginChild((cat + "Panel").c_str(), ImVec2(panel_width, 0), true);
            ImGui::TextDisabled("CATEGORY");

            ImGui::SetWindowFontScale(1.2f);
            ImGui::Text("%s", cat.c_str());
            ImGui::SetWindowFontScale(1.0f);

            ImGui::Separator();
            ImGui::Spacing();

            for (const auto& task : GetTasks()) {
                if (task.category == cat) {
                    if (ImGui::CollapsingHeader(task.name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                        ImGui::Spacing();
                        task.render();
                        ImGui::Spacing();
                    }
                }
            }

            ImGui::EndChild();

            if (i < categories.size() - 1) {
                ImGui::SameLine();
            }
        }

        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.09f, 0.09f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}