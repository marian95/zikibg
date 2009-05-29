/* *************************************************************************
 * Player.h :
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

class Player {

	public:
		char colour;
		char directionFrom;
		bool turnToPlay;
		char *playerName;
		Chip chips[15];
		bool dice1Played;
		bool dice2Played;
		bool dice1Rolled;
		bool dice2Rolled;
	Player(){}

	~Player(){}

	Player(char colouri, char direction, char *name)
	{
		colour = colouri;
		directionFrom = direction;
		playerName = name;
		turnToPlay=false;
		dice1Played=false;
		dice2Played=false;
		dice1Rolled=false;
		dice2Rolled=false;
	}


	void setChips(int initialTY, int initialBY, int initialX, int chipSpacingY, int chip_w, int chip_h, float scale_chip)
	{
		int i = 0, pY = 0, posId=0;

		if (directionFrom == 't')
		{	
			pY = initialTY;
			posId=1;
		}else{
			pY = initialBY ;
			posId=24;
		}

		for( i=0; i<=14; i++)
		{
			chips[i] =  Chip( posId, colour, initialX, pY, scale_chip, directionFrom, chip_w , chip_h);
			if (directionFrom == 't')
				pY+= chipSpacingY;
			else 
				pY-= chipSpacingY;	
			chips[i].triangleOrder=i;
		}
	}

};
