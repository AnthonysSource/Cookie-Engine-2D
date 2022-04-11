#include "Resources.h"

#include <stb_image.h>

namespace Cookie {

namespace Image {

	ImageCPU Load(const char *path) {
		ImageCPU i;
		i.m_Data = stbi_load(path, &i.m_Width, &i.m_Height, &i.m_NrChannels, 0);
		return i;
	}

	void Release(ImageCPU *image) { stbi_image_free(image->m_Data); }

} // namespace Image

} // namespace Cookie
