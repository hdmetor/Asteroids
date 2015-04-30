#include "engine.h"
#include "config.h"
#include "objects.h"
#include "init.h"
#include <vector>
#include <iostream>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


using namespace std;

vector<Spaceship*> spaceships;
vector<Asteroid*> asteroids;
vector<Bullet*> bullets;

enum MYKEYS {
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ESCAPE, KEY_SPACE
};

bool pressedKeys[6] = { false, false, false, false, false, false };

void CreateAsteoroids(vector<Asteroid*>& asteroids, int num) {
    for (int i = 0; i < num; i++) {
        int x = rand() % (SCREEN_W - 20) + 20;
        int y = rand() % (SCREEN_H - 20) + 20;
        //int x = 150;
        //int y = 100;
        float dir = .6 + rand() % 700 / 100;
        //float dir = .7;
        float speed = 2;
        float spin = .009;
        Asteroid* asteroid = new Asteroid(x, y, speed, dir, spin);
        asteroids.push_back(asteroid);
   }
}

void UpdateObjects() {
    bool restart = false;
    for (int i = 0; i < spaceships.size(); i++) {
        bool lives = spaceships[i]->Update();
    }

    for (int i = 0; i < asteroids.size(); i++) {
        bool lives = asteroids[i]->Update();
    }

    for (int i = 0; i < bullets.size(); i++) {
        bool lives = bullets[i]->Update();
        if (!lives) {
            delete bullets[i];
            bullets.erase(bullets.begin() +i);
        }
    }

    // Check bullet vs asteroid
    for (int j = 0; j < asteroids.size(); j++) {
        if (!restart) {
            Asteroid * asteroid = asteroids[j];
            bool lives = true;
            for (int i = 0; i < bullets.size(); i++) {
                Bullet* bullet = bullets[i];
                if (lives && IsDestroied(bullet, asteroid)) {
                    // SplitAsteroid();
                    spaceships[bullet->player]->points += 50*(1 + asteroid->stage);
                    delete bullet;
                    bullets.erase(bullets.begin() + i);
                    lives = false;
                }
            }   

            // check Asteroid vs Spaceship
            // spaceship is invincible in the for the first 2 seconds
            for (int i = 0; i <= spaceships.size(); i++) {
                if (lives && !restart) {
                    if (spaceships[i]->age > secondsOfInvincibility * FPS &&
                        IsDestroied(spaceships[i], asteroids[j])) {
                        spaceships[i]->lives -= 1;
                        restart = true;
                        continue;
                        
                    }
                }
            }  

            // if the asteroid was destroied, we remove it
            if (!lives && !restart) {
                delete asteroids[j];
                asteroids.erase(asteroids.begin() + j);
            }   
        }
    }

    if (rand() % 95 == 0) {
        CreateAsteoroids(asteroids, 1);
    }

    if (restart) {
        RepositionSpaceShips();
        return;
    }

}

void RepositionSpaceShips() {
    for (int i = 0; i < spaceships.size(); i++) {
        spaceships[i]->x = 300 + 2* i;
        spaceships[i]->y = 300 + 2* i;
        spaceships[i]->speed = 0;
        spaceships[i]->age = 0;
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
    for (int i = 0; i < bullets.size(); i++) {
        bullets[i]->Draw();
    }
    DrawPoints();
    DrawLives();
    al_flip_display();
}

void DrawPoints() {
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform); 
    al_rotate_transform(&transform, 0); 
    al_translate_transform(&transform, 0, 0); 
    al_use_transform(&transform);
    int bufferSize = 40;
    char buffer[bufferSize];
    sprintf(buffer, "Points: %d", spaceships[0]->points);
    al_draw_text(
                 font, 
                 al_map_rgb(0, 0, 255), 
                 SCREEN_W - 40, 20, ALLEGRO_ALIGN_RIGHT, 
                 buffer); 
    // In case of multiplayer, draw points here  
}

