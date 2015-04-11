#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/transformations.h>
#include "spaceship.h"
#include <iostream>

using namespace std;

const float deltaDegree = .2;
//ALLEGRO_COLOR red = al_map_rgb(255,0,0);

void printSpaseship(ALLEGRO_COLOR color) {
	al_draw_line(-8, 9, 0, -11, color, 3.0f); 
	al_draw_line(0, -11, 8, 9, color, 3.0f); 
	al_draw_line(-6, 4, -1, 4, color, 3.0f); 
	al_draw_line(6, 4, 1, 4, color, 3.0f);
	
}

Spaceship::Spaceship(float xx, float yy) {
	x = xx;
	y = yy;
	direction = 0;
	speed = 0;
	live = 1;
	color = al_map_rgb(255,0,0);
}

void Spaceship::DrawSpaceship() {
	color = al_map_rgb(0,255,0);
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

void Spaceship::moveLeft() {
	direction -= deltaDegree;
}

void Spaceship::moveRight() {
	direction += deltaDegree;
}

void Spaceship::Fire() {
	cout << "FIRE" << endl;
}

void Spaceship::passTime() {
	x += 1;
	y += 1;
}