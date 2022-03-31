#include "Core/Common.h"

namespace Cookie {

struct ImageCPU {
	i32 m_Width;
	i32 m_Height;
	i32 m_NrChannels;
	unsigned char *m_Data;
};

namespace Image {

	// Load a image on the CPU, the data
	// is stored as unsigned char
	// This internally allocates memory for the data
	ImageCPU Load(const char *path);

	// Release the memory of the cpu image
	void Release(ImageCPU *image);

} // namespace Image

} // namespace Cookie
