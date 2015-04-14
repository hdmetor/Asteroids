#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/transformations.h>
#include "objects.h"
#include <iostream>
#include <math.h>
#include "init.h"

// please use only one variable
int screen_w = 800;
int screen_h = 600;

const float maxSpeed = 5;

bool cylinder = true;
bool torus = true;

using namespace std;
void updateAndWrap(int& var, int diff, bool wrap, int max);
//ALLEGRO_COLOR red = al_map_rgb(255,0,0);

Object::Object (int x, int y, float direction) {
	this->x = x;
	this->y = y;
	this->direction = direction;
	this->speed = 0;
	this->live = 1;
	this->color = al_map_rgb(255,255,255);
}

Object::~Object() {
}

void Object::Update() {
	x += speed * sin(direction);
	y += - speed * cos(direction); 
}

Spaceship::Spaceship(int x, int y, float direction): Object(x, y, direction) {
	this->color = al_map_rgb(255, 0, 0);
}

Spaceship::~Spaceship() {

}

void Spaceship::Draw() {
	ALLEGRO_COLOR color = this->color;
	if (cylinder) {
		x = (x % screen_w + screen_w) % screen_w;
	}
	if (torus) {
		y = (y % screen_h + screen_h) % screen_h;
	}
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
	Shoot* blast = new Shoot(this->x + 20, this->y + 20, this->direction);
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
}

// Asteroid
Asteroid::Asteroid(int x, int y, float direction): Object(x, y, direction) {
	this->color = al_map_rgb(0, 0, 255);
}

Asteroid::~Asteroid(){

}

void Asteroid::Update() {
	this->x+=1;
}

void Asteroid::Draw() {
	color = this->color;
	ALLEGRO_TRANSFORM transform; 
	al_identity_transform(&transform); 
	al_rotate_transform(&transform, direction); 
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
Shoot::Shoot(int x, int y, float direction) : Object(x, y, direction) {
	this->color = al_map_rgb(0, 255, 0);
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
	al_draw_pixel(x, y, color);
};

void Shoot::Update() {
	int i = 0;
}

// Util
void updateAndWrap(int& var, int diff, bool wrap, int max) {
	var += diff;
	if (wrap) {
		var = (var % max + max) % max;
	}
}