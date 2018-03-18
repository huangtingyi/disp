#pragma once
#include <cstdint>

inline int64_t yuan2percentFen(const double yuan) {
	return int64_t((yuan + 0.00005) * 10000);
}