
#ifndef PIPELINEPATHOLOGYVIRUS_EXPORT_H
#define PIPELINEPATHOLOGYVIRUS_EXPORT_H

#ifdef PIPELINEPATHOLOGYVIRUS_STATIC_DEFINE
#  define PIPELINEPATHOLOGYVIRUS_EXPORT
#  define PIPELINEPATHOLOGYVIRUS_NO_EXPORT
#else
#  ifndef PIPELINEPATHOLOGYVIRUS_EXPORT
#    ifdef PipelinePathologyVirus_EXPORTS
        /* We are building this library */
#      define PIPELINEPATHOLOGYVIRUS_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define PIPELINEPATHOLOGYVIRUS_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef PIPELINEPATHOLOGYVIRUS_NO_EXPORT
#    define PIPELINEPATHOLOGYVIRUS_NO_EXPORT 
#  endif
#endif

#ifndef PIPELINEPATHOLOGYVIRUS_DEPRECATED
#  define PIPELINEPATHOLOGYVIRUS_DEPRECATED __declspec(deprecated)
#endif

#ifndef PIPELINEPATHOLOGYVIRUS_DEPRECATED_EXPORT
#  define PIPELINEPATHOLOGYVIRUS_DEPRECATED_EXPORT PIPELINEPATHOLOGYVIRUS_EXPORT PIPELINEPATHOLOGYVIRUS_DEPRECATED
#endif

#ifndef PIPELINEPATHOLOGYVIRUS_DEPRECATED_NO_EXPORT
#  define PIPELINEPATHOLOGYVIRUS_DEPRECATED_NO_EXPORT PIPELINEPATHOLOGYVIRUS_NO_EXPORT PIPELINEPATHOLOGYVIRUS_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef PIPELINEPATHOLOGYVIRUS_NO_DEPRECATED
#    define PIPELINEPATHOLOGYVIRUS_NO_DEPRECATED
#  endif
#endif

#endif /* PIPELINEPATHOLOGYVIRUS_EXPORT_H */