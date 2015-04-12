#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/transformations.h>
#include "objects.h"
#include <iostream>
#include <math.h>

using namespace std;

//ALLEGRO_COLOR red = al_map_rgb(255,0,0);

Object::Object (float x, float y, float direction) {
	this->x = x;
	this->y = y;
	this->direction = direction;
	this->speed = 0;
	this->live = 1;
	this->color = al_map_rgb(255,255,255);
}

void Object::passTime() {
	//x += 1;
	//y += 1;
}

Spaceship::Spaceship(float x, float y, float direction): Object(x, y, direction) {
	
	this->color = al_map_rgb(255, 0, 0);
}

void Spaceship::Draw() {
	color = this->color;
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

void Spaceship::moveLeft(const double delta) {
	//direction -= delta;
	//direction %= M_PI;
	x -= 1;
}

void Spaceship::moveRight(const double delta) {
	//direction += delta;
	//direction %= M_PI;
	x += 1;

}

void Spaceship::Fire() {
	cout << "FIRE" << endl;
}



void Spaceship::accelerate(const int delta) {
	//speed += deltaSpeed;
	y -= 1;
}

void Spaceship::decelerate(const int delta) {
	//speed -= deltaSpeed;
	y += 1;
}