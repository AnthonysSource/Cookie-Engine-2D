#include "RenderingSystem.h"
#include "Core/Application.h"
#include "Core/RenderingSystem/RenderingAPI.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

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

		program = Device::CreateProgram("shaders/MVP_Basic.vert", "shaders/MVP_Basic.frag");

		// Layout
		VertexArrayLayout layout;
		layout.AddAttribute(LayoutAttribute(0, FLOAT, 3, false));

		vertexArray = Device::CreateVertexArray();
		vertexArray.BindIndexBuffer(&indexBuffer);
		vertexArray.BindVertexBuffer(&vertexBuffer);
		vertexArray.SetLayout(&layout);
	}

	void RenderingSystem::Render() {
		glm::mat4 view =
			glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 proj = glm::perspective(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);
		glm::mat4 model = glm::mat4(1.0f);

		float rotRadians = (float)glm::radians(250.0f * glfwGetTime());
		model = glm::rotate(model, rotRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		program.SetUniformMat4("model", glm::value_ptr(model));
		program.SetUniformMat4("view", glm::value_ptr(view));
		program.SetUniformMat4("projection", glm::value_ptr(proj));

		// Issue Drawcall
		Context::BindProgram(&program);
		Context::DrawIndexed(&vertexArray);
	}

} // namespace RenderingSystem
} // namespace Cookie