/* *************************************************************************
 * aea-zikilib.h :
 * This is the support code library for the zikibg game.
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

bool wasItemClicked(SDL_Rect *item, int x , int y)
{
	int xS, xE, yS, yE;
	xS = item->x ;
	xE = item->x + item->w;
	yS = item->y ;
	yE = item->y + item->h;
        if ( (x > xS  ) && ( x < xE ) && (y > yS ) && ( y < yE) ){
	//printf ("x %d, y %d\txS %d\t xE %d\t ys %d\t yE %d\n",x,y, xS, xE, yS, yE);
                return true;
        }else
                return false;
}

void reconcileChipsXY(Triangle *t , Player *p1, Player *p2, int spacing, int iniTY, int iniBY)
{
	int i = 0, newY = 0;

	for (i=0;i<15;i++)
	{
		if(p1->chips[i].posId == t->posId)
		{
			p1->chips[i].rectd.x = t->rect.x;
			if (t->top_bottom == 't')
			{
				newY = iniTY + ( spacing * (p1->chips[i].triangleOrder) );
                		p1->chips[i].rectd.y = newY;
			}
			if (t->top_bottom == 'b')
			{
				newY = iniBY - ( spacing * (p1->chips[i].triangleOrder) );
                		p1->chips[i].rectd.y = newY;
			}

		}
		if(p2->chips[i].posId == t->posId)
		{
			p2->chips[i].rectd.x = t->rect.x;
			if (t->top_bottom == 't')
			{
				newY = iniTY + ( spacing * (p2->chips[i].triangleOrder) );
                		p2->chips[i].rectd.y = newY;
			}
			if (t->top_bottom == 'b')
			{
				newY = iniBY - ( spacing * (p2->chips[i].triangleOrder) );
                		p2->chips[i].rectd.y = newY;
			}

		}
	}
}

void drawChip(Chip *chip, SDL_Surface *screen, SDL_Surface *bmp_chip_b, SDL_Surface *bmp_chip_w)
{
	if (chip->colour == 'b')
        	SDL_BlitSurface(bmp_chip_b, &chip->rects, screen, &chip->rectd); 
        else
        	SDL_BlitSurface(bmp_chip_w, &chip->rects, screen, &chip->rectd);
}
void drawChips (Player *p1, Player *p2, SDL_Surface *screen, SDL_Surface *bmp_chip_b, SDL_Surface *bmp_chip_w, SDL_Event *event)
{
	// posId  0: top bank
	// posid 25: bottom bank

	int posId=0, chipCount=0, tOrder=0;

	for (posId=0;posId<=25;posId++)
	{
		for (tOrder=0; tOrder<30; tOrder++)
		{
			for(chipCount=0; chipCount<15; chipCount++)
			{
				if (p1->chips[chipCount].posId==posId && p1->chips[chipCount].triangleOrder==tOrder && !p1->chips[chipCount].wandering)
					drawChip(&p1->chips[chipCount], screen, bmp_chip_b, bmp_chip_w);
				if (p2->chips[chipCount].posId==posId && p2->chips[chipCount].triangleOrder==tOrder && !p2->chips[chipCount].wandering)
					drawChip(&p2->chips[chipCount], screen, bmp_chip_b, bmp_chip_w);
			}
		}
	
	}

	for(chipCount=0;chipCount<15;chipCount++)
	{
		if (p1->chips[chipCount].wandering)
		{
			p1->chips[chipCount].setXY(event->button.x, event->button.y);
	                drawChip(&p1->chips[chipCount], screen, bmp_chip_b, bmp_chip_w);
		}
		if (p2->chips[chipCount].wandering)
		{
			p2->chips[chipCount].setXY(event->button.x, event->button.y);
                        drawChip(&p2->chips[chipCount], screen, bmp_chip_b, bmp_chip_w);
		}
	}

	//SDL_Flip(screen);
}

bool positionPlayable (int triPosId, char direction, int dice, Chip *chip)
{
	printf("Running playable %c \n",direction );
	if (direction == 't')
	{
		printf("TriPos: %d\tChipPos: %d\t Dice: %d\n",triPosId,chip->posId, dice);
		if ((chip->posId + dice) == triPosId)
			return true;
		else
			return false;
	} else {
		printf("TriPos: %d\tChipPos: %d\t Dice: %d\n",triPosId,chip->posId, dice);
		if ((chip->posId - dice) == triPosId)
			return true;
		else
			return false;
	}
}

void playChip(Player *p, Chip *chip, Chip *topchip, int dice1, int dice2, int posId)
{
	puts("Running playChip");
			if (topchip && topchip->triangleOrder > 0 && topchip->colour != p->colour)
			{
				printf("Cannot rest chip on top of oponnets chips.\n");
				return;
			}
			// Player is attempting to rest his chip in a position
			// and so far the top chip of this position is valid.
			//
			//   Check that this position is within dice roll 
			if (!p->dice1Played && positionPlayable(posId, p->directionFrom, dice1, chip))
			{
			puts("111");
				// 1. Put to top of stack of triangle
				if (topchip && topchip!=chip)
					chip->triangleOrder = topchip->triangleOrder + 1;
				else
					chip->triangleOrder = 0;
			puts("222");

				// --2. Chip has new x/y
				// 3. Chip has new posId
				chip->posId=posId;
				// 4. Chip not wandering
			puts("333");
				chip->wandering=false;
				// 5. Player.dice1Played = true
				p->dice1Played=true;
			puts("444");
				return ;
			}
			if (!p->dice2Played && positionPlayable (posId, p->directionFrom, dice2, chip))
			{
				// 1. Put to top of stack of triangle
				if (topchip && topchip!=chip)
					chip->triangleOrder = topchip->triangleOrder + 1;
				else
					chip->triangleOrder = 0;
				// --2. Chip has new x/y
				// 3. Chip has new posId
				chip->posId=posId;
				// 4. Chip not wandering
				chip->wandering=false;
				// 5. Player.dice1Played = true
				p->dice2Played=true;
				return ;
			}
}

Chip * getWanderingChip(Player *p1, Player *p2, int playersTurn)
{
	Chip *c = NULL;
        int i=0;
    	if(playersTurn == 1)
      	{
		for (i=0; i < 15; i++)
		{
			if ( p1->chips[i].wandering)
				c = &p1->chips[i];
		}
	} else {
		for (i=0; i < 15; i++)
		{
			if ( p2->chips[i].wandering)
				c = &p2->chips[i];
		}

	}
	return c;
}

Chip * getTopChip(int posId, Player *p1, Player *p2, int player)
{
	Chip *c = NULL;

	int t=0, i=0;

	for (i=0; i < 15; i++)
	{
		if ( p1->chips[i].posId == posId && p1->chips[i].triangleOrder >= t)
		{
			c= &p1->chips[i];
			t= c->triangleOrder;
		}
		if ( p2->chips[i].posId == posId && p2->chips[i].triangleOrder >= t)
		{
			c= &p2->chips[i];
			t= c->triangleOrder;
		}
	}
//	if(player==1)
//		c=&p1->chips[i];
//	else
//		c= &p2->chips[i];

	return c;
}

void cancelPlay (Player *p1, Player *p2, int playersTurn)
{
	Chip *chip = getWanderingChip(p1,p2,playersTurn);
	if (chip)
		chip->wandering=false;

}
int doChip (int posId, Player *p1, Player *p2, int playersTurn, int dice1, int dice2)
{
	if (playersTurn ==1 && (!p1->dice1Rolled || !p1->dice2Rolled))
		return 0;
	if (playersTurn ==2 && (!p2->dice1Rolled || !p2->dice2Rolled))
		return 0;

	Chip *topchip = getTopChip(posId, p1, p2, playersTurn);
	Chip *chip = getWanderingChip(p1,p2,playersTurn);
	
	if (!chip)
	{
		printf("No wandering chips, going to pick-up\n");
              	// pickUpChip
		if (topchip)
		{
		  if(playersTurn ==1 && topchip->colour == p1->colour)
		  {
			printf("Pickup: Found a chip on posId %d to pickup.\n", posId);
			topchip->wandering=true;
		  }
		  if(playersTurn ==2 && topchip->colour == p2->colour)
		  {
			printf("Pickup: Found a chip on posId %d to pickup.\n", posId);
			topchip->wandering=true;
		  }
		} else {
			printf("Pickup: No chips on posId %d to pickup.\n", posId);
			
		}
		return 0;
	}

	// Drop the chip
	printf("Have wandering chip, going to drop\n");
	if (playersTurn == 1)
		playChip(p1, chip, topchip, dice1, dice2, posId);
	if (playersTurn == 2)
		playChip(p2, chip, topchip, dice1, dice2, posId);
	return 1;
}

void cpu_play (Player *p1, Player *p2, int playersTurn) {

	int i=0;
	Chip *topchip =  NULL; 
	Player *p = NULL ;

	if (playersTurn == 1)
		p = p1;	
	else 
		p = p2;

// TODO check which direction and set -- or ++
	for (i=0; i=25; i++)
	{
 		topchip = getTopChip(i, p1, p2, playersTurn);
		if (topchip && topchip->colour == p->colour)
		{
			

		}
		
	}
}


