#ifndef __IREXTRAS_DEFS__
#define __IREXTRAS_DEFS__

#if _WIN32 && !IREXTRAS_STATIC
#   define __CALLCONV __cdecl
#   ifdef LIBIREXTRAS_EXPORTS
#       define __IREXTRAS_API__ __declspec(dllexport)
#       define __IREXTRAS_EXPIMP_TEMPLATE__
#   else
#       define __IREXTRAS_API__ __declspec(dllimport)
#       define __IREXTRAS_EXPIMP_TEMPLATE__ extern
#   endif
#else
#   define __IREXTRAS_API__
#   define __CALLCONV
#   define __IREXTRAS_EXPIMP_TEMPLATE__
#endif

#endif
