#include <stdio.h>
#include "types.h"
#include "ray.h"

Vec2 CheckCollision(Ray* r, Rect* re)
{
	Vec2 col = {.x = -1, .y = -1};
	float x1 = r->position.x;
	float y1 = r->position.y;
	float x2 = r->position.x + (r->direction.x * 200);
	float y2 = r->position.y + (r->direction.y * 200);
	
		
	for(int i = 0; i < 4; i++)
	{
		for(int j = i+1; j < 4; j++){
		
			float x3 = re->vertices[i].x;
			float y3 = re->vertices[i].y;
			float x4 = re->vertices[j].x;
			float y4 = re->vertices[j].y;
			
			float den = ((x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4));

			if(den == 0){
				break;
			}

			float t = ((x1 - x3) * (y3 - y4)) - ((y1 - y3) * (x3 - x4));

			float u = ((x2 - x1) * (y1 - y3)) - ((y2 - y1) * (x1 - x3));
	
			if(t/den >= 0 && t/den <= 1 && u/den >= 0 && u/den <= 1)
			{
				col.x = x1 + t/den * (x2 - x1);
				col.y = y1 + t/den * (y2 - y1);
				return col;
			}
		}
	}
	
	return col;
}


