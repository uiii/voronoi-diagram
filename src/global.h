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

#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <utility>
#include <cmath>

#include "exception.h"

extern double SWEEPLINE;

class Edge;

enum CrossSide
{
	L = 0,
	R
};

class Point
{
public:
	Point(bool final):
		edge_(0),
		final_(final)
	{}

	virtual ~Point()
	{}

	virtual double x() const = 0;
	virtual double y() const = 0;

	void setEdge(Edge* e) { edge_ = e; }
	Edge* getEdge() const { return edge_; }

	virtual bool isFinal() { return final_; }

	virtual void print() const
	{
		std::cout << "[" << x() << ";" << y() << "]";
	}

	virtual std::ostream& getStream(std::ostream&) = 0;

	friend std::ostream& operator<<(std::ostream& os, Point* point)
	{
		if(point == 0) return os;
		else return point->getStream(os);
	}

private:
	Edge* edge_;

	bool final_;
};

class ExplicitPoint : public Point
{
public:
	ExplicitPoint(double x, double y):
		Point(true),
		x_(x),
		y_(y)
	{}

	double x() const { return x_; }
	double y() const { return y_; }

	std::ostream& getStream(std::ostream& os)
	{
		os << "[" << x() << ";" << y() << "]";
		return os;
	}

private:
	double x_;
	double y_;
};

class ImplicitPoint : public Point
{
public:
	ImplicitPoint(Point* FLeft, Point* FRight, CrossSide side):
		Point(false),
		FLeft_(FLeft),
		FRight_(FRight),
		side_(side)
	{}

	double x() const
	{
		if(FLeft_->y() == FRight_->y()) return (FLeft_->x() + FRight_->x()) / 2;

		double f1x = FLeft_->x();
		double f1y = FLeft_->y();

		double f2x = FRight_->x();
		double f2y = FRight_->y();

		double p1 = SWEEPLINE - f1y;
		double m1 = f1x;
		double n1 = f1y + p1 / 2;
		
		double p2 = SWEEPLINE - f2y;
		double m2 = f2x;
		double n2 = f2y + p2 / 2;

		double a = p2 - p1;
		double b = 2 * (m2 * p1 - m1 * p2);
		double c = 2 * p1 * p2 * (n2 - n1) + p2 * pow(m1, 2) - p1 * pow(m2, 2);

		double x1 = (-b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
		double x2 = (-b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);

		if(x1 > x2)
		{
			std::swap(x1, x2);
		}

		return side_ == L ? x1 : x2;
	}

	double y() const
	{
		double fx = FLeft_->x();
		double fy = FLeft_->y();

		double p = SWEEPLINE - fy;
		double m = fx;
		double n = fy + p / 2;

		double y = (pow(x(), 2) - 2 * x() * m + pow(m, 2) - 2 * p * n) / (-2 * p);

		return y;
	}

	std::ostream& getStream(std::ostream& os)
	{
		//os << FLeft_ << "x" << FRight_ << "x" << "[" << (side_ == L ? "L" : "R") << "]:[" << x() << ";" << y() << "]";
		os << "[" << x() << ";" << y() << "]";
		return os;
	}

private:
	Point* FLeft_; // left parabola focus
	Point* FRight_; // right parabola focus
	CrossSide side_;
};

#endif
