#include "RenderingSystem.h"
#include "Core/Application.h"
#include "Core/RenderingSystem/RenderingAPI.h"
#include "glad/glad.h"

namespace Cookie {
namespace RenderingSystem {

	using namespace RenderingAPI;

	// Temporal variables stored in the stack
	// for rendering tests
	VertexArray vertexArray;
	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;
	Program program;

	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f,
	};

	u32 indices[] = {0, 1, 2, 2, 3, 0};

	void RenderingSystem::Init() {
		// Viewport settings and resize
		glViewport(0, 0, 1280, 720);
		glfwSetFramebufferSizeCallback(
			Application::appData.m_Window,
			[](GLFWwindow *window, i32 width, i32 height) { glViewport(0, 0, width, height); });

		// Quad Rendering Test
		vertexBuffer = Device::CreateVertexBuffer((char *)vertices, sizeof(vertices));
		indexBuffer = Device::CreateIndexBuffer((char *)indices, sizeof(indices), UINT);

		program = Device::CreateProgram("shaders/basic.vert", "shaders/basic.frag");

		// Layout
		VertexArrayLayout layout;
		layout.AddAttribute(LayoutAttribute(0, FLOAT, 3, false));

		vertexArray = Device::CreateVertexArray();
		vertexArray.BindIndexBuffer(&indexBuffer);
		vertexArray.BindVertexBuffer(&vertexBuffer);
		vertexArray.SetLayout(&layout);
	}

	void RenderingSystem::Render() {
		// Issue Drawcall
		Context::BindProgram(&program);
		Context::DrawIndexed(&vertexArray);
	}

} // namespace RenderingSystem
} // namespace Cookie