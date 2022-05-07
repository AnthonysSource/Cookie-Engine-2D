#include "RenderingSystem.h"

#include "Core/Application.h"
#include "Render/IMGUI/IMGUI_Impl.h"
#include "Core/Math/Math.h"
#include "Render/RenderAPI.h"

#include "Resources/Resources.h"

#include "Entities/EntityAdmin.h"
#include "optick.h"

namespace Cookie {

	using namespace RenderingAPI;

	// --------------------------------------------------------------------------

	void RenderingSystem::InitSignature() {
		m_Signature.set(g_Admin->GetComponentSignatureID<TransformComponent>(), true);
		m_Signature.set(g_Admin->GetComponentSignatureID<RenderComponent>(), true);
	}

	void RenderingSystem::Init() {
		Context::Init();

		// Init IMGUI
		ImGuiRenderer::Init();
	}

	void RenderingSystem::Update(f32 dt) {
		OPTICK_EVENT();
		// Clear buffer
		Context::ClearColorBuffer(0.95f, 0.6f, 0.05f, 1.0f);

		Matrix4 view = glm::lookAt(Float3(0.0f, 0.0f, 5.0f), Float3(0.0f), Float3(0.0f, 1.0f, 0.0f));
		Matrix4 proj = glm::perspective(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);

		// Render all Objects
		for (auto const &entityID : m_EntitiesCache) {
			TransformComponent *t = g_Admin->GetComponent<TransformComponent>(entityID);
			RenderComponent *r = g_Admin->GetComponent<RenderComponent>(entityID);

			SpriteRenderData *sp = g_ResourcesDatabase.GetSpriteData(r->m_SpriteHandle);

			Context::BindProgram(&sp->m_Program);

			// Camera Config
			sp->m_Program.SetUniformMat4("view", glm::value_ptr(view));
			sp->m_Program.SetUniformMat4("projection", glm::value_ptr(proj));

			// Model
			Matrix4 model = Matrix4(1.0f);
			Float3 *rot = &t->m_Rotation;
			model = glm::translate(model, t->m_Position);
			model = glm::rotate(model, rot->x, Float3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, rot->y, Float3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, rot->z, Float3(0.0f, 0.0f, 1.0f));
			sp->m_Program.SetUniformMat4("model", glm::value_ptr(model));

			// Issue Drawcall
			Context::BindVertexBuffer(&sp->m_VertexBuffer);
			Context::BindIndexBuffer(&sp->m_IndexBuffer);
			Context::BindTexture(&sp->m_Texture);
			Context::BindLayout(&sp->m_Layout);
			Context::Submit();
		}

		// IMGUI Rendering
		ImGuiRenderer::NewFrame();
		ImGui::LabelText("FPS", "%f", 1.0f / dt);
		ImGuiRenderer::Render();

		// Swap Buffers
		glfwSwapBuffers(g_AppData.m_Window.m_Handle);
	}

	void RenderingSystem::Shutdown() { ImGuiRenderer::Shutdown(); }

} // namespace Cookie