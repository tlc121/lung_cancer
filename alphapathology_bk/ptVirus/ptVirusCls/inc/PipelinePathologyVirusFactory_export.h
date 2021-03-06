
#ifndef PIPELINEPATHOLOGYVIRUSFACTORY_EXPORT_H
#define PIPELINEPATHOLOGYVIRUSFACTORY_EXPORT_H

#ifdef PIPELINEPATHOLOGYVIRUSFACTORY_STATIC_DEFINE
#  define PIPELINEPATHOLOGYVIRUSFACTORY_EXPORT
#  define PIPELINEPATHOLOGYVIRUSFACTORY_NO_EXPORT
#else
#  ifndef PIPELINEPATHOLOGYVIRUSFACTORY_EXPORT
#    ifdef PipelinePathologyVirusFactory_EXPORTS
        /* We are building this library */
#      define PIPELINEPATHOLOGYVIRUSFACTORY_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define PIPELINEPATHOLOGYVIRUSFACTORY_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef PIPELINEPATHOLOGYVIRUSFACTORY_NO_EXPORT
#    define PIPELINEPATHOLOGYVIRUSFACTORY_NO_EXPORT 
#  endif
#endif

#ifndef PIPELINEPATHOLOGYVIRUSFACTORY_DEPRECATED
#  define PIPELINEPATHOLOGYVIRUSFACTORY_DEPRECATED __declspec(deprecated)
#endif

#ifndef PIPELINEPATHOLOGYVIRUSFACTORY_DEPRECATED_EXPORT
#  define PIPELINEPATHOLOGYVIRUSFACTORY_DEPRECATED_EXPORT PIPELINEPATHOLOGYVIRUSFACTORY_EXPORT PIPELINEPATHOLOGYVIRUSFACTORY_DEPRECATED
#endif

#ifndef PIPELINEPATHOLOGYVIRUSFACTORY_DEPRECATED_NO_EXPORT
#  define PIPELINEPATHOLOGYVIRUSFACTORY_DEPRECATED_NO_EXPORT PIPELINEPATHOLOGYVIRUSFACTORY_NO_EXPORT PIPELINEPATHOLOGYVIRUSFACTORY_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef PIPELINEPATHOLOGYVIRUSFACTORY_NO_DEPRECATED
#    define PIPELINEPATHOLOGYVIRUSFACTORY_NO_DEPRECATED
#  endif
#endif

#endif /* PIPELINEPATHOLOGYVIRUSFACTORY_EXPORT_H */
