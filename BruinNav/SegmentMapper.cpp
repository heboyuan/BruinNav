#include "provided.h"
#include "MyMap.h"
#include <vector>
#include "Support.h"
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:

	struct StreetVector	//a struct to make it possible to associate more than one vectors with a coordinate
	{
		StreetVector& operator=(const StreetVector& rhs)		//overload the assignment operator so if assign, we concatenate the vector
		{
			if (&rhs != this)
			{
					m_street.insert(m_street.end(),rhs.m_street.begin(),rhs.m_street.end());
			}
			return *this;
		}
		vector<StreetSegment> m_street;
	};

	MyMap<string, StreetVector> m_seg;
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
	StreetSegment holder;
	for (int n = 0; n < ml.getNumSegments(); n++)	//go over the segments
	{
		ml.getSegment(n, holder);
		StreetVector holder1;
		holder1.m_street.push_back(holder);	//initialize the StreetVector which are a list of StreetSegments
		
		m_seg.associate(holder.segment.start.latitudeText + holder.segment.start.longitudeText, holder1);	//associate the street with the start point
		m_seg.associate(holder.segment.end.latitudeText + holder.segment.end.longitudeText, holder1);	//associate the street with the end point

		if (!holder.attractions.empty())		//associate the street with the attractions
		{
			for (vector<Attraction>::iterator p = holder.attractions.begin(); p != holder.attractions.end(); p++)
			{
				m_seg.associate(p->geocoordinates.latitudeText+ p->geocoordinates.longitudeText, holder1);
			}
		}
	}

}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	vector<StreetSegment> segments;
	const StreetVector* holder;
	holder = m_seg.find(gc.latitudeText+gc.longitudeText);	//find the street related to the point
	if (holder == nullptr)
		return segments;
	else
	{
		segments = holder->m_street;	//return the list of streets
		return segments;
	}
}


//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
