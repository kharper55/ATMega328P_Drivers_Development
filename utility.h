

#ifndef UTILITY_H_
#define UTILITY_H_

#include <util/delay.h>
#include <math.h>

// why the hell does this work?
#define round_(x) (x - (unsigned long)(x)>=0?(unsigned long)((x)+1):(unsigned long)((x)-1))

#endif //UTILITY_H_