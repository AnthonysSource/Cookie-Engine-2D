#include "FileSystem.h"
#include "Ckpch.h"

namespace Cookie {
namespace FileSystem {

// write to a binary file
void FileSystem::WriteToFile(const char *path, const char *data,
							 const size_t size) {
	std::ofstream stream(path, std::ios::out | std::ios::binary);
	stream.write(data, size);
	stream.close();
};

// Read a binary file and store its content in the output buffer
void FileSystem::ReadFile(const char *path, char *outputBuffer, const size_t size) {
	std::ifstream stream(path, std::ios::in | std::ios::binary);
	stream.read(outputBuffer, size);
	stream.close();
}

std::string FileSystem::ReadTextFile(const char *path){
	std::string fileText;
	std::ifstream fileStream;
	fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		fileStream.open(path);
		std::stringstream textStream;
		textStream << fileStream.rdbuf();
		fileStream.close();
		fileText = textStream.str();
	}
	catch(const std::ifstream::failure e)
	{
		std::cerr << e.what() << '\n';
	}
	return fileText;
}

// void FileSystem::Write(char *data) { wf.write((char *)&data[i], sizeof(Dt));
// }

} // namespace FileSystem
} // namespace Cookie