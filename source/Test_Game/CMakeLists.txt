# Test Game
set(TestGame "Test_Game")

file(GLOB_RECURSE TestGame_Source
    "*.cpp"
    "*.h"
)

add_executable(${TestGame} ${TestGame_Source})

# Set target directories
set_target_properties(${TestGame} PROPERTIES
	OUTPUT_NAME "Test_Game"

	ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${BUILD_DIR_DEBUG}TestGame
	LIBRARY_OUTPUT_DIRECTORY_DEBUG ${BUILD_DIR_DEBUG}TestGame
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BUILD_DIR_DEBUG}TestGame
	
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${BUILD_DIR_RELEASE}TestGame
	LIBRARY_OUTPUT_DIRECTORY_RELEASE ${BUILD_DIR_RELEASE}TestGame
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BUILD_DIR_RELEASE}TestGame
	
	ARCHIVE_OUTPUT_DIRECTORY_DIST ${BUILD_DIR_DIST}TestGame
	LIBRARY_OUTPUT_DIRECTORY_DIST ${BUILD_DIR_DIST}TestGame
	RUNTIME_OUTPUT_DIRECTORY_DIST ${BUILD_DIR_DIST}TestGame
	
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED YES)

# Includes
target_include_directories(${TestGame} PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glfw/include
${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glad/include
${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glm
${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui
${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/stb_image
${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/spdlog/include
${CMAKE_CURRENT_SOURCE_DIR}/../Cookie_Engine/code/Engine
${CMAKE_CURRENT_SOURCE_DIR}/../Cookie_Engine/code/3rdParty/spdlog/include
${CMAKE_CURRENT_SOURCE_DIR}/../Cookie_Engine/code/3rdParty/glfw/include
${CMAKE_CURRENT_SOURCE_DIR}/../Cookie_Engine/code/3rdParty/glad/include
${CMAKE_CURRENT_SOURCE_DIR}/../Cookie_Engine/code/3rdParty/glm
${CMAKE_CURRENT_SOURCE_DIR}/../Cookie_Engine/code/3rdParty/imgui
${CMAKE_CURRENT_SOURCE_DIR}/../Cookie_Engine/code/3rdParty/stb_image)

# Dependencies
target_link_libraries(${TestGame} PRIVATE
	opengl32.lib
	glfw
	Cookie_Engine)

# Defines
target_compile_definitions(${TestGame} PRIVATE
	COOKIE_PLATFORM_WINDOWS
	GLFW_INCLUDE_NONE
	$<$<CONFIG:Debug>:COOKIE_DEBUG>
	$<$<CONFIG:Debug>:COOKIE_LOGGING>)