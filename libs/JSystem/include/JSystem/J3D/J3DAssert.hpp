#ifndef J3DASSERT_HPP
#define J3DASSERT_HPP

#include <JSystem/JUtility/JUTAssert.hpp>

#define J3D_ASSERT_NULLPTR(COND)                                               \
	JUT_ASSERT_MSG((COND) != 0, "Error : null pointer.")
#define J3D_ASSERT_RANGE(COND)                                                 \
	JUT_ASSERT_MSG((COND) != 0, "Error : range over.")
#define J3D_ASSERT_NONZEROARG(COND)                                            \
	JUT_ASSERT_MSG((COND) != 0, "Error : non-zero argument is specified 0.")
#define J3D_ASSERT_ALLOCMEM(COND)                                              \
	JUT_ASSERT_MSG((COND) != 0, "Error : allocate memory.")

#endif
