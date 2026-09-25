#ifndef JUT_DBG_HPP
#define JUT_DBG_HPP

// Get rid of this file?

#include <JSystem/JUtility/JUTAssert.hpp>
#include <JSystem/JUtility/JUTException.hpp>

// Panic functions
#define JUT_PANIC(MSG)   JUTException::panic(__FILE__, __LINE__, MSG);
#define JUT_PANIC_F(...) JUTException::panic_f(__FILE__, __LINE__, __VA_ARGS__);

#endif
