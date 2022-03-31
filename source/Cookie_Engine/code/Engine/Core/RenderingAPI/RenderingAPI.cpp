#include "RenderingAPI.h"
#include "Core/Application.h"
#include "Core/FileSystem/FileSystem.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Cookie {
namespace RenderingAPI {

	// returns the OpenGL representation of the
	// provided DataType
	int GetOpenGLDataType(DataType type) {
		switch (type) {
		case UINT:
			return GL_UNSIGNED_INT;
		case INT:
			return GL_INT;
		case FLOAT:
			return GL_FLOAT;
		default:
			return 0;
		}
	};

	// Returns the size of the provided data type
	int GetDataTypeSize(DataType type) {
		switch (type) {
		case UINT:
			return sizeof(u32);
		case INT:
			return sizeof(i32);
		case FLOAT:
			return sizeof(f32);
		default:
			return 0;
		}
	};

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

	void VertexArray::SetLayout(VertexArrayLayout *layout) {
		m_Layout = layout;
		u32 count = layout->m_Attributes.size();
		for (u32 i = 0; i < count; i++) {
			LayoutAttribute *attr = &layout->m_Attributes[i];
			glVertexAttribPointer(attr->m_Pos, attr->m_Count, GetOpenGLDataType(attr->m_Type),
								  attr->m_Normalized, layout->m_Stride,
								  (void *)(UINT_PTR)attr->m_Offset);
			glEnableVertexAttribArray(attr->m_Pos);
		}
	};

	LayoutAttribute::LayoutAttribute(u32 pos, DataType type, u32 count, bool normalized)
		: m_Pos(pos), m_Type(type), m_Count(count), m_Normalized(normalized),
		  m_Size(GetDataTypeSize(type) * count), m_Offset(-1){};

	void VertexArrayLayout::AddAttribute(LayoutAttribute attr) {
		m_Attributes.push_back(attr);
		attr.m_Size = GetDataTypeSize(attr.m_Type) * attr.m_Count;
		m_Stride = 0;
		u32 offset = 0;
		for (u32 i = 0; i < m_Attributes.size(); i++) {
			m_Attributes[i].m_Offset = offset;
			offset += m_Attributes[i].m_Size;
			m_Stride += m_Attributes[i].m_Size;
		};
	};

	void Program::SetUniformMat4(const char *name, const float *data) {
		glUniformMatrix4fv(glGetUniformLocation(m_DeviceID, name), 1, GL_FALSE, data);
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

		IndexBuffer Device::CreateIndexBuffer(char *data, u32 size, DataType type) {
			IndexBuffer ib;
			ib.m_Size = size;
			ib.m_DataType = type;
			ib.m_IndexCount = size / GetDataTypeSize(type);

			glGenBuffers(1, &ib.m_DeviceID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib.m_DeviceID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
			// This might not be necessary
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			return ib;
		};

		Texture CreateTexture(unsigned char *data, i32 width, i32 height) {
			Texture t;

			glGenTextures(1, &t.m_ID);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, t.m_ID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
						 data);
			return t;
		};

		Program Device::CreateProgram(const char *vertShaderPath, const char *fragShaderPath) {
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
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			}

			u32 fragShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragShader, 1, &fragSource, NULL);
			glCompileShader(fragShader);
			glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
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
						   GetOpenGLDataType(va->m_IndexBuffer->m_DataType), nullptr);
		}

		void Context::BindProgram(Program *p) { glUseProgram(p->m_DeviceID); }

		void Init() {
			// Set current OpenGL context to the window
			glfwMakeContextCurrent(Application::appData.m_Window);

			// Setup OpenGL function loading
			gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

			// Viewport settings and resize
			glViewport(0, 0, 1280, 720);
			glfwSetFramebufferSizeCallback(
				Application::appData.m_Window,
				[](GLFWwindow *window, i32 width, i32 height) { glViewport(0, 0, width, height); });
		};

		void Context::ClearColorBuffer(float r, float g, float b, float a) {
			glClearColor(r, g, b, a);
			glClear(GL_COLOR_BUFFER_BIT);
		}

	} // namespace Context

} // namespace RenderingAPI
} // namespace Cookie