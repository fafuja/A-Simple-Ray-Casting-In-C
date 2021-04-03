
typedef enum { False = 0, True = 1 } bool;

typedef struct Vector2	{
	float x;
	float y;
} Vec2;

typedef struct Rects{
	Vec2 position, size, vertices[4];
} Rect;

typedef struct Rays{
	Vec2 position, direction;
} Ray;

