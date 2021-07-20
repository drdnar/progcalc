#ifndef IGNORE_WARNING_H
#define IGNORE_WARNING_H

#ifdef __clang__

#ifndef STR
#define STR(X) #X
#endif /* STR */

#define IGNORE_WARNING(X) _Pragma("clang diagnostic push") _Pragma(STR(clang diagnostic ignored X))
#define END_IGNORE_WARNING _Pragma("clang diagnostic pop")

#else /* __clang__ */

#define IGNORE_WARNING(X)
#define END_IGNORE_WARNING

#define IGNORE_WARNING_UNUSED_PARAMETER

#endif /* __clang__ */

#endif /* IGNORE_WARNING_H */
