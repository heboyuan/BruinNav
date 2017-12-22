#include"provided.h"
#include<iostream>
using namespace std;


int main()
{
	cout << "Loading the map..." << endl <<endl;

	Navigator Nav;
	Nav.loadMapData("mapdata.txt");
	vector<NavSegment> directions;
	cout << "instructions===================================================" << endl
		<< "Map data is in mapdata.txt" << endl
		<< "Valid location name is in validlocs.txt" << endl <<endl
		<< "Valid Location Example"<<endl
		<<"UCLA Blood & Platelet Center"<<endl
		<< "10925 Kinross Avenue" << endl
		<< "Ralph's" << endl
		<< "UCLA School of Dentistry" << endl
		<< "==============================================================" << endl <<endl;
	string start, end;
	cout << "Where do you want to start (please enter valid location in validlocs.txt)"<<endl<<"Start Location: ";
	getline(cin, start);
	cout << "Where do you want to go (please enter valid location in validlocs.txt)" << endl << "Destination Location: ";
	getline(cin, end);
	Nav.navigate(start, end, directions);

	double length = 0;

	cout << endl << "Finding your rouute..." << endl;
	
	cout <<endl<< "Please Follow the navigation instrucntion below" << endl
		<< "===================================================" << endl;
	string temp = "";
	for (int n = 0; n < directions.size(); n++)
	{
		
		if (directions[n].m_command == 0)
		{
			if (temp != directions[n].m_streetName)
			{
				temp = directions[n].m_streetName;
				cout << "Please keep "<< directions[n].m_direction << " on "<< directions[n].m_streetName << endl << endl;
			}
			length += directions[n].m_distance;
		}
		else
		{
			cout << "Please turn " << directions[n].m_direction << " onto "
				<< directions[n].m_streetName << endl << endl;
		}

	}
	cout << "===================================================" << endl<<endl;

	cout << "Your Route is" << endl;
	cout << "Route Coordinate====================================" << endl;
	for (int n = 0; n < directions.size(); n++)
	{
		if (directions[n].m_command == 0)
		{
			cout
				<< directions[n].m_geoSegment.start.latitudeText << "," << directions[n].m_geoSegment.start.longitudeText << endl
				<< directions[n].m_geoSegment.end.latitudeText << "," << directions[n].m_geoSegment.end.longitudeText << endl;
		}
	}
	cout << "===================================================" << endl;
	cout << endl;
	cout << "the total distance is "<< length <<" miles" << endl << endl;
	
	cout << "Enter anything to exist" << endl;
	string finish;
	cin >> finish;
}