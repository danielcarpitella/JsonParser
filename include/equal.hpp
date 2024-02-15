#ifndef EQUAL_H
#define EQUAL_H

#include "../src/json.cpp"
#include <algorithm>


bool operator==(json const& x, json const& y);

bool operator!=(json const& x, json const& y);

#endif