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

#include "eventCalender.h"
#include "voronoiDiagram.h"
#include "coastline.h"

Event::~Event(){}

PlaceEvent::PlaceEvent(Point* point):
	Event(point, PLACE)
{
	arc_ = new Node(point, true);
}

CircleEvent::CircleEvent(Point* point, Node* arc):
	Event(point, CIRCLE)
{
	arc_ = arc;
	arc->setEvent(this);
}

bool comp(const Event* e1, const Event* e2)
{
	if(e1->getPoint()->y() == e2->getPoint()->y())
	{
		return e1->getPoint()->x() > e2->getPoint()->x();
	}
	else
	{
		return e1->getPoint()->y() > e2->getPoint()->y();
	}
}

void EventCalender::addEvent(Event* e)
{
	heap_.push_back(e);
	std::push_heap(heap_.begin(), heap_.end(), comp);
}

Event* EventCalender::getMostRecentEvent()
{
	if(heap_.empty())
	{
		throw Exception("Event calender is empty");
	}

	std::pop_heap(heap_.begin(), heap_.end(), comp);
	Event* e = heap_.back();
	heap_.pop_back();
	return e;
}
