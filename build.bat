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