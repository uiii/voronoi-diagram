/*
Copyright (c) 2010 Richard Jedlička

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "fortunesAlgorithm.h"

#include <iostream>

double SWEEPLINE = 0;

void FortunesAlgorithm::initialize(std::vector< Point* >& points)
{
	vd_.clear();
	ec_.clear();
	cl_.clear();
	circleEventPoints_.clear();
	SWEEPLINE = 0;

	std::vector< Point* >::iterator it;
	Point* point;
	for(it = points.begin(); it < points.end(); ++it)
	{
		point = *it;
		if(point->x() > 640 || point->y() > 480 || point->x() < 0 || point->y() < 0) continue;
		
		ec_.addEvent(new PlaceEvent(point));
	}
}

void FortunesAlgorithm::build()
{
	while(! ec_.empty())
	{
		nextStep();
	}
	
	nextStep();
}

void FortunesAlgorithm::nextStep()
{
	if(! ec_.empty())
	{
		Event* e = ec_.getMostRecentEvent();
		while(e->isValid() == false && !ec_.empty())
		{
			e = ec_.getMostRecentEvent();
		}

		if(e->isValid() == true)
		{
			SWEEPLINE = e->getPoint()->y();

			switch(e->getType())
			{
				case PLACE:
					if(lastPoint != 0) if(lastPoint->x() == e->getPoint()->x() && lastPoint->y() == e->getPoint()->y()) return;
					lastPoint = e->getPoint();

					handlePlaceEvent(e);
					break;

				case CIRCLE:
					handleCircleEvent(e);
					break;
			}

			return;
		}
	}

	SWEEPLINE = 1000;

	std::vector< Edge* >& edges = vd_.getEdges();
	Point* A;
	Point* B;
	for(int i = 0; i < edges.size(); i++)
	{
		A = edges[i]->getLeft();
		B = edges[i]->getRight();

		double k = B->x() - A->x();
		double l = B->y() - A->y();

		double a = -l;
		double b = k;
		double c = -a * A->x() - b * A->y();

		double x = A->x();
		double y = A->y();

		if(x < 640 && x > 0 && y < 480 && y > 0)
		{
		}
		else
		{
			if(x > 640)		{ 	y = (a * 640 + c) / -b;		x = 640;	}
			else if(x < 0)	{	y = c / -b;					x = 0;		}
		
			if(y > 480)		{	y = 480;	x = (b * y + c) / -a;	}
			else if(y < 0)	{	y = 0;		x = c / -a; 			}

			edges[i]->replacePoint(A, new ExplicitPoint(x, y));
		}

		x = B->x();
		y = B->y();

		if(x < 640 && x > 0 && y < 480 && y > 0)
		{
		}
		else
		{
			if(x > 640)		{ 	y = (a * 640 + c) / -b;		x = 640;	}
			else if(x < 0)	{	y = c / -b;					x = 0;		}
		
			if(y > 480)		{	y = 480;	x = (b * y + c) / -a;	}
			else if(y < 0)	{	y = 0;		x = c / -a; 			}

			edges[i]->replacePoint(B, new ExplicitPoint(x, y));
		}
	}
}

void FortunesAlgorithm::handlePlaceEvent(Event* e)
{
	Node* newArc = e->getArc();
	Point** pointArray = cl_.addParabola(newArc);

	if(pointArray != 0)
	{
		vd_.addEdge(new Edge(pointArray[0], pointArray[1]));
	}

	recalculateCircleEvents(newArc);
	recalculateCircleEvents(cl_.findLeftNeighbour(newArc));
	recalculateCircleEvents(cl_.findRightNeighbour(newArc));

	//delete[] pointArray;
}

void FortunesAlgorithm::handleCircleEvent(Event* e)
{
	Node* delArc = e->getArc();

	Node* left = cl_.findLeftNeighbour(delArc);
	Node* right = cl_.findRightNeighbour(delArc);

	Point** cross = cl_.deleteArc(delArc);

	Point* newPoint = new ExplicitPoint(cross[0]->x(), cross[0]->y());
	cross[0]->getEdge()->replacePoint(cross[0], newPoint);
	cross[1]->getEdge()->replacePoint(cross[1], newPoint);

	vd_.addEdge(new Edge(newPoint, cross[2]));

	recalculateCircleEvents(left);
	recalculateCircleEvents(right);

	//delete cross[0];
	//delete cross[1];
	delete[] cross;
}

void FortunesAlgorithm::recalculateCircleEvents(Node* arc)
{
	if(arc == 0) return;

	Node* left = cl_.findLeftNeighbour(arc);
	Node* right = cl_.findRightNeighbour(arc);

	checkCircleEvent(left, arc, right);
}

void FortunesAlgorithm::checkCircleEvent(Node* left, Node* middle, Node* right)
{
	if(left == 0 || middle == 0 || right == 0)
	{
		return;
	}
	else if((left->getPoint() == middle->getPoint()) || (left->getPoint() == right->getPoint()) || (middle->getPoint() == right->getPoint()))
	{
		return;
	}

	if(middle->getEvent() != 0)
	{
		middle->getEvent()->invalidate();
		middle->setEvent(0);
	}

	const Point* A = left->getPoint();
	const Point* B = middle->getPoint();
	const Point* C = right->getPoint();

	double a = B->x() - A->x();
	double b = B->y() - A->y();
	double c = -a * ((A->x() + B->x()) / 2) - b * ((A->y() + B->y()) / 2);

	double k = B->x() - C->x();
	double l = B->y() - C->y();
	double m = -k * ((C->x() + B->x()) / 2) - l * ((C->y() + B->y()) / 2);

	if((a * l - b * k) >= 0)
	{
		return;
	}

	double x;
	double y;
	if(b != 0 && l != 0)
	{
		x = (b * m - l * c) / (a * l - b * k);
		y = (a * x + c) / -b;
	}
	else
	{
		if(b == 0)
		{
			x = (B->x() + A->x()) / 2;
			y = (k * x + m) / -l;
		}
		else
		{
			x = (B->x() + C->x()) / 2;
			y = (a * x + c) / -b;
		}
	}

	double r = sqrt(pow(A->x() - x, 2) + pow(A->y() - y, 2));


	Point* eventPoint = new ExplicitPoint(x, y + r);
	Event* circleEvent = new CircleEvent(eventPoint, middle);

	circleEventPoints_.push_back(eventPoint);
	ec_.addEvent(circleEvent);
}
