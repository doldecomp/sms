#ifndef _H_MACROS_
#define _H_MACROS_

#define ARRAY_COUNT(arr) (int)(sizeof(arr) / sizeof(arr[0]))
#define FLAG_ON(V, F)    (((V) & (F)) == 0)
#define FLAG_OFF(V, F)   (((V) & (F)) != 0)

#define ALIGN_PREV(u, align) (u & (~(align - 1)))
#define ALIGN_NEXT(u, align) ((u + (align - 1)) & (~(align - 1)))
#define IS_ALIGNED(X, N)     (((X) & ((N)-1)) == 0)
#define IS_NOT_ALIGNED(X, N) (((X) & ((N)-1)) != 0)

#define READU32_BE(ptr, offset)                                                \
	(((u32)ptr[offset] << 24) | ((u32)ptr[offset + 1] << 16)                   \
	 | ((u32)ptr[offset + 2] << 8) | (u32)ptr[offset + 3]);

#ifdef DEBUG
#define ASSERTLINE(line, cond)                                                 \
	((cond) || (OSPanic(__FILE__, line, "Failed assertion " #cond), 0))

#define ASSERTMSGLINE(line, cond, msg)                                         \
	((cond) || (OSPanic(__FILE__, line, msg), 0))

// This is dumb but we dont have a Metrowerks way to do variadic macros in the
// macro to make this done in a not scrubby way.
#define ASSERTMSG1LINE(line, cond, msg, arg1)                                  \
	((cond) || (OSPanic(__FILE__, line, msg, arg1), 0))

#define ASSERTMSG2LINE(line, cond, msg, arg1, arg2)                            \
	((cond) || (OSPanic(__FILE__, line, msg, arg1, arg2), 0))

#define ASSERTMSGLINEV(line, cond, ...)                                        \
	((cond) || (OSPanic(__FILE__, line, __VA_ARGS__), 0))

#else
#define ASSERTLINE(line, cond)                      (void)0
#define ASSERTMSGLINE(line, cond, msg)              (void)0
#define ASSERTMSG1LINE(line, cond, msg, arg1)       (void)0
#define ASSERTMSG2LINE(line, cond, msg, arg1, arg2) (void)0
#define ASSERTMSGLINEV(line, cond, ...)             (void)0
#endif

#define ASSERT(cond) ASSERTLINE(__LINE__, cond)

#endif // _H_MACROS_
