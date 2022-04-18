project "CookieGame"
    kind "ConsoleApp"
    files {
        "**.cpp",
        "**.h"
    }
    includedirs {
        "%{wks.location}/source/code/CookieEngine/ThirdParty/glfw/include",
        "%{wks.location}/source/code/CookieEngine/ThirdParty/glad/include",
        "%{wks.location}/source/code/CookieEngine/ThirdParty/glm",
        "%{wks.location}/source/code/CookieEngine/ThirdParty/imgui",
        "%{wks.location}/source/code/CookieEngine/ThirdParty/stb_image",
        "%{wks.location}/source/code/CookieEngine/ThirdParty/spdlog/include",
        "%{wks.location}/source/code/CookieEngine/Engine"
    }
    links {
        "CookieEngine",
        "opengl32.lib",
        "GLFW"
    }

    postbuildcommands {
        "{COPYDIR} %{wks.location}/source/resources/** %{cfg.targetdir}"
    }