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

void CreateAsteoroids(std::vector<Asteroid*>&);

void PrintWinner();

bool IsDestroied(Shoot*, Asteroid*);
bool IsDestroied(Spaceship*, Asteroid*);

#endif