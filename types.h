
typedef enum { False = 0, True = 1 } bool;

typedef struct Vector2	{
	float x;
	float y;
} Vec2;

typedef struct Objects{
	Vec2 position, size;
} Object;

typedef struct Rays{
	Vec2 position, direction;
} Ray;

