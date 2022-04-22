#pragma once

#include "Core/Types/PrimitiveTypes.h"
#include "Core/Types/Pointers.h"
#include "Core/Types/String.h"

namespace Cookie::Log {

	enum class Verbosity {
		Detail = 0,
		Normal,
		Warning,
		Error,
		CriticalError,
	};

	struct LogEntry {
		String m_TimeStamp;
		String m_Message;
		String m_Channel;

		String m_FileName;
		String m_FunctionName;
		u32 m_LineNumber;

		Verbosity m_Verbosity;
	};


	void Initialize();
	void Shutdown();


	void AddEntry(Verbosity verbosity, char const *channel, char const *fileName, int lineNumber, char const *format, ...);
	void VAddEntry(Verbosity verbosity, char const *channel, char const *fileName, int lineNumber, char const *format, va_list argsList);

	void BasicEntry(char const *format, ...);
	void VBasicEntry(char const *format, va_list argsList);

} // namespace Cookie::Log

#ifdef COOKIE_LOGGING
	#define CKE_LOG_INFO(...)                                                                                                              \
		{ Cookie::Log::BasicEntry(__VA_ARGS__); }
#else
	#define CKE_LOG_INFO(...)
	#define CKE_LOG_WARNING(...)
	#define CKE_LOG_ERROR(...)
	#define CKE_LOG_ASSERT(...)
#endif