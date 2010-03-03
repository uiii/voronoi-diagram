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

#ifndef COASTLINE_H
#define COASTLINE_H

#include "global.h"

class Event;

enum ChildSide
{
	LEFT = 0,
	RIGHT
};

class Node
{
public:
	Node(Point* point = 0, bool leaf = false, Node* parent = 0, Node* leftChild = 0, Node* rightChild = 0):
		point_(point),
		leaf_(leaf),
		parent_(parent),
		event_(0)
	{
		child_ = new Node*[2];
		setChildren(leftChild, rightChild);
	}

	~Node()
	{
		delete child_[LEFT]; 
		delete child_[RIGHT];
		delete[] child_;
	}

	void setPoint(Point* point) { point_ = point; }

	void setChildren(Node* left, Node* right)
	{
		setChild(LEFT, left);
		setChild(RIGHT, right);
	}

	void setChild(ChildSide side, Node* child)
	{
		child_[side] = child;
		if(child != 0) child->setSide(side);
	}

	void setParent(Node* parent) { parent_ = parent; }
	void setSide(ChildSide side) { side_ = side; }
	void setEvent(Event* event) { event_ = event; }

	Point* getPoint() { return point_; }
	Node* getChild(ChildSide side) { return child_[side]; }
	Node* getParent() { return parent_; }
	ChildSide getSide() { return side_; }
	Event* getEvent() { return event_; }
	
	bool isLeaf() { return leaf_; }

	std::ostream& getStream(std::ostream& os)
	{
		os << "<" << point_ << ">" << (leaf_ ? "<l>" : "")
				<< "(" << child_[LEFT] << "," << child_[RIGHT] << ")";

		return os;
	}

	friend std::ostream& operator<<(std::ostream& os, Node* node)
	{
		if(node == 0) return os;
		else return node->getStream(os);
	}

private:
	Point* point_;

	Node** child_; // array of children

	Node* parent_;
	ChildSide side_;

	Event* event_; // circle event

	bool leaf_;
};

class Coastline
{
public:
	Coastline():
		root_(0)
	{}

	Point** addParabola(Node* arc);
	Point** deleteArc(Node* delArc);

	Node* findLeftCross(Node* node);
	Node* findRightCross(Node* node);

	Node* findLeftNeighbour(Node* node);
	Node* findRightNeighbour(Node* node);

	void clear() { delete root_; root_ = NULL; }

	void print(std::string str = "")
	{
		std::cout << str << "CL: " << root_ << std::endl;
	}

private:
	Node* root_;
};

#endif
