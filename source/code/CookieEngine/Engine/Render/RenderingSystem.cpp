#include "RenderingSystem.h"

#include "Core/Application.h"
#include "Core/Profiling/Profiling.h"
#include "Core/Math/Math.h"

#include "Render/RenderAPI.h"
#include "Render/IMGUI/IMGUI_Impl.h"

#include "Resources/Resources.h"

#include "Entities/EntityAdmin.h"

namespace Cookie {

	using namespace RenderingAPI;

	// Batch rendering
	// --------------------------------------------------------------------------

	struct RenderBatch {
		VertexBuffer m_VertexBuffer;
		IndexBuffer m_IndexBuffer;
		Layout m_Layout;
		Program m_Program;
		Texture m_Texture;

		u32 m_NumSpritesToDraw;
		TVector<Vertex> m_Vertices{};
	};

	RenderBatch batch;
	u32 *indices;

	void AddQuad(RenderBatch *batch, Float3 pos, u32 width, u32 height, f32 pixelsPerUnit) {
		Quad q;
		q.m_Vertices[0] =
			Vertex(Float3(-(f32)width / (2.0f * pixelsPerUnit), -(f32)height / (2.0f * pixelsPerUnit), 0.0f), Float2(0.0f, 0.0f));
		q.m_Vertices[1] =
			Vertex(Float3((f32)width / (2.0f * pixelsPerUnit), -(f32)height / (2.0f * pixelsPerUnit), 0.0f), Float2(1.0f, 0.0f));
		q.m_Vertices[2] =
			Vertex(Float3((f32)width / (2.0f * pixelsPerUnit), (f32)height / (2.0f * pixelsPerUnit), 0.0f), Float2(1.0f, 1.0f));
		q.m_Vertices[3] =
			Vertex(Float3(-(f32)width / (2.0f * pixelsPerUnit), (f32)height / (2.0f * pixelsPerUnit), 0.0f), Float2(0.0f, 1.0f));

		for (size_t i = 0; i < 4; i++) {
			q.m_Vertices[i].m_Pos += pos;
		}

		for (size_t i = 0; i < 4; i++) {
			batch->m_Vertices.push_back(q.m_Vertices[i]);
		}

		++batch->m_NumSpritesToDraw;
	}

	void FlushBatch(RenderBatch *batch) {
		Matrix4 view = glm::lookAt(Float3(0.0f, 0.0f, 5.0f), Float3(0.0f), Float3(0.0f, 1.0f, 0.0f));
		Matrix4 proj = glm::perspective(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);
		Matrix4 model = Matrix4(1.0f);

		Context::BindProgram(&batch->m_Program);

		batch->m_Program.SetUniformMat4("view", glm::value_ptr(view));
		batch->m_Program.SetUniformMat4("projection", glm::value_ptr(proj));
		batch->m_Program.SetUniformMat4("model", glm::value_ptr(model));

		// Issue Drawcall
		Device::BufferDataIntoVertexBuffer(&batch->m_VertexBuffer, (const char *)batch->m_Vertices.data(),
										   batch->m_Vertices.size() * sizeof(Vertex));
		Context::BindVertexBuffer(&batch->m_VertexBuffer);
		Context::BindIndexBuffer(&batch->m_IndexBuffer);
		Context::BindTexture(&batch->m_Texture);
		Context::BindLayout(&batch->m_Layout);
		Context::Submit(batch->m_NumSpritesToDraw);

		// Reset Batch
		batch->m_Vertices.clear();
		batch->m_NumSpritesToDraw = 0;
	}

	// --------------------------------------------------------------------------

	void RenderingSystem::InitSignature() {
		m_Signature.set(g_Admin->GetComponentSignatureID<TransformComponent>(), true);
		m_Signature.set(g_Admin->GetComponentSignatureID<RenderComponent>(), true);
	}

	void RenderingSystem::Init() {
		Context::Init();

		// Init IMGUI
		ImGuiRenderer::Init();

		// Init Batch Rendering
		u32 MAX_SPRITES = 100000;
		u32 MAX_INDICES = MAX_SPRITES * 6;

		batch.m_VertexBuffer = Device::CreateDynamicVertexBuffer(MAX_SPRITES);

		indices = new u32[MAX_INDICES];
		u32 offset = 0;
		for (size_t i = 0; i < MAX_INDICES; i += 6) {
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}

		batch.m_IndexBuffer = Device::CreateIndexBuffer((char *)indices, sizeof(u32) * MAX_INDICES, DataType::UINT);
	}

