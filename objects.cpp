#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/transformations.h>
#include "objects.h"
#include <iostream>
#include <math.h>
#include "init.h"
#include "config.h"
#include <cstdlib>


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

bool Object::Update() {
	int dx = (int)speed * sin(direction);
	int dy = (int)- speed * cos(direction);
	//cout << "x: SIN " << sin(direction) <<" "<< speed * sin(direction)<< endl;
	//cout << "y: COS " << cos(direction) <<" "<< speed * cos(direction) <<endl;
	//cout << y << "\t"<< pene <<endl;
	x += dx;
	y += dy; 
	//cout << "again\t" << y <<endl;
	//x += speed * dx;
	//y += speed * dy;
	return true;
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
	this->direction = 0;
	this->lives = startingLives;
	this->points = 0;
	this->age = 0;
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
	//updateAndWrap(x, -delta, cylinder, SCREEN_W);
	
}

void Spaceship::moveRight(const float delta) {
	this->direction += delta;
	//direction %= M_PI;
	//updateAndWrap(x, delta, cylinder, SCREEN_W);

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

bool Spaceship::Update() {
	Object::Update();
	if (cylinder) {
		x = (x % SCREEN_W + SCREEN_W) % SCREEN_W;
	}
	if (torus) {
		y = (y % SCREEN_H + SCREEN_H) % SCREEN_H;
	}
	if (age <= secondsOfInvincibility * FPS) {
		age += 1;
	}
	// check for collisions is on engine.cpp
	return true;
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
	this->stage = 0;
}

Asteroid::~Asteroid() {

}

void Asteroid::DebugPrint() {
	cout << "The follwing is an asteroid!" << endl;
	Object::DebugPrint();
	cout << "\tspin is :" << spin << endl;
	cout << "\trotated is :" << rotated << endl;
}

bool Asteroid::Update() {
	if (!Object::Update()) {
		return false;
	}
	rotated += spin;
	// bounce on vertical borders
	if (x - 20 <= 0 || x + 20 >= SCREEN_W) {
		direction = - direction;
	}
	// bounce on horizontal borders
	if (y - 20 <= 0 || y + 20 >= SCREEN_H){
		direction = M_PI - direction;
	}
	return true;
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
	this->player = 0; // support for multiplayer?
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

bool Shoot::Update() {
	if (!Object::Update()) {
		return false;
	}
	if (x * y < 0 || x > SCREEN_W || y > SCREEN_H) {
		return false;
	} else {
		return true;
	}
}

void Shoot::DebugPrint() {
	cout << "The follwing is a shoot!" << endl;
	Object::DebugPrint();
}

int wrap (int num, int max) {
	cout << num << max << endl;
	return ((num % max + max) % max);
}