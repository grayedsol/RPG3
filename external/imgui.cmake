# Download imgui
FetchContent_Declare(
    imgui
    PREFIX "external/imgui"
    GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
    GIT_TAG master
    TIMEOUT 10
)

FetchContent_MakeAvailable(imgui)

FetchContent_GetProperties(imgui
	SOURCE_DIR IMGUI_SOURCE_DIR
)
set(IMGUI_INCLUDE_DIR ${SOURCE_DIR}/include)