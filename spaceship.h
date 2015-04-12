#include <stdio.h>
#include <allegro5/allegro.h>
#include "init.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/transformations.h>

const int deltaSpeed = 1;
const float deltaDegree = .09;

class Spaceship {
public:
    Spaceship(float x, float y, float direction);
    void DrawSpaceship();
    void Fire();
    void moveLeft(const double delta = deltaDegree);
    void moveRight(const double delta = deltaDegree);
    void passTime();
    void accelerate(const int delta = deltaSpeed);
    void decelerate(const int delta = deltaSpeed);
//private:
	float x;
    float y;
    double direction; 
    float speed;
    int live; 
    ALLEGRO_COLOR color;
};

void printSpaseship(ALLEGRO_COLOR color);
