#include "FileSystem/FileSystem.h"
#include "Logging/Log.h"

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
			FileSystem::WriteToFile("test.bin", (char *)data, sizeof(data));

			Dt *dataRead = new Dt[2];
			FileSystem::ReadFile("test.bin", (char *)dataRead, sizeof(data));

			if (dataRead[0].a == 10 && dataRead[1].a == 25) {
				CKE_LOG_INFO("File System Tests Passed");
			} else {
				CKE_LOG_INFO("File System Tests FAILED");
			}
		}

	} // namespace FileSystem
} // namespace Cookie
