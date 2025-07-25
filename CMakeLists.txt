cmake_minimum_required(VERSION 3.24.2)

project(game VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(USE_SHARED_LIBS "Whether to use shared or static libraries." ON)
set(HIDE_TERMINAL False)

include(ExternalProject)

include("${CMAKE_SOURCE_DIR}/external/rapidjson.cmake")
include_directories(SYSTEM "${CMAKE_BINARY_DIR}/external/rapidjson/src/rapidjson/include")

include(FetchContent)
include("${CMAKE_SOURCE_DIR}/external/imgui.cmake")
include_directories(SYSTEM "${CMAKE_BINARY_DIR}/_deps/imgui-src/")
include_directories(SYSTEM "${CMAKE_BINARY_DIR}/_deps/imgui-src/backends")

set(SOURCES
	src/main.cpp
	src/InputHandler.cpp
	src/FPSHandler.cpp
	src/GRY_Video.cpp
	src/GRY_Audio.cpp
	src/GRY_Game.cpp
	src/Scene.cpp
	src/SceneManager.cpp
	src/transitions/FadeToBlack.cpp
	src/GRY_Lib.cpp
	src/GRY_JSON.cpp
	src/GRY_PixelGame.cpp
	src/GRY_Tiled.cpp
	src/GRY_Texture.cpp
	src/tile/TileMapDialogueResource.cpp
	src/tile/Tileset.cpp
	src/tile/TileCollision.cpp
	src/tile/TileTileMap.cpp
	src/tile/TileEntityMap.cpp
	src/scenes/ExampleScene.cpp
	src/scenes/TilesetScene.cpp
	src/scenes/TileMapScene.cpp
	src/scenes/MenuScene.cpp
	src/scenes/TileMapMenuScene.cpp
	src/scenes/MapMenuMiscScene.cpp
	src/tile/TileMapRenderer.cpp
	src/tile/TileMapCamera.cpp
	src/tile/TileMapMovement.cpp
	src/tile/TileMapQuadTrees.cpp
	src/tile/TileSpriteAnimator.cpp
	src/tile/TileMapInput.cpp
	src/tile/TileMapSpeak.cpp
	src/tile/TileMapScripting.cpp
	src/tile/TileMapScriptResource.cpp
	src/imguiDebugger.cpp
	src/QuadTree.cpp
	src/scenes/TextBoxScene.cpp
	src/textbox/Fontset.cpp
	src/textbox/TextBoxRenderer.cpp
	src/scenes/TextDecisionScene.cpp
	src/SoundResource.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
add_dependencies(game rapidjson)

if(UNIX)
	target_link_options(${PROJECT_NAME}
		PRIVATE -lprofiler
	)
endif()

target_sources(${PROJECT_NAME} PUBLIC
	${IMGUI_SOURCE_DIR}/imgui.cpp
	${IMGUI_SOURCE_DIR}/imgui_draw.cpp
	${IMGUI_SOURCE_DIR}/imgui_tables.cpp
	${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
	${IMGUI_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
	${IMGUI_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)

target_include_directories(${PROJECT_NAME}
	PRIVATE include
	PUBLIC ${RAPIDJSON_INCLUDE_DIR}
	PUBLIC ${IMGUI_INCLUDE_DIR}
)

configure_file(gameconfig.h.in gameconfig.h)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if(WIN32)

	if(HIDE_TERMINAL)
		message("-- Building without terminal")
		set_property(TARGET ${PROJECT_NAME} PROPERTY WIN32_EXECUTABLE True)
	endif()

	add_custom_command(TARGET game POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E make_directory
		"${CMAKE_BINARY_DIR}/assets"
	)
    
	add_custom_command(TARGET game POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_directory_if_different
		${PROJECT_SOURCE_DIR}/assets
		"${CMAKE_BINARY_DIR}/assets"
	)

	add_custom_command(TARGET game POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_directory_if_different
		${PROJECT_SOURCE_DIR}/config
		"${CMAKE_BINARY_DIR}/config"
	)
    
else()
	#TODO check if this can be better linux
	file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
	file(COPY config DESTINATION ${CMAKE_BINARY_DIR})
endif()

if(USE_SHARED_LIBS)
	message(STATUS "Building using shared libs")
	if(WIN32)
		message(STATUS "OS: Windows")
	elseif(UNIX)
		message(STATUS "OS: Unix")
	else()
		message(STATUS "OS Not explicitly supported")
	endif()

	find_library(SDL3_LIB NAMES SDL3 sdl3 PATH_SUFFIXES lib)
	find_library(SDL3_IMAGE_LIB NAMES SDL3_image sdl3_image PATH_SUFFIXES lib)
	find_library(FMOD_LIB NAMES fmodL_vc PATH_SUFFIXES lib)

	message(STATUS "SDL3 shared library: ${SDL3_LIB}")
	message(STATUS "SDL3_image shared library: ${SDL3_IMAGE_LIB}")
	message(STATUS "FMOD shared library:" ${FMOD_LIB})

	add_library(SDL3::SDL3 SHARED IMPORTED)
	add_library(SDL3::IMAGE SHARED IMPORTED)
	add_library(FMOD SHARED IMPORTED)

	if(WIN32)
	set_property(TARGET SDL3::SDL3 PROPERTY IMPORTED_IMPLIB ${SDL3_LIB})
	set_property(TARGET SDL3::IMAGE PROPERTY IMPORTED_IMPLIB ${SDL3_IMAGE_LIB})
	set_property(TARGET FMOD PROPERTY IMPORTED_IMPLIB ${FMOD_LIB})
	else()
	set_property(TARGET SDL3::SDL3 PROPERTY IMPORTED_LOCATION ${SDL3_LIB})
	set_property(TARGET SDL3::IMAGE PROPERTY IMPORTED_LOCATION ${SDL3_IMAGE_LIB})
	set_property(TARGET FMOD PROPERTY IMPORTED_LOCATION ${FMOD_LIB})
	endif()
else()
	message(STATUS "Static building currently unsupported until I have FMOD static libs")
	message(STATUS "Building using static libs")
	if(WIN32)
		message(STATUS "OS: Windows")
		# Windows libs
		message(STATUS "Linking windows libraries for static build")
		target_link_libraries(${PROJECT_NAME}
		PUBLIC cfgmgr32.lib
		PUBLIC imm32.lib
		PUBLIC Version.Lib
		PUBLIC winMM.lib
		)
	elseif(UNIX)
		message(STATUS "OS: Unix")
	else()
		message(STATUS "OS Not explicitly supported")
	endif()

	if(WIN32)
	find_library(SDL3_LIB NAMES SDL3-static sdl3-static PATH_SUFFIXES lib)
	find_library(SDL3_IMAGE_LIB NAMES SDL3_image-static sdl3_image-static PATH_SUFFIXES lib)
	else()
	find_library(SDL3_LIB NAMES libSDL3.a PATH_SUFFIXES lib)
	find_library(SDL3_IMAGE_LIB NAMES libSDL3_image.a PATH_SUFFIXES lib)
	endif()
	message(STATUS "SDL3 static library: ${SDL3_LIB}")
	message(STATUS "SDL3_image static library: ${SDL3_IMAGE_LIB}")

	add_library(SDL3::SDL3 STATIC IMPORTED)
	add_library(SDL3::IMAGE STATIC IMPORTED)

	set_property(TARGET SDL3::SDL3 PROPERTY IMPORTED_LOCATION ${SDL3_LIB})
	set_property(TARGET SDL3::IMAGE PROPERTY IMPORTED_LOCATION ${SDL3_IMAGE_LIB})
endif()

find_path(SDL3_INCLUDE_PATH NAMES SDL3)
find_path(SDL3_IMAGE_INCLUDE_PATH NAMES SDL3_image)
find_path(FMOD_INCLUDE_PATH NAMES fmod)

message(STATUS "SDL3 include path: ${SDL3_INCLUDE_PATH}")
message(STATUS "SDL3_image include path: ${SDL3_IMAGE_INCLUDE_PATH}")
message(STATUS "FMOD include path:" ${FMOD_INCLUDE_PATH})

target_include_directories(${PROJECT_NAME}
	PUBLIC ${PROJECT_BINARY_DIR}
	PUBLIC ${SDL3_INCLUDE_PATH}
	PUBLIC ${SDL3_IMAGE_INCLUDE_PATH}
	PUBLIC ${FMOD_INCLUDE_PATH}
)

target_link_libraries(${PROJECT_NAME} 
	PUBLIC SDL3::SDL3
	PUBLIC SDL3::IMAGE
	PUBLIC FMOD
)