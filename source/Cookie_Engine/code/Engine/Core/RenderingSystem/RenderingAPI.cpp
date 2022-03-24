#include "Core/BasicTypes.h"

namespace Cookie {
namespace RenderingAPI {

struct VertexBuffer {
	u32 m_DeviceID;
	// Source data ref
};

struct IndexBuffer {
	u32 m_DeviceID;
	u32 m_IndexCount; // Amount of indices in the buffer
};

struct VertexArray {
	u32 m_DeviceID;
	VertexBuffer *m_VertexBuffer;
	IndexBuffer *m_IndexBuffer;
	// Layout
};

struct Program {
	u32 m_DeviceID;
	// Frag Shader
	// Vert Shader
};

struct FragmentShader {
	u32 m_DeviceID;
	// Source Frag Shader
};

struct VertexShader {
	u32 m_DeviceID;
	// Source Vert Shader
};

struct MeshData {};

struct RenderComponent {
	u32 m_MeshID;
	u32 m_MaterialID;
};

std::unordered_map<u32, MeshData> m_MeshRegistry;

namespace Device {

VertexArray CreateVertexArray();
VertexBuffer CreateVertexBuffer();
IndexBuffer CreateIndexBuffer();
Program CreateProgram();

void DeleteProgram();

void ClearColorBuffer(float r, float g, float b, float a);

} // namespace Device

namespace Context {

void BindVertexArray(VertexArray *va);
void BindVertexBuffer(VertexBuffer *vb);
void BindIndexBuffer(IndexBuffer *ib);
void BindProgram(Program *p);

void DrawIndexed(VertexArray *va);

} // namespace Context

} // namespace RenderingAPI
} // namespace Cookie