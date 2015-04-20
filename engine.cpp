#include "engine.h"
#include "config.h"
#include "objects.h"
#include "init.h"
#include <vector>
#include <iostream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


using namespace std;

void CreateAsteoroids(vector<Asteroid*>& asteroids) {
    for (int i = 0; i < asteoridsNumber; i++) {
        int x = rand() % 760  + 20;
        int y = rand() % 560 + 20;
        //int x = 150;
        //int y = 100;
        float dir = .6 + rand() % 700 / 100;
        //float dir = .7;
        float speed = 2;
        float spin = .009;
        Asteroid* asteroid = new Asteroid(x, y, speed, dir, spin);
        //objects.push_back(asteroid);
        asteroids.push_back(asteroid);
   }
}

bool IsDestroied(Shoot* shoot, Asteroid* asteroid) {
    if (
        shoot->x > asteroid->x - 25 &&
        shoot->x < asteroid->x + 20 &&
        shoot->y > asteroid->y - 20 &&
        shoot->y < asteroid->y + 20
        ) {
        return true;
    } else {
        return false;
    }
}

bool IsDestroied(Spaceship* spaceship, Asteroid* asteroid) {

    if ((spaceship->x - 8 > asteroid->x + 20 - 1) || // is b1 on the right side of b2?
        (spaceship->y - 11 > asteroid->y + 20 - 1) || // is b1 under b2?
        (asteroid->x > spaceship->x - 20 + 15 - 1) || // is b2 on the right side of b1?
        (asteroid->y > spaceship->y + 20 - 1))   // is b2 under b1?
    {
        // no collision
        return false;
    }
    // collision
    return true;
}

void PrintWinner() {
    al_clear_to_color(al_map_rgb(0, 0, 0 ));
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform); 
    al_rotate_transform(&transform, 0); 
    al_translate_transform(&transform, 0, 0); 
    al_use_transform(&transform);
    al_draw_text(font, al_map_rgb(120,120,120), 300, 400, ALLEGRO_ALIGN_RIGHT, "You is the winner!\n");
    al_flip_display();
    al_rest(2.0);
};