#include "RenderingAPI.h"
#include <glad/glad.h>

namespace Cookie {
namespace RenderingAPI {
namespace Device {

VertexArray Device::CreateVertexArray() {
	VertexArray va;
	glGenVertexArrays(1, &va.m_DeviceID);

	// We shouldn't bind it here but we will temporarily
	glBindVertexArray(va.m_DeviceID);
	return va;
}

VertexBuffer Device::CreateVertexBuffer(char *data, u32 size) {
	VertexBuffer vb;
	vb.m_Size = size;
	glGenBuffers(1, &vb.m_DeviceID);
	glBindBuffer(GL_ARRAY_BUFFER, vb.m_DeviceID);
	// TODO: Add options to change gl_static_draw
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	return vb;
}

IndexBuffer Device::CreateIndexBuffer(char *data, u32 size) {
	IndexBuffer ib;
	glGenBuffers(1, &ib.m_DeviceID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib.m_DeviceID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	return ib;
};

// Program Device::CreateProgram();
// void DeleteProgram();
// void ClearColorBuffer(float r, float g, float b, float a);

} // namespace Device

namespace Context {

void DrawIndexed(VertexArray *va) {
	glBindVertexArray(va->m_DeviceID);

	// We need to store this parameters in the va
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

} // namespace Context

} // namespace RenderingAPI
} // namespace Cookie