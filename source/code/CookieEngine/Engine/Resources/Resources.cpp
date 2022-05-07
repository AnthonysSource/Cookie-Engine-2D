#include "Resources.h"

#include <stb_image.h>

using namespace Cookie::RenderingAPI;

namespace Cookie {

	// --------------------------------------------------------------------------

	ResourcesDatabase g_ResourcesDatabase;

	// --------------------------------------------------------------------------

	// Core Engine Resources That will always be loaded
	Program defaultSpriteProgram;

	// --------------------------------------------------------------------------

	namespace ResourcesSystem {

		TQueue<u32> m_AvailableIDs{};

		// System Lifetime
		// --------------------------------------------------------------------------
		void Init() {
			//	Initialize all the resource IDs
			for (u32 i = 0; i < 1000; i++) {
				m_AvailableIDs.push(i);
			}

			// Init Core Engine Resources
			defaultSpriteProgram = Device::CreateProgram("shaders/Sprite.vert", "shaders/Sprite.frag");
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
		ImageHandle LoadImage(const char *path) {
			ImageCPU *i = new ImageCPU();
			stbi_set_flip_vertically_on_load(true);
			i->m_Data = stbi_load(path, &i->m_Width, &i->m_Height, &i->m_NrChannels, 0);

			ImageHandle handle = m_AvailableIDs.front();
			m_AvailableIDs.pop();
			g_ResourcesDatabase.m_Images[handle] = i;
			return handle;
		}

		void ReleaseImage(ImageHandle handle) {
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

			// Get image from the database
			auto image = g_ResourcesDatabase.GetImage(imgHandle);

			SpriteRenderData *sp = new SpriteRenderData();

			using namespace RenderingAPI;

			sp->m_Height = image->m_Height;
			sp->m_Width = image->m_Width;
			sp->m_PixelsPerUnit = pixelsPerUnit;

			// Program
			sp->m_Program = defaultSpriteProgram;

			// Layout
			sp->m_Layout.AddAttribute(LayoutAttribute(0, FLOAT, 3, false));
			sp->m_Layout.AddAttribute(LayoutAttribute(1, FLOAT, 2, false));

			// Texture
			sp->m_Texture = Device::CreateTexture(image->m_Data, image->m_Width, image->m_Height);

			// Give the data an ID and add it to the Resources Database
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
