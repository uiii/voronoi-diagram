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

#include "gui.h"
#include "global.h"

GUI::GUI(std::string file):
	screen_(NULL),
	SCREEN_WIDTH(640),
	SCREEN_HEIGHT(480),
	SCREEN_BPP(32),
	state_(DOTS),
	fa_(vd_),
	file_(file),
	circleEvents(true)
{
	if(file != "")
	{
		std::ifstream fin(file_.c_str());

		double x, y;
		while(fin.good())
		{
			fin >> x;
			if(fin.good())
			{
				fin >> y;
				dots_.push_back(new ExplicitPoint(x,y));
			}
		}

		fin.close();
	}

	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{
		throw Exception("chyba inicializace");
	}
	
	if( SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_OPENGL ) == NULL )
	{
		throw Exception("chyba inicializace");
	}
	
	if( init_GL() == false )
	{
		throw Exception("chyba inicializace");
	}
	
	SDL_WM_SetCaption( "Voronoi", NULL );
} 

bool GUI::init_GL()
{
    glClearColor( 0, 0, 0, 0 );
	glColor4f( 1.0, 1.0, 1.0, 0 );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1 );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    if( glGetError() != GL_NO_ERROR )
    {
        return false;
    }

    return true;
}

void GUI::run()
{
		bool quit = false;
		while(quit == false)
		{
			if( SDL_PollEvent( &event_ ) )
        	{
				handleEvents();

	            if( event_.type == SDL_QUIT )
        	    {
    	            quit = true;
        	    }
        	}

	    	glClear( GL_COLOR_BUFFER_BIT );

			renderDots();

			if(state_ == VORONOI)
			{
				renderVoronoiDiagram();
				if(circleEvents) renderCircleEvents();
			}

	    	SDL_GL_SwapBuffers();

			//SDL_Delay(10);
		}

		SDL_Quit();
}

void GUI::renderDots()
{
	double x,y;
	for(int i = 0; i < dots_.size(); i++)
	{
		x = dots_[i]->x() - 2;
		y = dots_[i]->y();

		glTranslatef(x, y, 0);

		glBegin( GL_QUADS );

			glVertex3f( 0, 0, 0 );
			glVertex3f( 2, -2, 0 );
			glVertex3f( 5, 0, 0 );
			glVertex3f( 2, 2, 0 );

		glEnd();

		glLoadIdentity();				
	}
}

void GUI::renderVoronoiDiagram()
{
	std::vector< Edge* >& edges = vd_.getEdges();

	glBegin(GL_LINES);

		glVertex2f(0, SWEEPLINE);
		glVertex2f(SCREEN_WIDTH, SWEEPLINE);

	glEnd();

	for(int i = 0; i < edges.size(); i++)
	{
		Point* left = edges[i]->getLeft();
		Point* right = edges[i]->getRight();

		glBegin(GL_LINES);

			glVertex2f(left->x(), left->y());
			glVertex2f(right->x(), right->y());

		glEnd();

		glLoadIdentity();
	}
}

void GUI::renderCircleEvents()
{
	std::vector< Point* > points = fa_.getCircleEventPoints();

	glPushAttrib(GL_CURRENT_BIT);
	glColor4f(1.0, 0, 0, 0);

	double x,y;
	for(int i = 0; i < points.size(); i++)
	{
		x = points[i]->x() - 2;
		y = points[i]->y();

		glTranslatef(x, y, 0);

		glBegin( GL_QUADS );

			glVertex3f( 0, 0, 0 );
			glVertex3f( 2, -2, 0 );
			glVertex3f( 5, 0, 0 );
			glVertex3f( 2, 2, 0 );

		glEnd();

		glLoadIdentity();				
	}

	glPopAttrib();
}

void GUI::handleEvents()
{
	double x, y;

	if(state_ == DOTS)
	{
		if(event_.type == SDL_MOUSEBUTTONDOWN)
    	{
			if(event_.button.button == SDL_BUTTON_LEFT)
			{
            	x = event_.button.x;
            	y = event_.button.y;

				std::cout << "click: " << x << " " << y << std::endl;
				dots_.push_back(new ExplicitPoint(x,y));
			}
			else if(event_.button.button == SDL_BUTTON_RIGHT)
			{
				if(dots_.size() > 0)
				{
					delete dots_.back();
					dots_.pop_back();
				}
			}
		}
		else if(event_.type == SDL_KEYDOWN)
		{
			switch(event_.key.keysym.sym)
			{
				case SDLK_RETURN:
    				glClearColor( 0xFF, 0xFF, 0xFF, 0 );
					glColor4f( 0, 0, 0, 0 );

					fa_.initialize(dots_);

					state_ = VORONOI;
					break;

				case SDLK_ESCAPE:
					event_.type = SDL_QUIT;
					break;

				case SDLK_s:
					saveDots();
					break;
			}
		}
	}
	else if(state_ == VORONOI)
	{
		if(event_.type == SDL_KEYDOWN)
		{
			switch(event_.key.keysym.sym)
			{
				case SDLK_RETURN:
					fa_.build();
					saveVoronoi();
					break;

				case SDLK_ESCAPE:
    				glClearColor( 0, 0, 0, 0 );
					glColor4f( 1.0, 1.0, 1.0, 0 );

					state_ = DOTS;
					break;

				case SDLK_SPACE:
					fa_.nextStep();
					saveVoronoi();
					break;

				case SDLK_c:
					circleEvents = !circleEvents;
					break;
			}
		}
	}
}

void GUI::saveDots()
{
	if(file_ == "") file_ = "vstup";
	std::ofstream fout(file_.c_str());

	for(int i = 0; i < dots_.size(); i++)
	{
		fout << dots_[i]->x() << " " << dots_[i]->y() << "\n";
	}

	fout.close();
}

void GUI::saveVoronoi()
{
	std::ofstream fout("vystup");

	std::vector< Edge* > edges = vd_.getEdges();
	for(int i = 0; i < edges.size(); i++)
	{
		fout << edges[i]->getLeft() << " - " << edges[i]->getRight() << "\n";
	}

	fout.close();
}
