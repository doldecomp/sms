#ifndef VERSION_H
#define VERSION_H

#ifdef VERSION_GMSJ01
#define GMSJ01(value) value
#else
#define GMSJ01(value)
#endif

#ifdef VERSION_GMSP01
#define GMSP01(value) value
#else
#define GMSP01(value)
#endif

#define VERSION_SELECT_JOIN(a, b, c, d, e, f, g, h, ...) a b c d e f g h

#define VERSION_SELECT(...) (VERSION_SELECT_JOIN(__VA_ARGS__, , , , , , , ))

#endif
