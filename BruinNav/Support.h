#ifndef Operators_use_
#define Operators_use_
#include"provided.h"

bool operator==(const GeoCoord& lhs, const GeoCoord& rhs);

bool operator>(const GeoCoord& lhs, const GeoCoord& rhs);

bool operator<(const GeoCoord& lhs, const GeoCoord& rhs);

bool operator==(const GeoSegment& lhs, const GeoSegment& rhs);
#endif
