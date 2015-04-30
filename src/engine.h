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

void CreateAsteoroids(std::vector<Asteroid*>&, int num);
void UpdateObjects();
void RepositionSpaceShips();
void Redraw();
void DrawPoints();
void DrawLives();
void GameOver();

void DispatchKeys();
void KeyDown(ALLEGRO_EVENT);
bool KeyUp(ALLEGRO_EVENT);

bool GameContinues();
void PrintWinner();
void InitGame();

bool IsDestroied(Bullet*, Asteroid*);
bool IsDestroied(Spaceship*, Asteroid*);

void CleanupAsteroids();
void CleanupBullets();
void CleanupSpaceships();
void Cleanup();

#endif