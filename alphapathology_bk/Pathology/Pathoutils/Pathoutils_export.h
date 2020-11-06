
#ifndef PATHOUTILS_EXPORT_H
#define PATHOUTILS_EXPORT_H

#ifdef PATHOUTILS_STATIC_DEFINE
#  define PATHOUTILS_EXPORT
#  define PATHOUTILS_NO_EXPORT
#else
#  ifndef PATHOUTILS_EXPORT
#    ifdef Pathoutils_EXPORTS
        /* We are building this library */
#      define PATHOUTILS_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define PATHOUTILS_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef PATHOUTILS_NO_EXPORT
#    define PATHOUTILS_NO_EXPORT 
#  endif
#endif

#ifndef PATHOUTILS_DEPRECATED
#  define PATHOUTILS_DEPRECATED __declspec(deprecated)
#endif

#ifndef PATHOUTILS_DEPRECATED_EXPORT
#  define PATHOUTILS_DEPRECATED_EXPORT PATHOUTILS_EXPORT PATHOUTILS_DEPRECATED
#endif

#ifndef PATHOUTILS_DEPRECATED_NO_EXPORT
#  define PATHOUTILS_DEPRECATED_NO_EXPORT PATHOUTILS_NO_EXPORT PATHOUTILS_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef PATHOUTILS_NO_DEPRECATED
#    define PATHOUTILS_NO_DEPRECATED
#  endif
#endif

#endif /* PATHOUTILS_EXPORT_H */
