#include "Log.h"

#include "Core/FileSystem/FileSystem.h"

#include <stdio.h>
#include <cstdarg>
#include <iostream>

namespace Cookie::Log {

	// The minimum severity log entries need to have
	// to be outputted to the console
	namespace {

#define DEFINE_CHARS(def) #def,
		static char const *const g_VerbosityLabels[] = {CKE_LOG_VERBOSITY_DEFINITIONS(DEFINE_CHARS)};
		static char const *const g_ChannelLabels[] = {CKE_LOG_CHANNEL_DEFINITIONS(DEFINE_CHARS)};
#undef DEFINE_CHARS


		Verbosity g_MinVerbosity = Verbosity::Message;
		bool g_LogOriginCodeFile = false;


		FileSystem::OutputFileStream g_GlobalLogFile;

	} // namespace

	void Initialize() { g_GlobalLogFile.open("EngineLog.txt"); }

	void Shutdown() { g_GlobalLogFile.close(); }

	i32 VFormat(char *buffer, u32 bufferSize, char const *format, va_list argsList) {
		return vsnprintf(buffer, bufferSize, format, argsList);
	}

	i32 Format(char *buffer, u32 bufferSize, char const *format, ...) {
		va_list argsList;
		va_start(argsList, format);
		i32 charsCount = VFormat(buffer, bufferSize, format, argsList);
		va_end(argsList);

		return charsCount;
	}

	void AddEntry(Verbosity verbosity, Channel channel, char const *fileName, u32 lineNumber, char const *format, ...) {

		if (verbosity >= g_MinVerbosity) {
			va_list argsList;
			va_start(argsList, format);

			VAddEntry(verbosity, channel, fileName, lineNumber, format, argsList);

			va_end(argsList);
		}
	}

	void VAddEntry(Verbosity verbosity, Channel channel, char const *fileName, u32 lineNumber, char const *format, va_list argsList) {
		const u32 MAX_CHARS = 1024;
		static char s_LogBuffer[MAX_CHARS];

		// We first format the message with the arguments provided
		VFormat(s_LogBuffer, MAX_CHARS, format, argsList);

		// We need to make a copy of the formatted message
		String msg = s_LogBuffer;

		// We create the final message
		// LogFile Workaround
		if (g_LogOriginCodeFile) {
			Format(s_LogBuffer, MAX_CHARS, "[%s][%s : %u][%s] %s", g_ChannelLabels[(u32)channel], fileName, lineNumber,
				   g_VerbosityLabels[(u32)verbosity], msg.c_str());
		} else {
			Format(s_LogBuffer, MAX_CHARS, "[%s][%s] %s", g_ChannelLabels[(u32)channel], g_VerbosityLabels[(u32)verbosity], msg.c_str());
		}

		printf("%s\n", s_LogBuffer);

		g_GlobalLogFile << s_LogBuffer << std::endl;
	}

	void BasicEntry(char const *format, ...) {
		va_list argsList;
		va_start(argsList, format);
		VBasicEntry(format, argsList);
		va_end(argsList);
	}

	void VBasicEntry(char const *format, va_list argsList) {
		const u32 MAX_CHARS = 1024;
		static char s_LogBuffer[MAX_CHARS];

		vsnprintf(s_LogBuffer, MAX_CHARS, format, argsList);

		printf("%s\n", s_LogBuffer);
	}

} // namespace Cookie::Log