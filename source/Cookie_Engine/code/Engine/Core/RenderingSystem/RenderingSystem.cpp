#include "RenderingSystem.h"

#include "Core/Math.h"
#include "Core/Application.h"
#include "Core/IMGUI/IMGUI_Impl.h"
#include "Core/RenderingAPI/RenderingAPI.h"

#include "Resources/Resources.h"

namespace Cookie {
namespace RenderingSystem {

	using namespace RenderingAPI;

	// Temporal variables stored in the stack
	// for rendering tests
	VertexArray vertexArray;
	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;
	Program program;

	vec3 quadRot = vec3(0.0f);

	float vertices[] = {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
						0.5f,  0.5f,  0.0f, 1.0f, 1.0f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f};

	u32 indices[] = {0, 1, 2, 2, 3, 0};

	ImageCPU img;

	void RenderingSystem::Init() {
		Context::Init();

		// Vertex & Index Buffer
		vertexBuffer = Device::CreateVertexBuffer((char *)vertices, sizeof(vertices));
		indexBuffer = Device::CreateIndexBuffer((char *)indices, sizeof(indices), UINT);

		// Program
		program = Device::CreateProgram("shaders/Sprite.vert", "shaders/Sprite.frag");

		// VAO Layout
		VertexArrayLayout layout;
		layout.AddAttribute(LayoutAttribute(0, FLOAT, 3, false));
		layout.AddAttribute(LayoutAttribute(1, FLOAT, 2, false));

		// VAO Init
		vertexArray = Device::CreateVertexArray();
		vertexArray.BindIndexBuffer(&indexBuffer);
		vertexArray.BindVertexBuffer(&vertexBuffer);
		vertexArray.SetLayout(&layout);

		// Texture
		img = Image::Load("cookie.png");
		// Currently creating a texture binds it to the last used
		// vertex array
		Device::CreateTexture(img.m_Data, img.m_Width, img.m_Height);

		ImGuiRenderer::Init();
	}

	void RenderingSystem::Render() {
		// Clear buffer
		Context::ClearColorBuffer(0.95f, 0.6f, 0.05f, 1.0f);

		// Set MVP matrix
		mat4 view =
			glm::lookAt(vec3(0.0f, 0.0f, 5.0f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
		mat4 proj = glm::perspective(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);
		mat4 model = mat4(1.0f);

		model = glm::rotate(model, quadRot.x, vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, quadRot.y, vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, quadRot.z, vec3(0.0f, 0.0f, 1.0f));

		program.SetUniformMat4("model", glm::value_ptr(model));
		program.SetUniformMat4("view", glm::value_ptr(view));
		program.SetUniformMat4("projection", glm::value_ptr(proj));

		// IMGUI Rendering
		ImGuiRenderer::NewFrame();

		ImGui::Begin("Quad Parameters");
		ImGui::SliderFloat3("Rotation", glm::value_ptr(quadRot), -5.0f, 5.0f, "%.3f", 1.0f);
		ImGui::End();

		// Issue Drawcall
		Context::BindProgram(&program);
		Context::DrawIndexed(&vertexArray);

		// Render ImGui on top of everything
		ImGuiRenderer::Render();

		// Swap Buffers
		glfwSwapBuffers(Application::window.m_Window);
	}

	void RenderingSystem::Shutdown() {
		ImGuiRenderer::Shutdown();
		
		Image::Release(&img);
	}

} // namespace RenderingSystem
} // namespace Cookie