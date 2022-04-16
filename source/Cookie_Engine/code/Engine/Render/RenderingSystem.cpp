#include "RenderingSystem.h"

#include "Core/Application.h"
#include "Core/IMGUI/IMGUI_Impl.h"
#include "Core/Math.h"
#include "Core/RenderAPI/RenderAPI.h"

#include "Resources/Resources.h"

#include "Entities/EntityAdmin.h"

namespace Cookie {

using namespace RenderingAPI;

//-------------------------------------------------------------------------
// Temporal data to render cookies
// -----------------
// Temporal variables stored in the stack
// for rendering tests
VertexArray vertexArray;
VertexBuffer vertexBuffer;
IndexBuffer indexBuffer;
Program program;

Float3 quadRot = Float3(0.0f);

float vertices[] = {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
					0.5f,  0.5f,  0.0f, 1.0f, 1.0f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f};

u32 indices[] = {0, 1, 2, 2, 3, 0};

ImageCPU img;
//-------------------------------------------------------------------------

void RenderingSystem::InitSignature() {
	m_Signature.set(Application::g_Admin->GetComponentSignatureID<TransformComponent>(), true);
	m_Signature.set(Application::g_Admin->GetComponentSignatureID<RenderComponent>(), true);
}

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

	// Init IMGUI
	ImGuiRenderer::Init();
}

void RenderingSystem::Update(f32 dt) {
	// Clear buffer
	Context::ClearColorBuffer(0.95f, 0.6f, 0.05f, 1.0f);

	// Camera Config
	Matrix4 view = glm::lookAt(Float3(0.0f, 0.0f, 5.0f), Float3(0.0f), Float3(0.0f, 1.0f, 0.0f));
	Matrix4 proj = glm::perspective(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);
	program.SetUniformMat4("view", glm::value_ptr(view));
	program.SetUniformMat4("projection", glm::value_ptr(proj));

	// Render all Objects
	for (auto const &entityID : m_EntitiesCache) {
		TransformComponent *t = Application::g_Admin->GetComponent<TransformComponent>(entityID);

		Matrix4 model = Matrix4(1.0f);
		Float3 *rot = &t->m_Rotation;
		model = glm::translate(model, t->m_Position);
		model = glm::rotate(model, rot->x, Float3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rot->y, Float3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rot->z, Float3(0.0f, 0.0f, 1.0f));
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
	glfwSwapBuffers(Application::g_Window.m_Window);
}

void RenderingSystem::Shutdown() {
	ImGuiRenderer::Shutdown();
	Image::Release(&img);
}

} // namespace Cookie