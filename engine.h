#ifndef ENGINE_FILE
#define ENGINE_FILE
#include <vector>
#include "objects.h"
#include "init.h"

//Allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

extern std::vector<Spaceship*> spaceships;
extern std::vector<Asteroid*> asteroids;
extern std::vector<Shoot*> shoots;

void CreateAsteoroids(std::vector<Asteroid*>&);
void UpdateObjects();
void Redraw();

void DispatchKeys();
void KeyDown(ALLEGRO_EVENT);
bool KeyUp(ALLEGRO_EVENT);

bool GameContinues();
void PrintWinner();

bool IsDestroied(Shoot*, Asteroid*);
bool IsDestroied(Spaceship*, Asteroid*);

void Cleanup();

#endif