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

#ifndef GUI_H
#define GUI_H

#include <iostream>
#include <exception>
#include <string>
#include <fstream>
#include <vector>

#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"

#include "voronoiDiagram.h"
#include "fortunesAlgorithm.h"

enum GUIState
{
	DOTS,
	VORONOI
};

class GUI
{
public:
	GUI(std::string file = "");
	void run();
	void handleEvents();

private:
	bool init_GL();

	void renderDots();
	void renderVoronoiDiagram();
	void renderCircleEvents();

	void saveDots();
	void saveVoronoi();
	
	SDL_Surface* screen_;
	SDL_Event event_;

	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int SCREEN_BPP;

	std::vector< Point* > dots_;

	GUIState state_;

	FortunesAlgorithm fa_;
	VoronoiDiagram vd_;

	std::string file_;

	bool circleEvents;
};

#endif
