#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/transformations.h>
#include "objects.h"
#include <iostream>
#include <math.h>
#include "init.h"
#include <cstdlib>

// please use only one variable
int screen_w = 800;
int screen_h = 600;

const float maxSpeed = 7;

bool cylinder = true;
bool torus = true;

using namespace std;
//ALLEGRO_COLOR red = al_map_rgb(255,0,0);

int wrap (int num, int max);

Object::Object (int x, int y, float speed) {
	this->x = x;
	this->y = y;
	this->direction = 0;
	this->speed = speed;
	this->live = 1;
	this->color = al_map_rgb(255,255,255);
	this->name = 0;
}

Object::~Object() {
}

void Object::Update() {
	x += speed * sin(direction);
	y += - speed * cos(direction); 
}

void Object::DebugPrint() {
	cout << "Printing object" << endl;
	cout << "\tx is :" << x << endl;
	cout << "\ty is :" << y << endl;
	cout << "\tdirection is :" << direction << endl;
	cout << "\tspeed is :" << speed << endl;

}

void Spaceship::DebugPrint() {
	cout << "The follwing is a spaceship!" << endl;
	Object::DebugPrint();
}

Spaceship::Spaceship(int x, int y, float speed): Object(x, y, direction) {
	this->color = al_map_rgb(255, 0, 0);
	this->name = 1;
}

Spaceship::~Spaceship() {

}

void Spaceship::Draw() {
	ALLEGRO_COLOR color = this->color;
	ALLEGRO_TRANSFORM transform; 
	al_identity_transform(&transform); 
	al_rotate_transform(&transform, direction); 
	al_translate_transform(&transform, x, y); 
	al_use_transform(&transform);
	al_draw_line(-8, 9, 0, -11, color, 3.0f); 
	al_draw_line(0, -11, 8, 9, color, 3.0f); 
	al_draw_line(-6, 4, -1, 4, color, 3.0f); 
	al_draw_line(6, 4, 1, 4, color, 3.0f);
}

void Spaceship::moveLeft(const float delta) {
	this->direction -= delta;
	//direction %= M_PI;
	//updateAndWrap(x, -delta, cylinder, screen_w);
	
}

void Spaceship::moveRight(const float delta) {
	this->direction += delta;
	//direction %= M_PI;
	//updateAndWrap(x, delta, cylinder, screen_w);

}

Shoot* Spaceship::Fire() {
	Shoot* blast = new Shoot(x + 20 * sin(direction), y - 20 * cos(direction), maxSpeed + 2 , direction);
	return blast;
}

void Spaceship::accelerate(const float delta) {
	speed += deltaSpeed;
	if (speed > maxSpeed) {
		speed = maxSpeed;
	}
}

void Spaceship::decelerate(const float delta) {
	speed -= deltaSpeed / 2;
	if (speed < 0) {
		speed = 0;
	}
}

void Spaceship::Update() {
	Object::Update();
	if (cylinder) {
		x = (x % screen_w + screen_w) % screen_w;
	}
	if (torus) {
		y = (y % screen_h + screen_h) % screen_h;
	}
}

// Asteroid
Asteroid::Asteroid(int x, int y, float speed, float direction, float spin): Object(x, y, direction) {
	this->x = x;
	this->y =y;
	this->direction = direction;
	this->color = al_map_rgb(0, 0, 255);
	this->speed = speed;
	this->spin = spin;
	this->rotated = 0;
	this->name = 2;
}

Asteroid::~Asteroid() {

}

void Asteroid::DebugPrint() {
	cout << "The follwing is an asteroid!" << endl;
	Object::DebugPrint();
	cout << "\tspin is :" << spin << endl;
	cout << "\trotated is :" << rotated << endl;
}

void Asteroid::Update() {
	Object::Update();
	rotated += spin;
}

void Asteroid::Draw() {
	//color = this->color;
	ALLEGRO_TRANSFORM transform; 
	al_identity_transform(&transform); 
	al_rotate_transform(&transform, direction + rotated); 
	al_translate_transform(&transform, x, y); 
	al_use_transform(&transform);
	al_draw_line(-20, 20, -25, 5, color, 2.0f); 
	al_draw_line(-25, 5, -25, -10, color, 2.0f); 
	al_draw_line(-25, -10, -5, -10, color, 2.0f); 
	al_draw_line(-5, -10, -10, -20, color, 2.0f); 
	al_draw_line(-10, -20, 5, -20, color, 2.0f); 
	al_draw_line(5, -20, 20, -10, color, 2.0f); 
	al_draw_line(20, -10, 20, -5, color, 2.0f); 
	al_draw_line(20, -5, 0, 0, color, 2.0f); 
	al_draw_line(0, 0, 20, 10, color, 2.0f); 
	al_draw_line(20, 10, 10, 20, color, 2.0f); 
	al_draw_line(10, 20, 0, 15, color, 2.0f); 
	al_draw_line(0, 15, -20, 20, color, 2.0f);
}

// Shoot
Shoot::Shoot(int x, int y, float speed, float direction) : Object(x, y, speed) {
	this->color = al_map_rgb(0, 255, 255);
	this->direction = direction;
	this->name = 3;
};

Shoot::~Shoot() {

}

void Shoot::Draw () {
	ALLEGRO_COLOR color = this->color;
	ALLEGRO_TRANSFORM transform; 
	al_identity_transform(&transform); 
	al_rotate_transform(&transform, direction); 
	al_translate_transform(&transform, x, y); 
	al_use_transform(&transform);
	al_draw_pixel(0 , 0 , color);
};

void Shoot::Update() {
	Object::Update();
}

void Shoot::DebugPrint() {
	cout << "The follwing is a shoot!" << endl;
	Object::DebugPrint();
}

int wrap (int num, int max) {
	cout << num << max << endl;
	return ((num % max + max) % max);
}

/*int bounding_box_collision(int b1_x, int b1_y, int b1_w, int b1_h, int b2_x, int b2_y, int b2_w, int b2_h)
{
    if ((b1_x > b2_x + b2_w - 1) || // is b1 on the right side of b2?
        (b1_y > b2_y + b2_h - 1) || // is b1 under b2?
        (b2_x > b1_x + b1_w - 1) || // is b2 on the right side of b1?
        (b2_y > b1_y + b1_h - 1))   // is b2 under b1?
    {
        // no collision
        return 0;
    }
 
    // collision
    return 1;
}*/
