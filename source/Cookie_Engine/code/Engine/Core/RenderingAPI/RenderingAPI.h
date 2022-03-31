#include "Core/Common.h"
#include "Core/Math.h"

namespace Cookie {
namespace RenderingAPI {

	struct VertexBuffer {
		u32 m_DeviceID;
		u32 m_Size;
		// Source data ref
	};

	enum DataType { UINT, INT, FLOAT };

	struct IndexBuffer {
		u32 m_DeviceID;
		u32 m_Size;
		DataType m_DataType;
		u32 m_IndexCount; // Amount of indices in the buffer
	};

	struct LayoutAttribute {
		// These parameters are provided
		// on creation
		u32 m_Pos;
		DataType m_Type;
		u32 m_Count;
		bool m_Normalized;

		// These are derivated from the
		// previous ones
		u32 m_Size;
		u32 m_Offset;

		LayoutAttribute(u32 pos, DataType type, u32 count, bool normalized);
	};

	struct VertexArrayLayout {
		std::vector<LayoutAttribute> m_Attributes;
		u32 m_Stride;

		void AddAttribute(LayoutAttribute attr);
	};

	struct VertexArray {
		u32 m_DeviceID;
		VertexBuffer *m_VertexBuffer;
		IndexBuffer *m_IndexBuffer;
		VertexArrayLayout *m_Layout;
		// Layout

		void BindVertexBuffer(VertexBuffer *vb);
		void BindIndexBuffer(IndexBuffer *ib);
		void SetLayout(VertexArrayLayout *layout);
	};

	struct Program {
		u32 m_DeviceID;
		// Frag Shader
		// Vert Shader
		void SetUniformMat4(const char *name, const float *data);
	};

	struct FragmentShader {
		u32 m_DeviceID;
		// Source Frag Shader
	};

	struct VertexShader {
		u32 m_DeviceID;
		// Source Vert Shader
	};

	struct Texture {
		u32 m_ID;
	};

	namespace Device {
		VertexArray CreateVertexArray();
		VertexBuffer CreateVertexBuffer(char *data, u32 size);
		IndexBuffer CreateIndexBuffer(char *data, u32 size, DataType type);
		Texture CreateTexture(unsigned char *data, i32 width, i32 height);
		Program CreateProgram(const char *vertShaderPath, const char *fragShaderPath);

		void DeleteProgram();

	} // namespace Device

	namespace Context {
		void BindVertexArray(VertexArray *va);
		void BindVertexBuffer(VertexBuffer *vb);
		void BindIndexBuffer(IndexBuffer *ib);
		void BindProgram(Program *p);

		void Init();
		void ClearColorBuffer(float r, float g, float b, float a);
		void DrawIndexed(VertexArray *va);
	} // namespace Context

} // namespace RenderingAPI
} // namespace Cookie