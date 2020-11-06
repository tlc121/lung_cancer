
#ifndef COMMONUTILS_EXPORT_H
#define COMMONUTILS_EXPORT_H

#ifdef COMMONUTILS_STATIC_DEFINE
#  define COMMONUTILS_EXPORT
#  define COMMONUTILS_NO_EXPORT
#else
#  ifndef COMMONUTILS_EXPORT
#    ifdef CommonUtils_EXPORTS
        /* We are building this library */
#      define COMMONUTILS_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define COMMONUTILS_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef COMMONUTILS_NO_EXPORT
#    define COMMONUTILS_NO_EXPORT 
#  endif
#endif

#ifndef COMMONUTILS_DEPRECATED
#  define COMMONUTILS_DEPRECATED __declspec(deprecated)
#endif

#ifndef COMMONUTILS_DEPRECATED_EXPORT
#  define COMMONUTILS_DEPRECATED_EXPORT COMMONUTILS_EXPORT COMMONUTILS_DEPRECATED
#endif

#ifndef COMMONUTILS_DEPRECATED_NO_EXPORT
#  define COMMONUTILS_DEPRECATED_NO_EXPORT COMMONUTILS_NO_EXPORT COMMONUTILS_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef COMMONUTILS_NO_DEPRECATED
#    define COMMONUTILS_NO_DEPRECATED
#  endif
#endif

#endif /* COMMONUTILS_EXPORT_H */
