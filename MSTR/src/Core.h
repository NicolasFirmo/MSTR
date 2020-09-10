#pragma once

#define CONCAT(a,b) THIS_IS_STUPID(a,b)
#define THIS_IS_STUPID(a,b) a ## b
#define APPLINE(x) CONCAT(x,__LINE__)

#define PRAGMA(x) __pragma(x)

#ifndef NDEBUG
#define	DEBUG_BREAK() __debugbreak()
#define DebugLog(x) std::cerr << x;
#else
#define DebugLog(x)
#endif

#ifndef NDEBUG
#define NIC_ASSERT(x,s) if(!(x)){ DebugLog("ASSERTION FALIED: " << s << "\nLINE: " << __LINE__ << "\nFILE: " << __FILE__ << '\n'); DEBUG_BREAK();}
#else
#define NIC_ASSERT(x,s) PRAGMA(warning(disable: 552 553 834)) x; PRAGMA(warning(default: 552 553 834))
#endif

#ifndef NDEBUG
#define GLCall(x) while(glGetError()); x; while(auto error = glGetError())NIC_ASSERT(false, std::hex << "[OpenGL error] (0x0" << error << std::dec << "): " << #x)
#else
#define GLCall(x) x
#endif
