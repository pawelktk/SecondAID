cmake_minimum_required(VERSION 3.16)
project(SecondAID-GUI VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG latest
)
FetchContent_MakeAvailable(glfw)


FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG docking
)

FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
endif()


set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC 
    ${IMGUI_DIR} 
    ${IMGUI_DIR}/backends
)

target_link_libraries(imgui PUBLIC glfw)

if(WIN32)
    target_link_libraries(imgui PUBLIC opengl32)
else()
    target_link_libraries(imgui PUBLIC GL)
endif()


FetchContent_Declare(
    textedit
    GIT_REPOSITORY https://github.com/pawelktk/ImGuiColorTextEdit
    GIT_TAG master
)

FetchContent_Declare(
    filedialog
    GIT_REPOSITORY https://github.com/aiekick/ImGuiFileDialog
    GIT_TAG master
)


FetchContent_MakeAvailable(textedit)
FetchContent_GetProperties(filedialog)
if(NOT filedialog_POPULATED)
    FetchContent_Populate(filedialog)
endif()

set(TEXTEDIT_DIR ${textedit_SOURCE_DIR})
set(FILEDIALOG_DIR ${filedialog_SOURCE_DIR})


add_library(texteditor STATIC 
    ${TEXTEDIT_DIR}/TextEditor.cpp
    ${TEXTEDIT_DIR}/TextDiff.cpp
)

target_include_directories(texteditor PUBLIC ${TEXTEDIT_DIR})
target_link_libraries(texteditor PUBLIC imgui)

add_library(filedialog STATIC 
    ${FILEDIALOG_DIR}/ImGuiFileDialog.cpp
)
target_include_directories(filedialog PUBLIC ${FILEDIALOG_DIR})
target_link_libraries(filedialog PUBLIC imgui) 



set(SOURCES
    src/main.cpp
)

add_executable(SecondAID-GUI ${SOURCES})

target_include_directories(SecondAID-GUI PRIVATE include)


target_link_libraries(SecondAID-GUI PRIVATE imgui)


if(WIN32)
    target_link_libraries(SecondAID-GUI PRIVATE ws2_32 gdi32 imm32 dwmapi iphlpapi)
    target_link_options(SecondAID-GUI PRIVATE -static -static-libgcc -static-libstdc++)

    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set_target_properties(SecondAID-GUI PROPERTIES WIN32_EXECUTABLE ON)
    endif()
else()
    find_package(X11 REQUIRED)
    target_link_libraries(SecondAID-GUI PRIVATE dl ${X11_LIBRARIES})
endif()

target_link_libraries(SecondAID-GUI PRIVATE imgui texteditor filedialog)