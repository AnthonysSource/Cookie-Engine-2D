#include "FileSystem.h"
#include "Logging.h"

namespace Cookie {
namespace FileSystem {

struct Dt {
	int a;
	float b;
};

void RunFileSystemTest() {
	Dt data[2];
	data[0].a = 10;
	data[0].b = 3.1415;
	data[1].a = 25;
	data[1].b = 7.4754;
	FileSystem::WriteToFile("test", (char *)data, sizeof(data));

	Dt *dataRead = new Dt[2];
	FileSystem::ReadFile("test", (char *)dataRead, sizeof(data));

	if (dataRead[0].a == 10 && dataRead[1].b == 7.4754) {
		Log::Info("File System Tests Passed");
	} else {
		Log::Info("File System Tests FAILED");
	}
}

} // namespace FileSystem
} // namespace Cookie
