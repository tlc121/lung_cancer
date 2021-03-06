
#ifndef PIPELINEPATHOLOGYFEIAI_EXPORT_H
#define PIPELINEPATHOLOGYFEIAI_EXPORT_H

#ifdef PIPELINEPATHOLOGYFEIAI_STATIC_DEFINE
#  define PIPELINEPATHOLOGYFEIAI_EXPORT
#  define PIPELINEPATHOLOGYFEIAI_NO_EXPORT
#else
#  ifndef PIPELINEPATHOLOGYFEIAI_EXPORT
#    ifdef PipelinePathologyFEIAI_EXPORTS
        /* We are building this library */
#      define PIPELINEPATHOLOGYFEIAI_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define PIPELINEPATHOLOGYFEIAI_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef PIPELINEPATHOLOGYFEIAI_NO_EXPORT
#    define PIPELINEPATHOLOGYFEIAI_NO_EXPORT 
#  endif
#endif

#ifndef PIPELINEPATHOLOGYFEIAI_DEPRECATED
#  define PIPELINEPATHOLOGYFEIAI_DEPRECATED __declspec(deprecated)
#endif

#ifndef PIPELINEPATHOLOGYFEIAI_DEPRECATED_EXPORT
#  define PIPELINEPATHOLOGYFEIAI_DEPRECATED_EXPORT PIPELINEPATHOLOGYFEIAI_EXPORT PIPELINEPATHOLOGYFEIAI_DEPRECATED
#endif

#ifndef PIPELINEPATHOLOGYFEIAI_DEPRECATED_NO_EXPORT
#  define PIPELINEPATHOLOGYFEIAI_DEPRECATED_NO_EXPORT PIPELINEPATHOLOGYFEIAI_NO_EXPORT PIPELINEPATHOLOGYFEIAI_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef PIPELINEPATHOLOGYFEIAI_NO_DEPRECATED
#    define PIPELINEPATHOLOGYFEIAI_NO_DEPRECATED
#  endif
#endif

#endif /* PIPELINEPATHOLOGYFEIAI_EXPORT_H */
