// Core
#include <iostream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <cstdlib>
//Allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
// Self
#include "init.h"
#include "objects.h"
#include "config.h"
#include "engine.h"

using namespace std;

enum MYKEYS {
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ESCAPE, KEY_SPACE
};
enum NAMES {
    OBJECT, SPACESHIP, ASTEROID, SHOOT
};


int main(int argc, char **argv)
{

    srand(static_cast<unsigned int>(time(0)));
    
    bool key[] = { false, false, false, false, false, false };
    bool redraw = true;
    bool doexit = false;
    if (init()) {
        return -1;
    }
    float spaceshipStartSpeed  = 0;
    Spaceship* spaceship = new Spaceship(300,300, spaceshipStartSpeed);
    vector<Spaceship*> spaceships;
    
    spaceships.push_back(spaceship);
    vector<Asteroid*> asteroids;
    vector<Shoot*> shoots;
    CreateAsteoroids(asteroids);

    al_flip_display();
    al_start_timer(timer);

    while(!doexit) {

        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);
 
        if(event.type == ALLEGRO_EVENT_TIMER) {
            
            if (asteroids.size() == 0) {
                     
                doexit = true;
                PrintWinner();
                // immediately terminating the game
                break;
            }
            
            for (int i = 0; i < spaceships.size(); i++) {
                bool lives = spaceship->Update();
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
            // for each asteroid, first check if it was destroied
            // then check if it destroys a spaceship
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
                for (int i = 0; i <= spaceships.size(); i++) {
                    if (lives) {
                        if (IsDestroied(spaceships[i], asteroids[j])) {
                            spaceships[i]-> lives -= 1;
                        }
                    }
                }     
            }
            redraw = true;

            if(key[KEY_UP]) {
                //cout <<"UPPING" << endl;
                spaceship->accelerate();
            }
 
            if(key[KEY_DOWN]) {
                //cout <<"DOWNING" << endl;
                spaceship->decelerate();
            }
 
            if(key[KEY_LEFT]) {
                //cout <<"LEFTING" << endl;
                spaceship->moveLeft();
            }
 
            if(key[KEY_RIGHT]) {
                //cout <<"RIGHTING" << endl;
                spaceship->moveRight();
            }
 
            if(key[KEY_SPACE]) {
                //cout <<"FIRING" << endl;
                Shoot* blast = spaceship->Fire();
                //objects.push_back(blast);
                shoots.push_back(blast);
            }

        }
        // press a key
        else if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch(event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    key[KEY_UP] = true;
                    break;
 
                case ALLEGRO_KEY_DOWN:
                    key[KEY_DOWN] = true;
                    break;
 
                case ALLEGRO_KEY_LEFT: 
                    key[KEY_LEFT] = true;
                    break;
 
                case ALLEGRO_KEY_RIGHT:
                    key[KEY_RIGHT] = true;
                    break;

                case ALLEGRO_KEY_SPACE:
                    key[KEY_SPACE] = true;
                    break;
            }
        }
        // release a key
        else if(event.type == ALLEGRO_EVENT_KEY_UP) {
            switch(event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                    key[KEY_UP] = false;
                    break;
 
                case ALLEGRO_KEY_DOWN:
                    key[KEY_DOWN] = false;
                    break;
 
                case ALLEGRO_KEY_LEFT: 
                    key[KEY_LEFT] = false;
                    break;
 
                case ALLEGRO_KEY_RIGHT:
                    key[KEY_RIGHT] = false;
                    break;

                case ALLEGRO_KEY_SPACE:
                    key[KEY_SPACE] = false;
                    break;
 
                case ALLEGRO_KEY_ESCAPE:
                    doexit = true;
                    break;
            }
        }
        // close display
        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }

        // redraw
        if(redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
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
    } 

    // game was quitted
    al_destroy_display(display);
 /*
    for (int i = 0; i < objects.size(); i++) {
        delete objects[i];
    }
*/
    
    return 0;
}