void DrawLives() {
    int s = 0;
    ALLEGRO_COLOR color = al_map_rgb(0, 255, 0);
    for (int i = 0; i < spaceships[s]->lives; i++){
        ALLEGRO_TRANSFORM transform; 
        al_identity_transform(&transform); 
        al_rotate_transform(&transform, 0); 
        al_translate_transform(&transform, 20 * (i + 1), 20); 
        al_use_transform(&transform);
        al_draw_line(-8, 9, 0, -11, color, 3.0f); 
        al_draw_line(0, -11, 8, 9, color, 3.0f); 
        al_draw_line(-6, 4, -1, 4, color, 3.0f); 
        al_draw_line(6, 4, 1, 4, color, 3.0f);
    }
}

bool IsDestroied(Bullet* bullet, Asteroid* asteroid) {
    if (
        bullet->x > asteroid->x - 25 &&
        bullet->x < asteroid->x + 20 &&
        bullet->y > asteroid->y - 20 &&
        bullet->y < asteroid->y + 20
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
        Bullet* blast = spaceships[0]->Fire();
        bullets.push_back(blast);
        pressedKeys[KEY_SPACE] = false;
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
            spaceships[0]->acceleration = 0;
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
    } else if (spaceships[0]-> lives < 0) {
        GameOver();
        return false;
    } else {
        return true;
    }
}

void InitGame() {
    for (int i = 0; i < numPlayer; i++) {
        Spaceship* spaceship = new Spaceship(300,300, spaceshipStartSpeed);
        spaceship->points = 0;
        spaceships.push_back(spaceship);
    }
    CreateAsteoroids(asteroids, asteoridsNumber);
}

void PrintWinner() {
    al_clear_to_color(al_map_rgb(0, 0, 0 ));
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform); 
    al_rotate_transform(&transform, 0); 
    al_translate_transform(&transform, 0, 0); 
    al_use_transform(&transform);
    int bufferSize = 40;
    char buffer[bufferSize];
    sprintf(buffer, "Total points: %d", spaceships[0]->points);
    al_draw_text(
                 font, 
                 al_map_rgb(120,120,120), 
                 400, 200, ALLEGRO_ALIGN_RIGHT, 
                 "A winner is you!");   
    al_draw_text(
                 font, 
                 al_map_rgb(120,120,120), 
                 400, 250, ALLEGRO_ALIGN_RIGHT, 
                 buffer);

    al_draw_text(
             font, 
             al_map_rgb(0,255,0), 
             120, 120, ALLEGRO_ALIGN_RIGHT, 
             "very winner");
    al_draw_text(
             font, 
             al_map_rgb(255,0,0), 
             450, 450, ALLEGRO_ALIGN_RIGHT, 
             "much points");
    al_draw_text(
             font, 
             al_map_rgb(0,0,255), 
             600, 120, ALLEGRO_ALIGN_RIGHT, 
             "such asteroids");
    al_flip_display();
    al_rest(2.0);
};

void GameOver() {
    al_clear_to_color(al_map_rgb(0, 0, 0 ));
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform); 
    al_rotate_transform(&transform, 0); 
    al_translate_transform(&transform, 0, 0); 
    al_use_transform(&transform);
    al_draw_text(
                 font, 
                 al_map_rgb(120,120,120), 
                 400, 200, ALLEGRO_ALIGN_RIGHT, 
                 "Game Over!");
    al_flip_display();
    al_rest(2.0);     
}

void CleanupSpaceships() {
    for (int i = 0; i < spaceships.size(); i++) {
        delete spaceships[i];
    } 
    spaceships.clear();
}

void CleanupAsteroids() {
    for (int i = 0; i < asteroids.size(); i++) {
        delete asteroids[i];
    }
    asteroids.clear();
}

void CleanupBullets() {
    for (int i = 0; i < bullets.size(); i++) {
        delete bullets[i];
    }
    bullets.clear();
}

void Cleanup() {
    CleanupSpaceships();
    CleanupAsteroids();
    CleanupBullets();
}