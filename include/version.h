#ifndef VERSION_H
#define VERSION_H

#ifdef VERSION_GMSJ01
#define GMSJ01(value) value
#else
#define GMSJ01(value)
#endif

#ifdef VERSION_GMSE01
#define GMSE01(value) value
#else
#define GMSE01(value)
#endif

#ifdef VERSION_GMSP01
#define GMSP01(value) value
#else
#define GMSP01(value)
#endif

#ifdef VERSION_GMSJ01_01
#define GMSJ01_01(value) value
#else
#define GMSJ01_01(value)
#endif

#ifdef VERSION_GMSP01_NX
#define GMSP01_NX(value) value
#else
#define GMSP01_NX(value)
#endif

#define VERSION_SELECT_JOIN(a, b, c, d, e, f, g, h, ...) a b c d e f g h

#define VERSION_SELECT(...) (VERSION_SELECT_JOIN(__VA_ARGS__, , , , , , , ))

#define VSD_STR(x) #x
#define VSD_XSTR(x) VSD_STR(x)
#define VERSION_SELECT_DEFAULT(defualt_value, ...)                              \
    (sizeof(VSD_XSTR(VERSION_SELECT_JOIN(__VA_ARGS__, , , , , , , , ))) > 1     \
         ? (VERSION_SELECT_JOIN(__VA_ARGS__, , , , , , , , ))                   \
         : (defualt_value))

#endif
