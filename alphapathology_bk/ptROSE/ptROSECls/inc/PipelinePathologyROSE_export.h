
#ifndef PIPELINEPATHOLOGYROSE_EXPORT_H
#define PIPELINEPATHOLOGYROSE_EXPORT_H

#ifdef PIPELINEPATHOLOGYROSE_STATIC_DEFINE
#  define PIPELINEPATHOLOGYROSE_EXPORT
#  define PIPELINEPATHOLOGYROSE_NO_EXPORT
#else
#  ifndef PIPELINEPATHOLOGYROSE_EXPORT
#    ifdef PipelinePathologyROSE_EXPORTS
        /* We are building this library */
#      define PIPELINEPATHOLOGYROSE_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define PIPELINEPATHOLOGYROSE_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef PIPELINEPATHOLOGYROSE_NO_EXPORT
#    define PIPELINEPATHOLOGYROSE_NO_EXPORT 
#  endif
#endif

#ifndef PIPELINEPATHOLOGYROSE_DEPRECATED
#  define PIPELINEPATHOLOGYROSE_DEPRECATED __declspec(deprecated)
#endif

#ifndef PIPELINEPATHOLOGYROSE_DEPRECATED_EXPORT
#  define PIPELINEPATHOLOGYROSE_DEPRECATED_EXPORT PIPELINEPATHOLOGYROSE_EXPORT PIPELINEPATHOLOGYROSE_DEPRECATED
#endif

#ifndef PIPELINEPATHOLOGYROSE_DEPRECATED_NO_EXPORT
#  define PIPELINEPATHOLOGYROSE_DEPRECATED_NO_EXPORT PIPELINEPATHOLOGYROSE_NO_EXPORT PIPELINEPATHOLOGYROSE_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef PIPELINEPATHOLOGYROSE_NO_DEPRECATED
#    define PIPELINEPATHOLOGYROSE_NO_DEPRECATED
#  endif
#endif

#endif /* PIPELINEPATHOLOGYROSE_EXPORT_H */
