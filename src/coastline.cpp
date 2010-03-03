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

#include "coastline.h"

#include <iostream>

#include "global.h"

Point** Coastline::addParabola(Node* newArc)
{
	Node* node = root_;
	if(node == 0)
	{
		root_ = newArc;

		return 0;
	}
	else
	{
		while(node->isLeaf() != true)
		{
			if(newArc->getPoint()->x() <= node->getPoint()->x())
			{
				node = node->getChild(LEFT);
			}
			else if(newArc->getPoint()->x() > node->getPoint()->x())
			{
				node = node->getChild(RIGHT);
			}
		}

		Point* leftCross;
		Point* rightCross;

		Node* newNode;

		if(node->getPoint()->y() == newArc->getPoint()->y())
		{
			leftCross = new ImplicitPoint(node->getPoint(), newArc->getPoint(), L);
			rightCross = new ExplicitPoint(leftCross->x(), 0);

		   	newNode = new Node(leftCross, false, node->getParent(), node, newArc);
			newArc->setParent(newNode);
		}
		else
		{
	   		leftCross = new ImplicitPoint(node->getPoint(), newArc->getPoint(), L);
			rightCross = new ImplicitPoint(node->getPoint(), newArc->getPoint(), R);

			newNode = new Node(leftCross, false, node->getParent(), 0, 0);
			Node* newNode2 = new Node(rightCross, false, newNode, 0, 0);
			Node* newNode4 = new Node(node->getPoint(), true, newNode2);

			newArc->setParent(newNode2);

			newNode->setChild(LEFT, node);
			newNode->setChild(RIGHT, newNode2);
			
			newNode2->setChild(LEFT, newArc);
			newNode2->setChild(RIGHT, newNode4);
		}

		if(node->getParent() == 0)
		{
			root_ = newNode;
		}
		else
		{
			if(node->getParent()->getChild(LEFT) == node)
			{
				node->getParent()->setChild(LEFT, newNode);
			}
			else
			{
				node->getParent()->setChild(RIGHT, newNode);
			}
		}

		node->setParent(newNode);

		Point** array = new Point*[2];
		array[0] = leftCross;
		array[1] = rightCross;
		return array;
	}
}

Point** Coastline::deleteArc(Node* delArc)
{
	Node* changedNode;
	Node* replacedNode;
	Node* tmp;

	Point* oldFirstCross;
	Point* oldSecondCross;

	Point* left;
	Point* right;

	Point* newCross;
	CrossSide side;

	int sign;
	if(delArc->getSide() == LEFT)
	{
		changedNode = findLeftCross(delArc);
		replacedNode = findRightCross(delArc);

		oldFirstCross = changedNode->getPoint();
		oldSecondCross = replacedNode->getPoint();

		left = findLeftNeighbour(delArc)->getPoint();
		right = findRightNeighbour(delArc)->getPoint();
		side = left->y() > right->y() ? R : L;
		newCross = new ImplicitPoint(left, right, side);

		replacedNode->getParent()->setChild(replacedNode->getSide(), replacedNode->getChild(RIGHT));
		replacedNode->getChild(RIGHT)->setParent(replacedNode->getParent());

		replacedNode->setChild(RIGHT, 0);
	}
	else
	{
		changedNode = findRightCross(delArc);
		replacedNode = findLeftCross(delArc);
		
		oldFirstCross = changedNode->getPoint();
		oldSecondCross = replacedNode->getPoint();

		left = findLeftNeighbour(delArc)->getPoint();
		right = findRightNeighbour(delArc)->getPoint();
		side = left->y() > right->y() ? R : L;
		newCross = new ImplicitPoint(left, right, side);
		
		replacedNode->getParent()->setChild(replacedNode->getSide(), replacedNode->getChild(LEFT));
		replacedNode->getChild(LEFT)->setParent(replacedNode->getParent());

		replacedNode->setChild(LEFT, 0);
	}

	changedNode->setPoint(newCross);

	delete replacedNode;

	Point** array = new Point*[2];
	array[0] = oldFirstCross;
	array[1] = oldSecondCross;
	array[2] = newCross;
	return array;
}

Node* Coastline::findLeftCross(Node* node)
{
	if(node == 0) return 0;
	else if(node->getParent() == 0) return 0;

	Node* neighbour = node;
	while(neighbour->getParent()->getChild(LEFT) == neighbour)
	{
		neighbour = neighbour->getParent();
		if(neighbour->getParent() == 0) return 0;
	}

	return neighbour->getParent();
}

Node* Coastline::findRightCross(Node* node)
{
	if(node == 0) return 0;
	else if(node->getParent() == 0) return 0;

	Node* neighbour = node;
	while(neighbour->getParent()->getChild(RIGHT) == neighbour)
	{
		neighbour = neighbour->getParent();
		if(neighbour->getParent() == 0) return 0;
	}

	return neighbour->getParent();
}

Node* Coastline::findLeftNeighbour(Node* node)
{
	Node* leftCross = findLeftCross(node);

	if(leftCross == 0) return 0;

	Node* neighbour = leftCross->getChild(LEFT);
	while(neighbour->isLeaf() == false)
	{
		neighbour = neighbour->getChild(RIGHT);
	}

	return neighbour;
}

Node* Coastline::findRightNeighbour(Node* node)
{
	Node* rightCross = findRightCross(node);

	if(rightCross == 0) return 0;

	Node* neighbour = rightCross->getChild(RIGHT);
	while(neighbour->isLeaf() == false)
	{
		neighbour = neighbour->getChild(LEFT);
	}

	return neighbour;
}
