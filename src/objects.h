#ifndef _OBJ_FILE_
#define _OBJ_FILE_

#include <stdio.h>
#include <allegro5/allegro.h>
#include "init.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/transformations.h>

const float deltaSpeed = .5;
const float deltaDegree = .09;

// forward declaration for IsDestroied function
class Asteroid;

class Object {
public:
    Object(int x, int y, float speed);
    virtual ~Object();
    virtual void Draw() =0;
    virtual bool Update();
    virtual void DebugPrint();
//private:
    int x;
    int y;
    float dx;
    float dy;
    double direction; 
    float speed;
    int live; 
    ALLEGRO_COLOR color;
};

class Bullet : public Object {
public:
    Bullet(int x, int y, float speed, float direction);
    ~Bullet();
    void Draw();
    bool Update();
    void DebugPrint();
    friend bool IsDestroied(Bullet*, Asteroid*);
    int player;

};

class Spaceship : public Object {
public:
    Spaceship(int x, int y, float speed);
    ~Spaceship();
    void Draw();
    Bullet* Fire();
    void moveLeft(const float delta = deltaDegree);
    void moveRight(const float delta = deltaDegree);
    void accelerate(const float delta = deltaSpeed);
    void decelerate(const float delta = deltaSpeed);
    bool Update();
    void DebugPrint();
    friend bool IsDestroied(Spaceship*, Asteroid*);
    int lives;
    int points;
    int age;
    float acceleration;
};

class Asteroid : public Object {
public:
    Asteroid(int x, int y, float speed, float direction, float spin);
    ~Asteroid();
    void Draw();
    bool Update();
    float spin;
    float rotated;
    void DebugPrint();
    friend bool IsDestroied(Bullet*, Asteroid*);
    friend bool IsDestroied(Spaceship*, Asteroid*);
    int stage;
};

#endif