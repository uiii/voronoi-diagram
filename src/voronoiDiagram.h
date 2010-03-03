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

#ifndef VORONOIDIAGRAM_H
#define VORONOIDIAGRAM_H

#include <vector>
#include <list>

#include "global.h"

#include "eventCalender.h"
#include "coastline.h"

class Edge
{
public:
	Edge(Point* p1, Point* p2):
		p1_(p1),
		p2_(p2)
	{
		p1_->setEdge(this);
		p2_->setEdge(this);
	}

	void replacePoint(Point* oldPoint, Point* newPoint);

	Point* getLeft() { return p1_; }
	Point* getRight() { return p2_; }

private:
	Point* p1_;
	Point* p2_;
};

class VoronoiDiagram
{
public:
	VoronoiDiagram()
	{}

	void render();
	
	void addEdge(Edge* e) { edges_.push_back(e); }
	std::vector< Edge* >& getEdges() { return edges_; }

	void clear();

private:
	std::vector< Edge* > edges_;

};

#endif
