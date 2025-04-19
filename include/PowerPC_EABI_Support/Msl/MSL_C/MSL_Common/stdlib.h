#ifndef MSL_STDLIB_H_
#define MSL_STDLIB_H_

#include "abort_exit.h"
#include "arith.h"
#include "mbstring.h"
#include "strtoul.h"

#define RAND_MAX 32767

#ifdef __cplusplus
extern "C" {
#endif

int rand(void);
void srand(size_t seed);

#ifdef __cplusplus
};
#endif

#endif
