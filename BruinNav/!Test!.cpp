#include"provided.h"
#include<iostream>
using namespace std;


int main()
{
	/*MapLoader test;
	test.load("mapdata.txt");*/
	//cout << test.getNumSegments();
	//AttractionMapper am;
	//am.init(test); // let our object build its internal data structures
	//			 // by iterating thru all segments from the MapLoader object
	//			 GeoCoord fillMe;
	//string attraction = "CVS";
	//bool found = am.getGeoCoord(attraction, fillMe);
	//if (!found)
	//{
	//	cout << "No geolocation found for " << attraction << endl;
	//}
	//else
	//{
	//	cout << "The location of " << attraction << " is " <<
	//		fillMe.latitude << ", " << fillMe.longitude << endl;
	//}
	//
	//
	//SegmentMapper sm; 
	//sm.init(test);
	//GeoCoord lookMeUp("34.0510988", "-118.4600758");
	//std::vector<StreetSegment> vecOfAssociatedSegs(sm.getSegments(lookMeUp));
	//if (vecOfAssociatedSegs.empty()) 
	//{
	//	cout << "Error";
	//} 
	//else 
	//{
	//	cout << "Here are all the segments associated with your coordinate:" << endl;
	//	for (auto s : vecOfAssociatedSegs)
	//	{
	//		cout << "Segment’s street: " << s.streetName << endl; cout << "Segment’s start lat/long: " << s.segment.start.latitude << ", " << s.segment.start.longitude << endl;
	//		cout << "Segment’s end lat/long: " << s.segment.end.latitude << ", " << s.segment.end.longitude << endl;
	//		cout << "This segment has " << s.attractions.size() <<
	//			" attractions on it." << endl;
	//	}
	//}
	
	Navigator test1;
	test1.loadMapData("mapdata.txt");
	vector<NavSegment> directions;
	cerr << endl;
	cerr<<test1.navigate("1061 Broxton Avenue", "308 North Rodeo Drive", directions)<<endl;
	double length = 0;
	for (int n = 0; n < directions.size(); n++)
	{
		if (directions[n].m_command == 0)
		{
			cerr << directions[n].m_direction << endl
				<< directions[n].m_distance << endl
				<< directions[n].m_streetName << endl
				<< "start " << directions[n].m_geoSegment.start.latitudeText << " " << directions[n].m_geoSegment.start.longitudeText << endl
				<< "end " << directions[n].m_geoSegment.end.latitudeText << " " << directions[n].m_geoSegment.end.longitudeText << endl << endl;
			length += directions[n].m_distance;
		}
		else
		{
			cerr << "turn " << directions[n].m_direction << endl
				<< directions[n].m_streetName << endl<<endl;
		}

	}
	
	for (int n = 0; n < directions.size(); n++)
	{
		if (directions[n].m_command == 0)
		{
			cerr
				<< directions[n].m_geoSegment.start.latitudeText << "," << directions[n].m_geoSegment.start.longitudeText << endl
				<< directions[n].m_geoSegment.end.latitudeText << "," << directions[n].m_geoSegment.end.longitudeText << endl;
		}
	}
	cout << length;

}