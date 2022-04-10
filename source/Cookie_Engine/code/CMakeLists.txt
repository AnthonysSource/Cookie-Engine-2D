# Engine Core
set(CookieEngine "Cookie_Engine")

# Get all source code files
file(GLOB_RECURSE CookieEngine_Source
	"Engine/*.cpp"
	"Engine/*.h"
	"Engine/*.hpp"
	"Engine/*.c"
	"3rdParty/imgui/*.cpp"
	"3rdParty/imgui/*.h")

# Add the Glad source file
list(APPEND CookieEngine_Source "3rdParty/glad/src/glad.c")

# Generate static library	
add_library(${CookieEngine} ${CookieEngine_Source})

# Set target directories
set_target_properties(${CookieEngine} PROPERTIES
	OUTPUT_NAME "Cookie_Engine"

	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${BUILD_DIR_DEBUG}Cookie_Engine
	LIBRARY_OUTPUT_DIRECTORY_DEBUG ${BUILD_DIR_DEBUG}Cookie_Engine
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BUILD_DIR_DEBUG}Cookie_Engine
	
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${BUILD_DIR_RELEASE}Cookie_Engine
	LIBRARY_OUTPUT_DIRECTORY_RELEASE ${BUILD_DIR_RELEASE}Cookie_Engine
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BUILD_DIR_RELEASE}Cookie_Engine
	
	ARCHIVE_OUTPUT_DIRECTORY_DIST ${BUILD_DIR_DIST}Cookie_Engine
	LIBRARY_OUTPUT_DIRECTORY_DIST ${BUILD_DIR_DIST}Cookie_Engine
	RUNTIME_OUTPUT_DIRECTORY_DIST ${BUILD_DIR_DIST}Cookie_Engine
	
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED YES)
	
# Includes
target_include_directories(${CookieEngine} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glfw/include
	${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glad/include
	${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glm
	${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui
	${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/stb_image
	${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/spdlog/include
	${CMAKE_CURRENT_SOURCE_DIR}/Engine)

# Dependencies
target_link_libraries(${CookieEngine} PRIVATE
	opengl32.lib
	glfw)

# Defines
target_compile_definitions(${CookieEngine} PRIVATE
	COOKIE_PLATFORM_WINDOWS
	GLFW_INCLUDE_NONE
	$<$<CONFIG:Debug>:COOKIE_DEBUG>
	$<$<CONFIG:Debug>:COOKIE_LOGGING>)
	
# Set precompiled headers
target_precompile_headers(${CookieEngine} PUBLIC 
	$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/Engine/Ckpch.h>)