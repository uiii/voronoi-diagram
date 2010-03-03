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

#ifndef EVENTCALENDER_H
#define EVENTCALENDER_H

#include <vector>
#include <algorithm>

#include "global.h"

class Node;

class Coastline;
class EventCalender;
class VoronoiDiagram;

enum EventType
{
	PLACE,
	CIRCLE
};

class Event
{
public:
	Event(Point* point, EventType type):
		point_(point),
		valid_(true),
		type_(type)
	{};

	virtual ~Event() = 0;

	virtual bool isValid() { return valid_; }	
	virtual void invalidate() { valid_ = false; }

	virtual Point* getPoint() const { return point_; }
	virtual EventType getType() const { return type_; }

	Node* getArc() { return arc_; }

protected:
	Point* point_;
	bool valid_;

	Node* arc_;

	EventType type_;
};

class PlaceEvent : public Event
{
public:
	PlaceEvent(Point* point);
};

class CircleEvent : public Event
{
public:
	CircleEvent(Point* point, Node* arc);
};

class EventCalender
{
public:
	EventCalender()
	{}

	void addEvent(Event* e);
	Event* getMostRecentEvent();

	bool empty() { return heap_.empty(); }
	void clear() { heap_.clear(); }

private:
	std::vector< Event* > heap_;

};

#endif
