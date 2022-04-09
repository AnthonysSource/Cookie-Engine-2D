#include "RenderingSystem.h"

#include "Core/Application.h"
#include "Core/IMGUI/IMGUI_Impl.h"
#include "Core/Math.h"
#include "Core/RenderingAPI/RenderingAPI.h"

#include "Resources/Resources.h"

#include "EntityObjectModel/EntityAdmin.h"

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

		// Object model test
		Object::GenerateCookies();
	}

	void RenderingSystem::Render() {
		// Clear buffer
		Context::ClearColorBuffer(0.95f, 0.6f, 0.05f, 1.0f);

		// Object model test
		Object::UpdateCookies();

		// Camera Config
		mat4 view = glm::lookAt(vec3(0.0f, 0.0f, 5.0f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
		mat4 proj = glm::perspective(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);
		program.SetUniformMat4("view", glm::value_ptr(view));
		program.SetUniformMat4("projection", glm::value_ptr(proj));

		using namespace Object;
		// Render all Objects
		for (size_t i = 0; i < m_Transforms.size(); i++) {
			mat4 model = mat4(1.0f);
			vec3 *rot = &m_Transforms[i].m_Rotation;
			model = glm::translate(model, m_Transforms[i].m_Position);
			model = glm::rotate(model, rot->x, vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, rot->y, vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, rot->z, vec3(0.0f, 0.0f, 1.0f));
			program.SetUniformMat4("model", glm::value_ptr(model));

			// Issue Drawcall
			Context::BindProgram(&program);
			Context::DrawIndexed(&vertexArray);
		}

		// IMGUI Rendering
		ImGuiRenderer::NewFrame();

		ImGui::Begin("Quad Parameters");
		ImGui::SliderFloat3("Rotation", glm::value_ptr(quadRot), -5.0f, 5.0f, "%.3f", 1.0f);
		ImGui::End();
		// Render ImGui on top of everything
		ImGuiRenderer::Render();

		// Swap Buffers
		glfwSwapBuffers(Application::window.m_Window);
	}

	void RenderingSystem::Shutdown() {
		ImGuiRenderer::Shutdown();

		Object::ClearCookies();

		Image::Release(&img);
	}

} // namespace RenderingSystem
} // namespace Cookie