/* *************************************************************************
 * Chip.h :
 * This is one of the classes used in the zikibg game.
 * *************************************************************************
 * Author: Amer El-Abed
 * Email : amer.elabed@gmail.com
 * Blog  : aelabed.blogspot.com
 * 
 * Copyright 2009 Amer El-Abed
 * *************************************************************************
 * License: 
    This file is part of zikibg.

    zikibg is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    zikibg is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with zikibg.  If not, see <http://www.gnu.org/licenses/>.
 * *************************************************************************
 */

#include "Play.h"
class Chip {

	public:
		 int posId;
		char colour;
		int triangleOrder;
//	public int offsetX;
//	public int offsetY;
		double scale;
		char top_bottom;
		SDL_Rect rects, rectd;
		Play play;
		bool wandering;

	Chip(){}

	~Chip(){}

	Chip(int posIdi, char colouri, int offsetXi, int offsetYi, double scalei, char tobo, int w, int h)
	{
		posId=posIdi;
		colour= colouri;
	//	offsetX= offsetXi;
	//	offsetY= offsetYi;
		scale= scalei;
		rects.x=0;
		rects.y=0;	
		rects.w=w;
		rects.h=h;

		rectd.x=offsetXi;
		rectd.y=offsetYi;
		rectd.w=w;
		rectd.h=h;
	
		triangleOrder=0;

		top_bottom=tobo;
		wandering=false;
	}
	void setXY(int x , int y)
	{
	//	rects.x = rectd.x;
	//	rects.y = rectd.y;
		rectd.x = x;
		rectd.y = y;
	}
};
