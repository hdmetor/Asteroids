#include "engine.h"
#include "config.h"
#include "objects.h"
#include <vector>
#include <iostream>
using namespace std;

void CreateAsteoroids(vector<Asteroid*>& asteroids) {
    for (int i = 0; i < asteoridsNumber; i++) {
        cout << i << endl;
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