#include "Log.h"

#include <stdio.h>
#include <cstdarg>
#include <iostream>

namespace Cookie::Log {

	void Initialize() {}

	void Shutdown() {}


	void AddEntry(Verbosity severity, char const *channel, char const *fileName, int lineNumber, char const *format, ...) {
		va_list argsList;
		va_start(argsList, format);

		VAddEntry(severity, channel, fileName, lineNumber, format, argsList);

		va_end(argsList);
	}

	void VAddEntry(Verbosity severity, char const *channel, char const *fileName, int lineNumber, char const *format, va_list argsList) {
		const u32 MAX_CHARS = 1024;
		static char s_LogBuffer[MAX_CHARS];

		vsnprintf(s_LogBuffer, MAX_CHARS, format, argsList);

		printf("%s\n", s_LogBuffer);
	}

	void BasicEntry(char const *format, ...) {
		va_list argsList;
		va_start(argsList, format);

		BasicEntry(format, argsList);

		va_end(argsList);
	}

	void BasicEntry(char const *format, va_list argsList) {
		const u32 MAX_CHARS = 1024;
		static char s_LogBuffer[MAX_CHARS];

		vsnprintf(s_LogBuffer, MAX_CHARS, format, argsList);

		printf("%s\n", s_LogBuffer);
	}

} // namespace Cookie::Log