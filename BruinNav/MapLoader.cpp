#include "provided.h"
#include <string>
#include<fstream>
#include<vector>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
	vector<StreetSegment*> m_streets;
};

MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
	vector<StreetSegment*>::iterator p = m_streets.begin();		//clean up
	while (p != m_streets.end())
	{
		delete (*p);
		p++;
	}
}

bool MapLoaderImpl::load(string mapFile)
{
	int num_of_attr = 0;
	int line_is = 0;
	string line;
	ifstream m_file(mapFile);
	if (m_file)
	{
		while (getline(m_file, line))		//go over each line of file
		{
			if (num_of_attr != 0)	//if there is attrctions needed to be processed
			{
				string name, lo, holder;
				for (int n = 0; n < line.length(); n++)		//go over the characters in the line
				{
					if (line[n] == '|')	//if meet '|' , we have the name of attraction
					{
						name = holder;
						holder = "";
					}
					else if (line[n] == ' '&& holder == "")		//if meet ' ' (space) and holder is empty, the space must be the space behind the "," which should be ignored
					{
					}
					else if (line[n] == ',')		//if meet ',' we find latitude
					{
						lo = holder;
						holder = "";
					}
					else	//else get the character to holder
					{
						holder += line[n];
					}
				}
				Attraction my;	//initialize attraction
				my.geocoordinates = GeoCoord(lo, holder);
				my.name = name;
				m_streets.back()->attractions.push_back(my);
				num_of_attr--;	//decrease the number of attractions need to be processed
				line_is = 0;	//indicate that this line is attractions
			}
			else if (line_is == 0)		//if last line is attraction and we processed all of them, this line must be street name
			{
				m_streets.push_back(new StreetSegment);
				m_streets.back()->streetName = line;
				line_is = 1;
			}
			else if (line_is == 1)	//if last line is street name, this must be the start and end coordinate
			{
				GeoCoord* s = nullptr;
				string lo, holder;
				for (int n = 0; n < line.length(); n++)
				{
					if (line[n] == ',' && holder != "")		//if meet ',' we find latitude
					{
						lo = holder;
						holder = "";
					}
					else if (line[n] == ' ' && holder == "")		//if meet ' ' (space) and holder is empty, the space must be the space behind the "," which should be ignored
					{

					}
					else if (line[n] == ' ' && holder != "")		//if meet ' ' (space) and holder is not empty, we reach the end
					{
						s = new GeoCoord(lo, holder);
						holder = "";
						lo = "";
					}
					else	//else get the character to holder
					{
						holder += line[n];
					}
				}
				m_streets.back()->segment = GeoSegment(*s, GeoCoord(lo,holder));
				delete s;
				line_is = 2;
			}
			else if (line_is == 2)	//if last line is the street's coordinate, this must be the line indicate the number of attractions
			{
				num_of_attr = stoi(line);
				line_is = 0;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

size_t MapLoaderImpl::getNumSegments() const
{
	return m_streets.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if (segNum < m_streets.size())
	{
		seg = *m_streets[segNum];
		return true;
	}
	else
		return false;
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
