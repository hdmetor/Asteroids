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

vector<Spaceship*> spaceships;
vector<Asteroid*> asteroids;
vector<Shoot*> shoots;

enum MYKEYS {
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ESCAPE, KEY_SPACE
};

bool pressedKeys[6] = { false, false, false, false, false, false };

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

void UpdateObjects() {
    for (int i = 0; i < spaceships.size(); i++) {
        bool lives = spaceships[i]->Update();
    }

    for (int i = 0; i < asteroids.size(); i++) {
        bool lives = asteroids[i]->Update();
    }

    for (int i = 0; i < shoots.size(); i++) {
        bool lives = shoots[i]->Update();
        if (!lives) {
            delete shoots[i];
            shoots.erase(shoots.begin() +i);
        }
    }

    // Check bullet vs asteroid
    for (int j = 0; j < asteroids.size(); j++) {
        Asteroid * asteroid = asteroids[j];
        bool lives = true;
        for (int i = 0; i < shoots.size(); i++) {
            Shoot* shoot = shoots[i];
            if (IsDestroied(shoot, asteroid)) {
                delete shoot;
                delete asteroids[j];
                shoots.erase(shoots.begin() + i);
                asteroids.erase(asteroids.begin() + j);
                lives = false;
            }
        }   

        // Check Asteroid vs Spaceship
        for (int i = 0; i <= spaceships.size(); i++) {
            if (lives) {
                if (IsDestroied(spaceships[i], asteroids[j])) {
                    spaceships[i]->lives -= 1;
                    cout << "Lives: " << spaceships[i]->lives << endl;
                }
            }
        }     
    }

}

void Redraw() {
    al_clear_to_color(al_map_rgb(0,0,0));
    for (int i = 0; i < spaceships.size(); i++) {
        spaceships[i]->Draw();
    }
    for (int i = 0; i < asteroids.size(); i++) {
        asteroids[i]->Draw();
    }
    for (int i = 0; i < shoots.size(); i++) {
        shoots[i]->Draw();
    }
al_flip_display();
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

void DispatchKeys() {
    if(pressedKeys[KEY_UP]) {
        spaceships[0]->accelerate();
    }
 
    if(pressedKeys[KEY_DOWN]) {
        spaceships[0]->decelerate();
    }

    if(pressedKeys[KEY_LEFT]) {
        spaceships[0]->moveLeft();
    }

    if(pressedKeys[KEY_RIGHT]) {
        spaceships[0]->moveRight();
    }

    if(pressedKeys[KEY_SPACE]) {
        Shoot* blast = spaceships[0]->Fire();
        shoots.push_back(blast);
    }
}

void KeyDown(ALLEGRO_EVENT event) {
    switch(event.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
            pressedKeys[KEY_UP] = true;
            break;

        case ALLEGRO_KEY_DOWN:
            pressedKeys[KEY_DOWN] = true;
            break;

        case ALLEGRO_KEY_LEFT: 
            pressedKeys[KEY_LEFT] = true;
            break;

        case ALLEGRO_KEY_RIGHT:
            pressedKeys[KEY_RIGHT] = true;
            break;

        case ALLEGRO_KEY_SPACE:
            pressedKeys[KEY_SPACE] = true;
            break;
    }
}


bool KeyUp(ALLEGRO_EVENT event) {
    switch(event.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
            pressedKeys[KEY_UP] = false;
            return false;
            break;

        case ALLEGRO_KEY_DOWN:
            pressedKeys[KEY_DOWN] = false;
            return false;
            break;

        case ALLEGRO_KEY_LEFT: 
            pressedKeys[KEY_LEFT] = false;
            return false;
            break;

        case ALLEGRO_KEY_RIGHT:
            pressedKeys[KEY_RIGHT] = false;
            return false;
            break;

        case ALLEGRO_KEY_SPACE:
            pressedKeys[KEY_SPACE] = false;
            return false;
            break;

        case ALLEGRO_KEY_ESCAPE:
            return true;
            break;
    }
    return true;
}

bool GameContinues() {
    if (asteroids.size() == 0) {
        PrintWinner();
        return false;
    } else {
        return true;
    }
}

void InitGame() {
    float spaceshipStartSpeed  = 0;
    Spaceship* spaceship = new Spaceship(300,300, spaceshipStartSpeed);
    spaceships.push_back(spaceship);
    CreateAsteoroids(asteroids);
}

void PrintWinner() {
    al_clear_to_color(al_map_rgb(0, 0, 0 ));
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform); 
    al_rotate_transform(&transform, 0); 
    al_translate_transform(&transform, 0, 0); 
    al_use_transform(&transform);
    al_draw_text(font, al_map_rgb(120,120,120), 300, 400, ALLEGRO_ALIGN_RIGHT, "A winner is you!\n");
    al_flip_display();
    al_rest(2.0);
};

void Cleanup() {
    for (int i = 0; i < spaceships.size(); i++) {
        delete spaceships[i];
    }
    for (int i = 0; i < asteroids.size(); i++) {
        delete asteroids[i];
    }
    for (int i = 0; i < shoots.size(); i++) {
        delete shoots[i];
    }
}