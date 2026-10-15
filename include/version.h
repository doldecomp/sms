#ifndef VERSION_H
#define VERSION_H

#ifdef VERSION_GMSJ01
#define GMSJ01(value) (value)
#else
#define GMSJ01(value)
#endif

#ifdef VERSION_GMSE01
#define GMSE01(value) (value)
#else
#define GMSE01(value)
#endif

#ifdef VERSION_GMSP01
#define GMSP01(value) (value)
#else
#define GMSP01(value)
#endif

#ifdef VERSION_GMSJ01_01
#define GMSJ01_01(value) (value)
#else
#define GMSJ01_01(value)
#endif

#ifdef VERSION_GMSP01_NX
#define GMSP01_NX(value) (value)
#else
#define GMSP01_NX(value)
#endif

// VERSION_SELECT(default_value, GMSJ01(value), ...) expands to just the
// selected expression, or the default when no listed version is enabled.
// Enabled version wrappers form a parenthesized value; disabled ones are empty.
// The probe recognizes those parentheses without generating a C++ condition.
#define VERSION_SELECT_PROBE(...)        ~, 1
#define VERSION_SELECT_SECOND(a, b, ...) b
#define VERSION_SELECT_CHECK(...)        VERSION_SELECT_SECOND(__VA_ARGS__, 0, 0)
#define VERSION_SELECT_IS_VALUE(value)   VERSION_SELECT_IS_VALUE_I((value))
// MWCC needs token pasting with the argument tuple to rescan expanded wrappers.
#ifdef __MWERKS__
#define VERSION_SELECT_IS_VALUE_I(args) VERSION_SELECT_IS_VALUE_II##args
#else
#define VERSION_SELECT_IS_VALUE_I(args) VERSION_SELECT_IS_VALUE_II args
#endif
#define VERSION_SELECT_IS_VALUE_II(value)                                      \
	VERSION_SELECT_CHECK(VERSION_SELECT_PROBE value)
#define VERSION_SELECT_CHOOSE_0(default_value, value) (default_value)
#define VERSION_SELECT_CHOOSE_1(default_value, value) value
#define VERSION_SELECT_CHOOSE_I(has_value, default_value, value)               \
	VERSION_SELECT_CHOOSE_##has_value(default_value, value)
#define VERSION_SELECT_CHOOSE(has_value, default_value, value)                 \
	VERSION_SELECT_CHOOSE_I(has_value, default_value, value)
#define VERSION_SELECT_VALUE(default_value, value)                             \
	VERSION_SELECT_CHOOSE(VERSION_SELECT_IS_VALUE(value), default_value, value)
#define VERSION_SELECT_IMPL(default_value, a, b, c, d, e, f, g, h, ...)        \
	VERSION_SELECT_VALUE(default_value, a b c d e f g h)
#define VERSION_SELECT(...) VERSION_SELECT_IMPL(__VA_ARGS__, , , , , , , , , )
#endif
