#ifndef COMPILE_H
#define COMPILE_H

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

#define min(m,n) ((m) < (n) ? (m) : (n))
#define max(m,n) ((m) > (n) ? (m) : (n))


/*
 * GNU gcc built-in branch annotation
 */

#ifndef likely
# define likely(x)      __builtin_expect(!!(x), 1)
#endif

#ifndef unlikely
# define unlikely(x)    __builtin_expect(!!(x), 0)
#endif


#endif // COMPILE_H
