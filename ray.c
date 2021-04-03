#include <stdio.h>
#include "types.h"

bool CheckCollision(Ray* r, Object* o){
	// ray = x1 + t(x2 - x1) | y1 + t(y2 - y1)
	float x1 = r->position.x;
	float y1 = r->position.y;
	float x2 = r->position.x + (r->direction.x * 200);
	float y2 = r->position.y + (r->direction.y * 200);
	
	float x3 = o->position.x;
	float y3 = o->position.y;
	float x4 = o->position.x;
	float y4 = o->position.y + o->size.y;

	float den = ((x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4));

	if(den == 0){
		return False;
	}

	float t = ((x1 - x3) * (y3 - y4)) - ((y1 - y3) * (x3 - x4));

	float u = ((x2 - x1) * (y1 - y3)) - ((y2 - y1) * (x1 - x3));
	
	printf("%f %f \n", x2, y2);

	if(t/den >= 0 && t/den <= 1 && u/den >= 0 && u/den <= 1)
	{
		return True;
	}else{
		return False;
	}
}

