#pragma once

#include "Core/Types/PrimitiveTypes.h"
#include "Core/Types/Containers.h"
#include "Core/Defines.h"

#include "Render/RenderAPI.h"

namespace Cookie {

	// --------------------------------------------------------------------------
	using ImageHandle = u32;
	using SpriteHandle = u32;

	// Image loaded in the CPU
	struct ImageCPU {
		i32 m_Width;
		i32 m_Height;
		i32 m_NrChannels;
		unsigned char *m_Data;
	};

	// All the data used by the render API to
	// render this sprite
	struct SpriteRenderData {
		RenderingAPI::VertexBuffer m_VertexBuffer;
		RenderingAPI::IndexBuffer m_IndexBuffer;
		RenderingAPI::Layout m_Layout;
		RenderingAPI::Program m_Program;
		RenderingAPI::Texture m_Texture;

		TVector<RenderingAPI::Vertex> m_Vertices{};
		TVector<u32> m_Indices{};
	};

	struct ResourcesDatabase {
		THashMap<SpriteHandle, SpriteRenderData *> m_Sprites;
		THashMap<ImageHandle, ImageCPU *> m_Images;

		CKE_FORCE_INLINE SpriteRenderData *GetSpriteData(u32 spriteID) {
			CKE_ASSERT(m_Sprites.count(spriteID) != 0, "Acessing a sprite that doesn't exist");
			return m_Sprites[spriteID];
		}

		CKE_FORCE_INLINE ImageCPU *GetImage(u32 imageID) {
			CKE_ASSERT(m_Images.count(imageID) != 0, "Acessing an image that doesn't exist");
			return m_Images[imageID];
		}
	};

	// --------------------------------------------------------------------------

	extern ResourcesDatabase g_ResourcesDatabase;

	// --------------------------------------------------------------------------

	namespace ResourcesSystem {

		void Init();
		void Shutdown();

		// Sprites
		SpriteHandle GenerateSprite(ImageHandle imgHandle, f32 pixelsPerUnit);
		void DeleteSprite(SpriteHandle handle);

		// Images
		ImageHandle LoadImage(const char *path);
		void ReleaseImage(ImageHandle handle);

	} // namespace ResourcesSystem

	// --------------------------------------------------------------------------

} // namespace Cookie
