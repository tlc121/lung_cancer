
#ifndef PIPELINEPATHOLOGYTCTFACTORY_EXPORT_H
#define PIPELINEPATHOLOGYTCTFACTORY_EXPORT_H

#ifdef PIPELINEPATHOLOGYTCTFACTORY_STATIC_DEFINE
#  define PIPELINEPATHOLOGYTCTFACTORY_EXPORT
#  define PIPELINEPATHOLOGYTCTFACTORY_NO_EXPORT
#else
#  ifndef PIPELINEPATHOLOGYTCTFACTORY_EXPORT
#    ifdef PipelinePathologyTCTFactory_EXPORTS
        /* We are building this library */
#      define PIPELINEPATHOLOGYTCTFACTORY_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define PIPELINEPATHOLOGYTCTFACTORY_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef PIPELINEPATHOLOGYTCTFACTORY_NO_EXPORT
#    define PIPELINEPATHOLOGYTCTFACTORY_NO_EXPORT 
#  endif
#endif

#ifndef PIPELINEPATHOLOGYTCTFACTORY_DEPRECATED
#  define PIPELINEPATHOLOGYTCTFACTORY_DEPRECATED __declspec(deprecated)
#endif

#ifndef PIPELINEPATHOLOGYTCTFACTORY_DEPRECATED_EXPORT
#  define PIPELINEPATHOLOGYTCTFACTORY_DEPRECATED_EXPORT PIPELINEPATHOLOGYTCTFACTORY_EXPORT PIPELINEPATHOLOGYTCTFACTORY_DEPRECATED
#endif

#ifndef PIPELINEPATHOLOGYTCTFACTORY_DEPRECATED_NO_EXPORT
#  define PIPELINEPATHOLOGYTCTFACTORY_DEPRECATED_NO_EXPORT PIPELINEPATHOLOGYTCTFACTORY_NO_EXPORT PIPELINEPATHOLOGYTCTFACTORY_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef PIPELINEPATHOLOGYTCTFACTORY_NO_DEPRECATED
#    define PIPELINEPATHOLOGYTCTFACTORY_NO_DEPRECATED
#  endif
#endif

#endif /* PIPELINEPATHOLOGYTCTFACTORY_EXPORT_H */