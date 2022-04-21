#include "Resources.h"

#include <stb_image.h>

namespace Cookie {

	// --------------------------------------------------------------------------

	ResourcesDatabase g_ResourcesDatabase;

	// --------------------------------------------------------------------------

	namespace ResourcesSystem {

		TQueue<u32> m_AvailableIDs{};

		void Init() {
			//	Initialize all the resource IDs
			for (size_t i = 0; i < 1000; i++) {
				m_AvailableIDs.push(i);
			}
		}

		void Shutdown() {
			// Release all the Sprites
			for (auto pair : g_ResourcesDatabase.m_Sprites) {
				delete pair.second;
			}
			g_ResourcesDatabase.m_Sprites.clear();
		}

		// Images
		// --------------------------------------------------------------------------
		ImageHandle Load(const char *path) {
			ImageCPU *i = new ImageCPU();
			stbi_set_flip_vertically_on_load(true);
			i->m_Data = stbi_load(path, &i->m_Width, &i->m_Height, &i->m_NrChannels, 0);

			ImageHandle handle = m_AvailableIDs.front();
			m_AvailableIDs.pop();
			g_ResourcesDatabase.m_Images[handle] = i;
			return handle;
		}

		void Release(ImageHandle handle) {
			// Delete the image from the database
			// and free the image data
			auto img = g_ResourcesDatabase.m_Images[handle];
			stbi_image_free(g_ResourcesDatabase.m_Images[handle]->m_Data);
			delete img;
			g_ResourcesDatabase.m_Images.erase(handle);
		}

		// Sprites
		// --------------------------------------------------------------------------
		SpriteHandle GenerateSprite(ImageHandle imgHandle, f32 pixelsPerUnit) {

			auto image = g_ResourcesDatabase.GetImage(imgHandle);

			SpriteRenderData *sp = new SpriteRenderData();

			using namespace RenderingAPI;

			sp->m_VertexArray = Device::CreateVertexArray();

			// Quad
			// clang-format off
			sp->m_Vertices = { -image->m_Width / (2.0f * pixelsPerUnit), -image->m_Height / (2.0f * pixelsPerUnit), 0.0f,   0.0f, 0.0f, 
								image->m_Width / (2.0f * pixelsPerUnit), -image->m_Height / (2.0f * pixelsPerUnit), 0.0f,   1.0f, 0.0f,
							    image->m_Width / (2.0f * pixelsPerUnit),  image->m_Height / (2.0f * pixelsPerUnit), 0.0f,   1.0f, 1.0f, 
							   -image->m_Width / (2.0f * pixelsPerUnit),  image->m_Height / (2.0f * pixelsPerUnit), 0.0f,   0.0f, 1.0f};
			sp->m_Indices = {0, 1, 2,
							 2, 3, 0};
			// clang-format on

			// Vertex & Index Buffer
			sp->m_VertexBuffer = Device::CreateVertexBuffer((char *)sp->m_Vertices.data(), sizeof(float) * sp->m_Vertices.size());
			sp->m_IndexBuffer = Device::CreateIndexBuffer((char *)sp->m_Indices.data(), sizeof(u32) * sp->m_Indices.size(), UINT);

			// Program
			sp->m_Program = Device::CreateProgram("shaders/Sprite.vert", "shaders/Sprite.frag");

			// VAO Layout
			VertexArrayLayout layout;
			layout.AddAttribute(LayoutAttribute(0, FLOAT, 3, false));
			layout.AddAttribute(LayoutAttribute(1, FLOAT, 2, false));

			// VAO Init
			sp->m_VertexArray.BindIndexBuffer(&sp->m_IndexBuffer);
			sp->m_VertexArray.BindVertexBuffer(&sp->m_VertexBuffer);
			sp->m_VertexArray.SetLayout(&layout);

			sp->m_Texture = Device::CreateTexture(image->m_Data, image->m_Width, image->m_Height);

			// Currently creating a texture binds it to the last used
			// vertex array
			u32 handle = m_AvailableIDs.front();
			m_AvailableIDs.pop();
			g_ResourcesDatabase.m_Sprites[handle] = sp;

			return handle;
		}

		void DeleteSprite(SpriteHandle handle) {
			SpriteRenderData *sp = g_ResourcesDatabase.m_Sprites[handle];
			delete sp;
			g_ResourcesDatabase.m_Sprites.erase(handle);
		}

	} // namespace ResourcesSystem

} // namespace Cookie
