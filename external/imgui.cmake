# Download imgui
ExternalProject_Add(
    imgui
    PREFIX "external/imgui"
    GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
    GIT_TAG master
    TIMEOUT 10
    CMAKE_ARGS
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
)

ExternalProject_Get_Property(imgui SOURCE_DIR)
set(IMGUI_SOURCE_DIR ${SOURCE_DIR})
set(IMGUI_INCLUDE_DIR ${SOURCE_DIR}/include)