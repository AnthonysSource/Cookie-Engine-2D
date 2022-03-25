#include "RenderingAPI.h"
#include "Core/FileSystem/FileSystem.h"

#include <glad/glad.h>

namespace Cookie {
namespace RenderingAPI {

void VertexArray::BindVertexBuffer(VertexBuffer *vb) {
	m_VertexBuffer = vb;
	glBindVertexArray(m_DeviceID);
	glBindBuffer(GL_ARRAY_BUFFER, vb->m_DeviceID);
};

void VertexArray::BindIndexBuffer(IndexBuffer *ib) {
	m_IndexBuffer = ib;
	glBindVertexArray(m_DeviceID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->m_DeviceID);
};

namespace Device {

VertexArray Device::CreateVertexArray() {
	VertexArray va;
	glGenVertexArrays(1, &va.m_DeviceID);
	return va;
};

VertexBuffer Device::CreateVertexBuffer(char *data, u32 size) {
	VertexBuffer vb;
	vb.m_Size = size;

	glGenBuffers(1, &vb.m_DeviceID);
	glBindBuffer(GL_ARRAY_BUFFER, vb.m_DeviceID);
	// TODO: Add options to change gl_static_draw
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	// This might not be necessary
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vb;
};

int GetOpenGLDataType(DataType type) {
	switch (type) {
	case UINT_32:
		return GL_UNSIGNED_INT;
		break;
	default:
		return 0;
		break;
	}
	return 0;
};

IndexBuffer Device::CreateIndexBuffer(char *data, u32 size, DataType type) {
	IndexBuffer ib;
	ib.m_Size = size;
	ib.m_DataType = type;
	ib.m_IndexCount = size / type;

	glGenBuffers(1, &ib.m_DeviceID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib.m_DeviceID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	// This might not be necessary
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return ib;
};

Program Device::CreateProgram(const char *vertShaderPath,
							  const char *fragShaderPath) {
	Program p;
	std::string vertexString = FileSystem::ReadTextFile(vertShaderPath);
	std::string fragString = FileSystem::ReadTextFile(fragShaderPath);
	const char *vertexSource = vertexString.c_str();
	const char *fragSource = fragString.c_str();

	u32 vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertexSource, NULL);
	glCompileShader(vertShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}

	u32 fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragSource, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}

	u32 program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	glDetachShader(program, vertShader);
	glDetachShader(program, fragShader);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	p.m_DeviceID = program;
	return p;
};

// void DeleteProgram();
// void ClearColorBuffer(float r, float g, float b, float a);

} // namespace Device

namespace Context {

void DrawIndexed(VertexArray *va) {
	glBindVertexArray(va->m_DeviceID);

	// We need to store this parameters in the va
	glDrawElements(GL_TRIANGLES, va->m_IndexBuffer->m_IndexCount,
				   Device::GetOpenGLDataType(va->m_IndexBuffer->m_DataType),
				   nullptr);
}

void Context::BindProgram(Program *p) { glUseProgram(p->m_DeviceID); }

} // namespace Context

} // namespace RenderingAPI
} // namespace Cookie