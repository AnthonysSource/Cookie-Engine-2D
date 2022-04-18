project "CookieEngine"
    kind "StaticLib"
    files {
        "Engine/**.c",
        "Engine/**.cpp",
        "Engine/**.h",
        "Engine/**.hpp",
        "ThirdParty/imgui/**.cpp",
        "ThirdParty/imgui/**.h",
        "ThirdParty/stb_image/*.cpp",
        "ThirdParty/glad/src/glad.c"
    }
    includedirs {
        "ThirdParty/glfw/include",
        "ThirdParty/glad/include",
        "ThirdParty/glm",
        "ThirdParty/imgui",
        "ThirdParty/stb_image",
        "ThirdParty/spdlog/include",
        "Engine"
    }
    links {
        "opengl32.lib",
        "GLFW"
    }

group "ThirdParty"
    include("ThirdParty/glfw")
group "" -- end of "ThirdParty"