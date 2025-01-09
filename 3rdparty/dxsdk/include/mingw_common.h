#ifndef MINGW_COMMON_H
#define MINGW_COMMON_H




#if defined(_MSC_VER) && !defined(_MSC_EXTENSIONS)
#define NONAMELESSUNION		1
#endif
#if defined(NONAMELESSSTRUCT) && \
   !defined(NONAMELESSUNION)
#define NONAMELESSUNION		1
#endif
#if defined(NONAMELESSUNION)  && \
   !defined(NONAMELESSSTRUCT)
#define NONAMELESSSTRUCT	1
#endif

#ifndef __ANONYMOUS_DEFINED
#define __ANONYMOUS_DEFINED
#define _ANONYMOUS_UNION  __MINGW_EXTENSION
#define _ANONYMOUS_STRUCT __MINGW_EXTENSION
#ifndef NONAMELESSUNION
#define _UNION_NAME(x)
#define _STRUCT_NAME(x)
#else /* NONAMELESSUNION */
#define _UNION_NAME(x)  x
#define _STRUCT_NAME(x) x
#endif
#endif	/* __ANONYMOUS_DEFINED */

#ifndef DUMMYUNIONNAME
# ifdef NONAMELESSUNION
#  define DUMMYUNIONNAME  u
#  define DUMMYUNIONNAME1 u1	/* Wine uses this variant */
#  define DUMMYUNIONNAME2 u2
#  define DUMMYUNIONNAME3 u3
#  define DUMMYUNIONNAME4 u4
#  define DUMMYUNIONNAME5 u5
#  define DUMMYUNIONNAME6 u6
#  define DUMMYUNIONNAME7 u7
#  define DUMMYUNIONNAME8 u8
#  define DUMMYUNIONNAME9 u9
# else /* NONAMELESSUNION */
#  define DUMMYUNIONNAME
#  define DUMMYUNIONNAME1	/* Wine uses this variant */
#  define DUMMYUNIONNAME2
#  define DUMMYUNIONNAME3
#  define DUMMYUNIONNAME4
#  define DUMMYUNIONNAME5
#  define DUMMYUNIONNAME6
#  define DUMMYUNIONNAME7
#  define DUMMYUNIONNAME8
#  define DUMMYUNIONNAME9
# endif
#endif	/* DUMMYUNIONNAME */

#ifndef DUMMYSTRUCTNAME
# ifdef NONAMELESSUNION
#  define DUMMYSTRUCTNAME  s
#  define DUMMYSTRUCTNAME1 s1	/* Wine uses this variant */
#  define DUMMYSTRUCTNAME2 s2
#  define DUMMYSTRUCTNAME3 s3
#  define DUMMYSTRUCTNAME4 s4
#  define DUMMYSTRUCTNAME5 s5
# else
#  define DUMMYSTRUCTNAME
#  define DUMMYSTRUCTNAME1	/* Wine uses this variant */
#  define DUMMYSTRUCTNAME2
#  define DUMMYSTRUCTNAME3
#  define DUMMYSTRUCTNAME4
#  define DUMMYSTRUCTNAME5
# endif
#endif /* DUMMYSTRUCTNAME */

#endif  // MINGW_COMMON_H
