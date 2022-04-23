#pragma once

#include "Core/Types/PrimitiveTypes.h"
#include "Core/Types/Pointers.h"
#include "Core/Types/String.h"

#include "Core/Types/Containers.h"

namespace Cookie::Log {

	//-------------------------------------------------------------------------

#define CKE_LOG_VERBOSITY_DEFINITIONS(F)                                                                                                   \
	F(Detail)                                                                                                                              \
	F(Normal)                                                                                                                              \
	F(Warning)                                                                                                                             \
	F(Error)                                                                                                                               \
	F(CriticalError)

#define CKE_LOG_CHANNEL_DEFINITIONS(F)                                                                                                     \
	F(Core)                                                                                                                                \
	F(Rendering)                                                                                                                           \
	F(Input)                                                                                                                               \
	F(FileSystem)

#define DEFINE_ENUM(def) def,
	enum Verbosity : u8 { CKE_LOG_VERBOSITY_DEFINITIONS(DEFINE_ENUM) };
	enum Channel : u8 { CKE_LOG_CHANNEL_DEFINITIONS(DEFINE_ENUM) };
#undef DEFINE_ENUM

	//-------------------------------------------------------------------------

	struct LogEntry {
		String m_TimeStamp;
		String m_Message;
		Bitset<32> m_Channels;

		String m_FileName;
		String m_FunctionName;
		u32 m_LineNumber;

		Verbosity m_Verbosity;
	};

	//-------------------------------------------------------------------------

	void Initialize();
	void Shutdown();

	//-------------------------------------------------------------------------

	void AddEntry(Verbosity verbosity, Channel channel, char const *fileName, u32 lineNumber, char const *format, ...);
	void VAddEntry(Verbosity verbosity, Channel channel, char const *fileName, u32 lineNumber, char const *format, va_list argsList);

	void BasicEntry(char const *format, ...);
	void VBasicEntry(char const *format, va_list argsList);

} // namespace Cookie::Log

// Macros
//-------------------------------------------------------------------------

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#ifdef COOKIE_LOGGING
	#define CKE_LOG_INFO(...)                                                                                                              \
		{ Cookie::Log::BasicEntry(__VA_ARGS__); }
	#define CKE_LOG_NORMAL(CHANNEL, ...) Cookie::Log::AddEntry(Cookie::Log::Verbosity::Normal, CHANNEL, __FILENAME__, __LINE__, __VA_ARGS__)
#else
	#define CKE_LOG_INFO(...)
	#define CKE_LOG_WARNING(...)
	#define CKE_LOG_ERROR(...)
	#define CKE_LOG_ASSERT(...)
#endif

//-------------------------------------------------------------------------