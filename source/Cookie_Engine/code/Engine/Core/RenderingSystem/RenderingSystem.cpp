#include "RenderingSystem.h"
#include "Core/Application.h"
#include "Core/IMGUI/IMGUI_Impl.h"
#include "Core/RenderingAPI/RenderingAPI.h"

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

	glm::vec3 quadRot = glm::vec3(0.0f);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f,
	};

	u32 indices[] = {0, 1, 2, 2, 3, 0};

	void RenderingSystem::Init() {
		Context::Init();

		// Quad Rendering Test
		vertexBuffer = Device::CreateVertexBuffer((char *)vertices, sizeof(vertices));
		indexBuffer = Device::CreateIndexBuffer((char *)indices, sizeof(indices), UINT);

		program = Device::CreateProgram("shaders/MVP_Basic.vert", "shaders/MVP_Basic.frag");

		VertexArrayLayout layout;
		layout.AddAttribute(LayoutAttribute(0, FLOAT, 3, false));

		vertexArray = Device::CreateVertexArray();
		vertexArray.BindIndexBuffer(&indexBuffer);
		vertexArray.BindVertexBuffer(&vertexBuffer);
		vertexArray.SetLayout(&layout);

		IMGUI_Impl::Init();
	}

	void RenderingSystem::Render() {
		// Clear buffer
		Context::ClearColorBuffer(0.95f, 0.6f, 0.05f, 1.0f);

		IMGUI_Impl::NewFrame();

		// Set MVP matrix
		glm::mat4 view =
			glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 proj = glm::perspective(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::rotate(model, quadRot.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, quadRot.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, quadRot.z, glm::vec3(0.0f, 0.0f, 1.0f));

		program.SetUniformMat4("model", glm::value_ptr(model));
		program.SetUniformMat4("view", glm::value_ptr(view));
		program.SetUniformMat4("projection", glm::value_ptr(proj));

		// IMGUI Rendering
		ImGui::Begin("Quad Parameters");
		ImGui::SliderFloat3("Rotation", glm::value_ptr(quadRot), -5.0f, 5.0f, "%.3f", 1.0f);
		ImGui::End();

		// Issue Drawcall
		Context::BindProgram(&program);
		Context::DrawIndexed(&vertexArray);

		IMGUI_Impl::Render();

		// Swap Buffers
		glfwSwapBuffers(Application::appData.m_Window);
	}

	void RenderingSystem::Shutdown() { IMGUI_Impl::Shutdown(); }

} // namespace RenderingSystem
} // namespace Cookie