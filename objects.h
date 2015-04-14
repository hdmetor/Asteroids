#ifndef _OBJ_FILE_
#define _OBJ_FILE_

#include <stdio.h>
#include <allegro5/allegro.h>
#include "init.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/transformations.h>

const int deltaSpeed = 1;
const float deltaDegree = .09;
//const int deltaDegree = 1;

class Object {
public:
    Object(int x, int y, float direction);
    ~Object();
    virtual void Draw() =0;
    virtual void Update() =0;
//private:
    int x;
    int y;
    double direction; 
    float speed;
    int live; 
    ALLEGRO_COLOR color;
};

class Shoot : public Object {
public:
    Shoot(int x, int y, float direction);
    ~Shoot();
    void Draw();
    void Update();
};

class Spaceship : public Object {
public:
    Spaceship(int x, int y, float direction);
    ~Spaceship();
    void Draw();
    Shoot* Fire();
    void moveLeft(const float delta = deltaDegree);
    void moveRight(const float delta = deltaDegree);
    void accelerate(const int delta = deltaSpeed);
    void decelerate(const int delta = deltaSpeed);
    void Update();
};

class Asteroid : public Object {
public:
    ~Asteroid();
    Asteroid(int x, int y, float direction);
    void Draw();
    void Update();
};



#endif