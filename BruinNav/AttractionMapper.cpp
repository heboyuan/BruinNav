#include "provided.h"
#include "MyMap.h"
#include <string>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
	MyMap<string, GeoCoord> m_attraction;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
	StreetSegment holder;
	for (int n = 0; n < ml.getNumSegments(); n++)	//go over all the street segments
	{
		ml.getSegment(n, holder);
		if (!holder.attractions.empty())	//if there is attraction
		{
			string m_name;
			for (vector<Attraction>::iterator p = holder.attractions.begin(); p != holder.attractions.end(); p++)	//go over the attractions
			{
				for (int n = 0; n < p->name.length(); n++)		//make sure they are all lowercase
					m_name += tolower(p->name[n]);
				m_attraction.associate(m_name, p->geocoordinates);	//associate the attraction with coordinate
				m_name = "";
			}
		}
	}
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	for (int n = 0; n < attraction.length(); n++)	//change to lowercase
		attraction[n] = tolower(attraction[n]);
	const GeoCoord* Holder;
	Holder = m_attraction.find(attraction);	//if the coordinate associated with the attraction
	if (Holder == nullptr)
		return false;
	else
	{
		gc = *Holder;
		return true;
	}
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
