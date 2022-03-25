#include "Application.h"
#include "BasicTypes.h"
#include "InputSystem/InputSystem.h"
#include "Logging/Logging.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "FileSystem/FileSystem.h"
#include "FileSystem/FileSystemTest.h"

#include "Core/RenderingSystem/RenderingAPI.h"

namespace Cookie {
namespace Application {

	// Ptr to the application window
	GLFWwindow *s_Window;

	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f,
	};

	u32 indices[] = {0, 1, 2, 2, 3, 0};

	void Application::Init() {
		FileSystem::RunFileSystemTest();

		COOKIE_LOG_INFO("Starting up Cookie Engine");
		// Init and create window
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		COOKIE_LOG_INFO("Creating window");
		s_Window = glfwCreateWindow(1280, 720, "Cookie Engine", NULL, NULL);
		if (!s_Window) {
			glfwTerminate();
		}

		COOKIE_LOG_INFO("Setting up graphics context and OpenGL");
		// Set current OpenGL context to the window
		glfwMakeContextCurrent(s_Window);

		// Setup OpenGL function loading
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		COOKIE_LOG_INFO("Initializing input system");
		InputSystem::Init(s_Window);

		// Viewport settings and resize
		glViewport(0, 0, 1280, 720);
		glfwSetFramebufferSizeCallback(s_Window, [](GLFWwindow *window, i32 width, i32 height) {
			glViewport(0, 0, width, height);
		});

		// Quad Rendering Test
		u32 fragShader = 0;
		u32 vertShader = 0;
		u32 program = 0;

		using namespace RenderingAPI;
		// VAO
		VertexArray va = Device::CreateVertexArray();

		// VBO
		VertexBuffer vb = Device::CreateVertexBuffer((char *)vertices, sizeof(vertices));

		// EBO
		IndexBuffer ib = Device::CreateIndexBuffer((char *)indices, sizeof(indices), UINT_32);

		va.BindIndexBuffer(&ib);
		va.BindVertexBuffer(&vb);

		// Shader & Program
		Program p = Device::CreateProgram("shaders/basic.vert", "shaders/basic.frag");

		// Layout
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);

		COOKIE_LOG_INFO("Starting engine loop");
		while (!glfwWindowShouldClose(s_Window)) {
			glClearColor(0.95f, 0.6f, 0.05f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// Issue Drawcall
			Context::BindProgram(&p);
			Context::DrawIndexed(&va);

			glfwSwapBuffers(s_Window);
			glfwPollEvents();
			InputSystem::Update();
		}
	}

	void Application::Shutdown() {
		Log::Info("Shutting down");
		glfwTerminate();
	}

} // namespace Application
} // namespace Cookie