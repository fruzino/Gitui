# **GitUI**
### **Lightweight Git management.** 
## How to use
* Paste the ``gitui.exe`` file inside your folder
* Double click
* Do whatever you want next
## How to mod
* The ``gitui.cpp`` is there, edit that
Example:
```C++
class TaskGithubLogin {
public:
    inline static AutoTask<TaskGithubLogin> _reg{"Header",  "Description"};

    void Render() {
        if (ImGui::Button("Button name", ImVec2(-FLT_MIN, 30))) {
            system("Command execution");
            // other stuff
        }
    }
};
```
* Run build.bat

(It is for creating the actual ``gitui.exe``)
