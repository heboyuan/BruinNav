#include "provided.h"
#include"MyMap.h"
#include "Support.h"
#include <string>
#include <vector>
using namespace std;

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
	struct Node			//the node used by A* algorithm
	{
		Node()
		{}

		Node(GeoCoord m_co, double f, double g, double h, string n)
			:m_co(m_co), f(f), g(g), h(h), sname(n)
		{}

		bool operator==(const Node& rhs)	//overload the == operator so it's easier to find compare
		{
			if (m_co == rhs.m_co)
				return true;
			else
				return false;
		}

		Node* parent;		//pointer to parent node
		GeoCoord m_co;
		double f, g, h;		//the f,g,h value used by A* algorithm
		string sname;		//the street name used when reconstruct the road
	};

	void reconstruction(Node* bui, vector<NavSegment> &directions, GeoCoord &start) const	//function used to reconstuct the road from end to start
	{

		if (bui->parent->m_co == start)		//if the parent node is start
		{
			GeoSegment linex(bui->parent->m_co, bui->m_co);		//create a segment according to the coordinate
			double anglex = angleOfLine(linex);					//calculate the angle
			string directionx;
			if (anglex >= 0 && anglex <= 22.5)					//find the direction
				directionx = "east";
			else if (anglex > 22.5 && anglex <= 67.5)
				directionx = "northeast";
			else if (anglex > 67.5 && anglex <= 112.5)
				directionx = "north";
			else if (anglex > 112.5 && anglex <= 157.5)
				directionx = "northwest";
			else if (anglex > 157.5 && anglex <= 202.5)
				directionx = "west";
			else if (anglex > 202.5 && anglex <= 247.5)
				directionx = "southwest";
			else if (anglex > 247.5 && anglex <= 292.5)
				directionx = "south";
			else if (anglex > 292.5 && anglex <= 337.5)
				directionx = "southeast";
			else
				directionx = "east";
			double distancex = distanceEarthMiles(bui->parent->m_co, bui->m_co);	//find the distance
			directions.push_back(NavSegment(directionx, bui->sname, distancex, linex));		//push it onto the vector
			return;
		}
		else		//if parent node is not start
		{
			reconstruction(bui->parent, directions, start);		//recursively reconstruct other
			GeoSegment linex(bui->parent->m_co, bui->m_co);		//create a segment according to the coordinate
			double anglex = angleOfLine(linex);					//calculate the angle
			string directionx;
			if (anglex >= 0 && anglex <= 22.5)					//find the direction
				directionx = "east";
			else if (anglex > 22.5 && anglex <= 67.5)
				directionx = "northeast";
			else if (anglex > 67.5 && anglex <= 112.5)
				directionx = "north";
			else if (anglex > 112.5 && anglex <= 157.5)
				directionx = "northwest";
			else if (anglex > 157.5 && anglex <= 202.5)
				directionx = "west";
			else if (anglex > 202.5 && anglex <= 247.5)
				directionx = "southwest";
			else if (anglex > 247.5 && anglex <= 292.5)
				directionx = "south";
			else if (anglex > 292.5 && anglex <= 337.5)
				directionx = "southeast";
			else
				directionx = "east";
			double distancex = distanceEarthMiles(bui->parent->m_co, bui->m_co);	//find the distance
			if (bui->sname != bui->parent->sname)		//if we take a turn to a new road
			{
				GeoSegment linez(bui->parent->parent->m_co, bui->parent->m_co);
				double anglez = angleBetween2Lines(linez, linex);	//calculate the angle
				if (anglez < 180)	//push NavSegment accordingly
					directions.push_back(NavSegment("left", bui->sname));
				else
					directions.push_back(NavSegment("right", bui->sname));
			}
			directions.push_back(NavSegment(directionx, bui->sname, distancex, linex));

		}
	}

	MapLoader m_map;
	SegmentMapper m_seg;
	AttractionMapper m_att;	
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)		//use other class to load data
{
	if (m_map.load(mapFile))
	{
		m_seg.init(m_map);
		m_att.init(m_map);
		return true;
	}
	else
		return false;
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
	

	vector<Node*> open;
	vector<Node*> close;

	GeoCoord holder_start, holder_end;
	if (m_att.getGeoCoord(start, holder_start) == false)	//if can't find the start point
		return NAV_BAD_SOURCE;
	if (m_att.getGeoCoord(end, holder_end) == false)	//if can't find the end point
		return NAV_BAD_DESTINATION;


	Node startp(holder_start, distanceEarthMiles(holder_start, holder_end),0, distanceEarthMiles(holder_start, holder_end),"");	//create a node according to start point
	
	vector<StreetSegment> holders = m_seg.getSegments(holder_start);	//find the street that the start attraction is on
	Node* starts = new Node, *starte = new Node;

	starts->m_co = holders[0].segment.start;	//push on the node represent the start of the street where the start attraction is on
	starts->g = distanceEarthMiles(holder_start, starts->m_co);
	starts->h = distanceEarthMiles(starts->m_co, holder_end);
	starts->f = starts->g + starts->h;
	starts->sname = holders[0].streetName;
	starts->parent = &startp;
	open.push_back(starts);

	starte->m_co = holders[0].segment.end;		//push on the node represent the start of the street where the start attraction is on
	starte->g = distanceEarthMiles(holder_start, starte->m_co);
	starte->h = distanceEarthMiles(starte->m_co, holder_end);
	starte->f = starte->g + starte->h;
	starte->sname = holders[0].streetName;
	starte->parent = &startp;
	open.push_back(starte);

	while (!open.empty())		//while there is still potential node
	{
		int best_index = 0;
		double b_f = open[0]->f;
		for (int n = 0; n != open.size(); n++)	//go over the open list and check find the one with great protential(indicated by f)
		{
			if (open[n]->f < b_f)
			{
				b_f = open[n]->f;
				best_index = n;
			}
		}
		Node* q = open[best_index];		//"go to" the node with greatest potential
		vector<Node*>::iterator p = open.begin() + best_index;
		close.push_back((*p));			//mark that we have been to here
		open.erase(p);					//remove the mode from open list

		vector<StreetSegment> holder = m_seg.getSegments(q->m_co);	//find all the potential successor of current node

		Node* successor;
		for (vector<StreetSegment>::iterator p = holder.begin(); p != holder.end(); p++)	//go over the successors
		{
			successor = new Node;
			if (!p->attractions.empty())	//if successor have attractions
			{
				for (vector<Attraction>::iterator n = p->attractions.begin(); n != p->attractions.end(); n++)	//go over the attractions
				{
					if (n->geocoordinates == holder_end)	//if the attraction is the end
					{
						
						double g = q->g + distanceEarthMiles(q->m_co, holder_end);
						double h = 0;
						double f = g;
						successor->m_co = holder_end;
						successor->f = f;
						successor->g = g;
						successor->h = h;
						successor->sname = p->streetName;
						successor->parent = q;
						
						reconstruction(successor, directions, holder_start);	//start reconstruct the road
						
						delete successor;
						for (int n = 0; n < close.size(); n++)		//clean up the dynamically allocated node
							delete close[n];
						for (int n = 0; n < open.size(); n++)
							delete open[n];

						return NAV_SUCCESS;
						
					}
				}
			}
			
			if (q->m_co == p->segment.start)			//if is at the end of a street, calculate the data needed to make a node
			{
				double g = q->g + distanceEarthMiles(p->segment.start, p->segment.end);
				double h = distanceEarthMiles(p->segment.end, holder_end);
				double f = g + h;
				successor->m_co = p->segment.end;
				successor->f = f;
				successor->g = g;
				successor->h = h;
				successor->sname = p->streetName;
			}
			else if (q->m_co == p->segment.end)			//if is at the end of a street, calculate the data needed to make the node
			{
				double g = q->g + distanceEarthMiles(p->segment.start, p->segment.end);
				double h = distanceEarthMiles(p->segment.start, holder_end);
				double f = g + h;
				successor->m_co = p->segment.start;
				successor->f = f;
				successor->g = g;
				successor->h = h;
				successor->sname = p->streetName;
			}

			bool cont = false;
			for (int n = 0; n < close.size(); n++)		//go through the close list
				if (close[n]->m_co == successor->m_co)	//if the node is already on close list
				{
					delete successor;
					cont = true;
					break;
				}

			if (cont == true)	//if is on the close list, go on
				continue;

			bool skip = false;
			for (int n = 0; n < open.size(); n++)	//go through the open list
				if (open[n]->m_co == successor->m_co && open[n]->f <= successor->f)	//if the node is already on the open list and the one on the open list have better potential
				{
					skip = true;	//indicate we should skip
					break;
				}

			if (skip)		//if we should skip
			{
				delete successor;
			}
			else		//if the successor have the potential of making up best road
			{
				successor->parent = q;
				open.push_back(successor);	//push it on open list
			}
		}

	}
	for (int n = 0; n < close.size(); n++)	//if we can't find a way, clean up the close list
		delete close[n];
	return NAV_NO_ROUTE;
	
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
