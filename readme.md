# How to use
**Following is a guide on how to use ``gitui.cpp``**
## For normal people
This is for people who **just want to use GitUI**
* Paste the gitui.exe inside your project
* Run it.
* Do whatever you want next :)
### NOTE: If you haven't logged in yet (github) do it or else it wont work! 
**Connect repo aswell**
## For people who want to mod 
This is for people who **want to mod GitUI**
* Add a new command
```C++
class TaskSync {
public:
    inline static AutoTask<TaskSync> _reg{"Publish", "Sync (Pull then Push)"}; // 1st argument function name 2nd button

    void Render() {
        if (ImGui::Button("Sync Now", ImVec2(-FLT_MIN, 30))) { 
            // code here
            system("git pull");
            system("git push");
        }
    }
};
```
* Configuring build process

You can also config the ``build.bat`` 
This will handle how the CPP file builds itself

* Open build.bat with a IDE
* Edit the changes
* Done.

You need to run this after you have made changes to gitui.cpp
# Build.bat

Build.bat usually looks like this:
```Bat
@echo off
set GLFW_INC=C:\glfw-3.4.bin.WIN64\include
set GLFW_LIB=C:\glfw-3.4.bin.WIN64\lib-mingw-w64

g++ gitui.cpp imgui/*.cpp imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp ^
    -o gitui.exe ^
    -I./imgui -I./imgui/backends ^
    -I"%GLFW_INC%" ^
    -L"%GLFW_LIB%" ^
    -lglfw3 -lopengl32 -lgdi32 -limm32

if %errorlevel% equ 0 (
    echo Build successful!
    gitui.exe
) else (
    echo Build failed.
)
pause
```