	// --------------------------------------------------------------------------

	void RenderingSystem::Update(f32 dt) {
		CKE_PROFILE_EVENT();
		// Clear buffer
		Context::ClearColorBuffer(0.95f, 0.6f, 0.05f, 1.0f);

		// TODO: Rendering Flow
		//
		// On Initialization:
		//		Group all entities by layers
		//		For each layer group:
		//			Group all entities by sprite
		// Only update the groups when a new rendereable entity is created
		//
		// On Update
		//		Discard all entities outside of the camera view
		//		For all layer groups
		//				For all sprite groups
		//					Create a Batch
		//					Add Sprite Quads
		//					Submit Batch Drawcall
		//
		u32 numBatches = 0;
		for (auto const &entityID : m_EntitiesCache) {
			TransformComponent *t = g_Admin->GetComponent<TransformComponent>(entityID);
			RenderComponent *r = g_Admin->GetComponent<RenderComponent>(entityID);

			SpriteRenderData *sp = g_ResourcesDatabase.GetSpriteData(r->m_SpriteHandle);

			if (batch.m_Texture.m_ID != sp->m_Texture.m_ID) {
				if (batch.m_NumSpritesToDraw > 0) {
					FlushBatch(&batch);
					++numBatches;
				}
				batch.m_Texture = sp->m_Texture;
				batch.m_Layout = sp->m_Layout;
				batch.m_Program = sp->m_Program;
			}

			AddQuad(&batch, t->m_Position, sp->m_Width, sp->m_Height, sp->m_PixelsPerUnit);
		}

		FlushBatch(&batch);
		++numBatches;

		//// TODO: Adjust proj matrix dynamically
		// Matrix4 view = glm::lookAt(Float3(0.0f, 0.0f, 5.0f), Float3(0.0f), Float3(0.0f, 1.0f, 0.0f));
		// Matrix4 proj = glm::perspective(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);

		//// Render all Objects
		// for (auto const &entityID : m_EntitiesCache) {
		//	TransformComponent *t = g_Admin->GetComponent<TransformComponent>(entityID);
		//	RenderComponent *r = g_Admin->GetComponent<RenderComponent>(entityID);

		//	SpriteRenderData *sp = g_ResourcesDatabase.GetSpriteData(r->m_SpriteHandle);

		//	Context::BindProgram(&sp->m_Program);

		//	// Camera Config
		//	sp->m_Program.SetUniformMat4("view", glm::value_ptr(view));
		//	sp->m_Program.SetUniformMat4("projection", glm::value_ptr(proj));

		//	// Model
		//	Matrix4 model = Matrix4(1.0f);
		//	Float3 *rot = &t->m_Rotation;
		//	model = glm::translate(model, t->m_Position);
		//	model = glm::rotate(model, rot->x, Float3(1.0f, 0.0f, 0.0f));
		//	model = glm::rotate(model, rot->y, Float3(0.0f, 1.0f, 0.0f));
		//	model = glm::rotate(model, rot->z, Float3(0.0f, 0.0f, 1.0f));
		//	sp->m_Program.SetUniformMat4("model", glm::value_ptr(model));

		//	// Issue Drawcall
		//	Context::BindVertexBuffer(&sp->m_VertexBuffer);
		//	Context::BindIndexBuffer(&sp->m_IndexBuffer);
		//	Context::BindTexture(&sp->m_Texture);
		//	Context::BindLayout(&sp->m_Layout);
		//	Context::Submit();
		//}

		// IMGUI Rendering
		ImGuiRenderer::NewFrame();
		ImGui::LabelText("FPS", "%f", 1.0f / dt);
		ImGui::LabelText("FrameTime", "%f ms", dt * 1000.0f);
		ImGui::LabelText("Batches", "%d", numBatches);
		ImGuiRenderer::Render();

		// Swap Buffers
		glfwSwapBuffers(g_AppData.m_Window.m_Handle);
	}

	void RenderingSystem::Shutdown() {
		ImGuiRenderer::Shutdown();
		delete indices;
	}

} // namespace Cookie