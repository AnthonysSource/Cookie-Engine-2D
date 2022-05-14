#include "RenderingSystem.h"

#include "Core/Application.h"
#include "Core/Profiling/Profiling.h"
#include "Core/Math/Math.h"

#include "Render/RenderAPI.h"
#include "Render/IMGUI/IMGUI_Impl.h"

#include "Resources/Resources.h"

#include "Entities/EntityAdmin.h"
#include "Entities/BaseComponents.h"

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
		u32 m_MaxSpritesToDraw;
		// TVector<Vertex> m_Vertices{};
		Vertex *m_Vertices;
	};

	RenderBatch batch;
	u32 *indices;

	CKE_FORCE_INLINE void AddQuad(RenderBatch *batch, Float3 pos, u32 width, u32 height, f32 pixelsPerUnit) {

		batch->m_Vertices[batch->m_NumSpritesToDraw * 4 + 0] = Vertex(
			Float3(-(f32)width / (2.0f * pixelsPerUnit) + pos.x, -(f32)height / (2.0f * pixelsPerUnit) + pos.y, pos.z), Float2(0.0f, 0.0f));
		batch->m_Vertices[batch->m_NumSpritesToDraw * 4 + 1] = Vertex(
			Float3((f32)width / (2.0f * pixelsPerUnit) + pos.x, -(f32)height / (2.0f * pixelsPerUnit) + pos.y, pos.z), Float2(1.0f, 0.0f));
		batch->m_Vertices[batch->m_NumSpritesToDraw * 4 + 2] = Vertex(
			Float3((f32)width / (2.0f * pixelsPerUnit) + pos.x, (f32)height / (2.0f * pixelsPerUnit) + pos.y, pos.z), Float2(1.0f, 1.0f));
		batch->m_Vertices[batch->m_NumSpritesToDraw * 4 + 3] = Vertex(
			Float3(-(f32)width / (2.0f * pixelsPerUnit) + pos.x, (f32)height / (2.0f * pixelsPerUnit) + pos.y, pos.z), Float2(0.0f, 1.0f));

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
		Device::BufferDataIntoVertexBuffer(&batch->m_VertexBuffer, (const char *)batch->m_Vertices,
										   batch->m_NumSpritesToDraw * 4 * sizeof(Vertex));
		Context::BindVertexBuffer(&batch->m_VertexBuffer);
		Context::BindIndexBuffer(&batch->m_IndexBuffer);
		Context::BindTexture(&batch->m_Texture);
		Context::BindLayout(&batch->m_Layout);
		Context::Submit(batch->m_NumSpritesToDraw);

		// Reset Batch
		// batch->m_Vertices.clear();
		batch->m_NumSpritesToDraw = 0;
	}

	// Rendering System Lifetime
	// --------------------------------------------------------------------------

	void RenderingSystem::InitSignature() {
		// SetRequiredComponent<TransformComponent>();
		// SetRequiredComponent<RenderComponent>();

		m_Signature.set(g_Admin->GetComponentSignatureID<TransformComponent>(), true);
		m_Signature.set(g_Admin->GetComponentSignatureID<RenderComponent>(), true);

		m_RendereableView = g_Admin->CreateView(m_Signature);
	}

	void RenderingSystem::Init() {
		Context::Init();

		// Init IMGUI
		ImGuiRenderer::Init();

		// Init Batch Rendering
		u32 MAX_SPRITES = 5000;
		u32 MAX_INDICES = MAX_SPRITES * 6;
		u32 MAX_VERTICES = MAX_SPRITES * 4;

		batch.m_MaxSpritesToDraw = MAX_SPRITES;
		batch.m_VertexBuffer = Device::CreateDynamicVertexBuffer(MAX_SPRITES);

		indices = new u32[MAX_INDICES];
		batch.m_Vertices = new Vertex[MAX_VERTICES];
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

	void RenderingSystem::Shutdown() {
		ImGuiRenderer::Shutdown();

		delete indices;
		delete batch.m_Vertices;
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

		auto transforms = g_Admin->GetComponentArray<TransformComponent>();
		auto render = g_Admin->GetComponentArray<RenderComponent>();

		for (auto const &entityID : m_RendereableView->m_Entities) {
			TransformComponent *t = transforms->Get(entityID);
			RenderComponent *r = render->Get(entityID);

			SpriteRenderData *sp = g_ResourcesDatabase.GetSpriteData(r->m_SpriteHandle);

			// This approach means that we don't guarantee drawing
			// all the sprites with the same texture in the same batch
			if (batch.m_Texture.m_ID != sp->m_Texture.m_ID) {
				if (batch.m_NumSpritesToDraw > 0) {
					FlushBatch(&batch);
					++numBatches;
				}
				batch.m_Texture = sp->m_Texture;
				batch.m_Layout = sp->m_Layout;
				batch.m_Program = sp->m_Program;
			}

			if (batch.m_NumSpritesToDraw >= batch.m_MaxSpritesToDraw - 1) {
				FlushBatch(&batch);
				++numBatches;
				batch.m_Texture = sp->m_Texture;
				batch.m_Layout = sp->m_Layout;
				batch.m_Program = sp->m_Program;
			}

			AddQuad(&batch, t->m_Position, sp->m_Width, sp->m_Height, sp->m_PixelsPerUnit);
		}

		FlushBatch(&batch);
		++numBatches;

		// IMGUI Rendering
		ImGuiRenderer::NewFrame();
		ImGui::LabelText("FPS", "%f", 1.0f / dt);
		ImGui::LabelText("FrameTime", "%f ms", (f64)dt * 1000.0f);
		ImGui::LabelText("Batches", "%d", numBatches);
		ImGuiRenderer::Render();

		// Swap Buffers
		glfwSwapBuffers(g_AppData.m_Window.m_Handle);
	}


	// Previous rendering Loop

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

} // namespace Cookie