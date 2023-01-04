#ifndef KMP_H 
#define KMP_H

#include "../common.h"

vec<int> const& next_naive(std::string const& pattern);
vec<int> const& next_opt(std::string const& pattern);

void test_next_naive();
void test_next_opt();


#endif
