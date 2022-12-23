#ifndef _UTILS_H_
#define _UTILS_H_
#include "defines.h"

/**
 * try and search target file in parent directories with max depth limit
 */
std::string getPath(const std::string &target, int depth = 5);

#endif