
#ifndef EVALUATIONALG_EXPORT_H
#define EVALUATIONALG_EXPORT_H

#ifdef EVALUATIONALG_STATIC_DEFINE
#  define EVALUATIONALG_EXPORT
#  define EVALUATIONALG_NO_EXPORT
#else
#  ifndef EVALUATIONALG_EXPORT
#    ifdef EvaluationAlg_EXPORTS
        /* We are building this library */
#      define EVALUATIONALG_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define EVALUATIONALG_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef EVALUATIONALG_NO_EXPORT
#    define EVALUATIONALG_NO_EXPORT 
#  endif
#endif

#ifndef EVALUATIONALG_DEPRECATED
#  define EVALUATIONALG_DEPRECATED __declspec(deprecated)
#endif

#ifndef EVALUATIONALG_DEPRECATED_EXPORT
#  define EVALUATIONALG_DEPRECATED_EXPORT EVALUATIONALG_EXPORT EVALUATIONALG_DEPRECATED
#endif

#ifndef EVALUATIONALG_DEPRECATED_NO_EXPORT
#  define EVALUATIONALG_DEPRECATED_NO_EXPORT EVALUATIONALG_NO_EXPORT EVALUATIONALG_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef EVALUATIONALG_NO_DEPRECATED
#    define EVALUATIONALG_NO_DEPRECATED
#  endif
#endif

#endif /* EVALUATIONALG_EXPORT_H */
