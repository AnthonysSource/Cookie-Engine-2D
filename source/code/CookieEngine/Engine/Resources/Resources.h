#include "Core/Types/PrimitiveTypes.h"
#include "Core/Types/Containers.h"
#include "Core/Defines.h"

#include "Core/RenderAPI/RenderAPI.h"

namespace Cookie {

	// --------------------------------------------------------------------------

	struct ImageCPU {
		i32 m_Width;
		i32 m_Height;
		i32 m_NrChannels;
		unsigned char *m_Data;
	};

	struct SpriteRenderData {
		RenderingAPI::VertexArray m_VertexArray;
		RenderingAPI::VertexBuffer m_VertexBuffer;
		RenderingAPI::IndexBuffer m_IndexBuffer;
		RenderingAPI::Program m_Program;
		RenderingAPI::Texture m_Texture;

		TVector<float> m_Vertices;
		TVector<u32> m_Indices;
	};

	struct ResourcesDatabase {
		THashMap<u32, SpriteRenderData *> m_Sprites;

		CKE_FORCE_INLINE SpriteRenderData *GetSpriteData(u32 spriteID) {
			CKE_ASSERT(m_Sprites.count(spriteID) != 0, "Acessing a sprite that doesn't exist");
			return m_Sprites[spriteID];
		}
	};

	// --------------------------------------------------------------------------

	extern ResourcesDatabase g_ResourcesDatabase;

	// --------------------------------------------------------------------------

	namespace ResourcesSystem {

		void Init();
		void Shutdown();

		// Sprites
		u32 GenerateSprite(ImageCPU image, f32 pixelsPerUnit);
		void DeleteSprite(u32 spriteID);

		// Images
		//
		// Load a image on the CPU, the data
		// is stored as unsigned char
		// This internally allocates memory for the data
		ImageCPU Load(const char *path);
		// Release the memory of the cpu image
		void Release(ImageCPU *image);


	} // namespace ResourcesSystem

	// --------------------------------------------------------------------------

} // namespace Cookie
