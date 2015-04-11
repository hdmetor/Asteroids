#include <stdio.h>
#include <allegro5/allegro.h>
#include "init.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/transformations.h>

class Spaceship {
public:
    Spaceship(float xx, float yy);
    void DrawSpaceship();
//private:
	float x;
    float y;
    float direction; 
    float speed;
    int live; 
    ALLEGRO_COLOR color;
};

void printSpaseship(ALLEGRO_COLOR color);