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

Object::Object (int x, int y, float speed) {
	this->x = x;
	this->y = y;
	this->direction = 0;
	this->speed = speed;
	this->live = 1;
	this->color = al_map_rgb(0, 0, 0);
}

Object::~Object() {
}

bool Object::Update() {
	int dx = (int)speed * sin(direction);
	int dy = (int)- speed * cos(direction);
	x += dx;
	y += dy; 
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
	this->direction = 0;
	this->lives = startingLives;
	this->age = 0;
}

Spaceship::~Spaceship() {

}

void Spaceship::Draw() {
	if (age > 120 || age % 2 == 0) {
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
	} else {
		ALLEGRO_COLOR color = al_map_rgb(0, 255, 0);
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

}

void Spaceship::moveLeft(const float delta) {
	this->direction -= delta;
}

void Spaceship::moveRight(const float delta) {
	this->direction += delta;
}

Bullet* Spaceship::Fire() {
	Bullet* blast = new Bullet(x + 20 * sin(direction), y - 20 * cos(direction), maxSpeed + 2 , direction);
	return blast;
}

void Spaceship::accelerate(const float delta) {
	acceleration += delta;
	if (acceleration > maxAcceleration) {
		acceleration = maxAcceleration;
	}
}

void Spaceship::decelerate(const float delta) {
	if (speed == 0) {
		return;
	}
	acceleration -= delta;
	if (acceleration < - maxAcceleration) {
		acceleration = - maxAcceleration;
	}
}

bool Spaceship::Update() {
	speed += acceleration;
	if (speed > maxSpeed) {
		speed = maxSpeed;
	}
	if (speed < 0) {
		speed = 0;
	}
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
	this->y = y;
	this->direction = direction;
	this->color = al_map_rgb(0, 0, 255);
	this->speed = speed;
	this->spin = spin;
	this->rotated = 0;
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

// Bullet
Bullet::Bullet(int x, int y, float speed, float direction) : Object(x, y, speed) {
	this->color = al_map_rgb(0, 255, 255);
	this->direction = direction;
	this->player = 0; // support for multiplayer?
};

Bullet::~Bullet() {

}

void Bullet::Draw () {
	ALLEGRO_COLOR color = this->color;
	ALLEGRO_TRANSFORM transform; 
	al_identity_transform(&transform); 
	al_rotate_transform(&transform, direction); 
	al_translate_transform(&transform, x, y); 
	al_use_transform(&transform);
	al_draw_pixel(0 , 0 , color);
};

bool Bullet::Update() {
	if (!Object::Update()) {
		return false;
	}
	if (x * y < 0 || x > SCREEN_W || y > SCREEN_H) {
		return false;
	} else {
		return true;
	}
}

void Bullet::DebugPrint() {
	cout << "The follwing is a bullet!" << endl;
	Object::DebugPrint();
}
