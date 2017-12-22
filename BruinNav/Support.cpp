#include "Support.h"
#include "provided.h"
#include<string>
using namespace std;

bool operator==(const GeoCoord& lhs, const GeoCoord& rhs)	//overload == operator for GeoCoord by compare the latitude and longitude
{
	return lhs.latitude == rhs.latitude && lhs.longitude == rhs.longitude;
}


bool operator>(const GeoCoord& lhs, const GeoCoord& rhs)	//overload > operator for GeoCoord by compare the latitude and longitude text
{
	string adv = lhs.latitudeText + lhs.longitudeText;
	string oadv = rhs.latitudeText + rhs.longitudeText;
	if (adv > oadv)
		return true;
	else
		return false;
}

bool operator<(const GeoCoord& lhs, const GeoCoord& rhs)	//overload < operator for GeoCoord by compare the latitude and longitude text
{
	string adv = lhs.latitudeText + lhs.longitudeText;
	string oadv = rhs.latitudeText + rhs.longitudeText;
	if (adv < oadv)
		return true;
	else
		return false;
}

bool operator==(const GeoSegment& lhs, const GeoSegment& rhs)	//overload == operator for GeoSegment by compare the latitude and longitude of the start and end
{
	if (lhs.end == rhs.end && lhs.start == rhs.start)
		return true;
	else if (lhs.start == rhs.end && lhs.end == rhs.start)
		return true;
	else
		return false;
}