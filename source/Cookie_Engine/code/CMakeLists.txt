# Engine Core
set(CookieEngine "Cookie_Engine")

# Get all source code files
file(GLOB_RECURSE CookieEngine_Source
	"Engine/*.cpp"
	"Engine/*.h"
	"Engine/*.hpp"
	"Engine/*.c"
	"ThirdParty/imgui/*.cpp"
	"ThirdParty/imgui/*.h"
	"ThirdParty/stb_image/*cpp")

# Add the Glad source file
list(APPEND CookieEngine_Source "ThirdParty/glad/src/glad.c")

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
	RUNTIME_OUTPUT_DIRECTORY_DIST ${BUILD_DIR_DIST}Cookie_Engine)
	
# Includes
target_include_directories(${CookieEngine} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/glfw/include
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/glad/include
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/glm
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/stb_image
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/spdlog/include
	${CMAKE_CURRENT_SOURCE_DIR}/Engine)

# Dependencies
target_link_libraries(${CookieEngine} PUBLIC
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