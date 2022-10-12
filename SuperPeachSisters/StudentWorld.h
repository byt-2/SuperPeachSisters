#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>
using namespace std;
class Actor;
class Peach;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();
  virtual ~StudentWorld()
    {
        cleanUp();
    }
    bool overlap(int a1, int a2, int b1, int b2);
    bool block(int x, int y);
    bool overlapMonsters(Actor* c);
    int randomDirection(); //needs randomDirection for koopa, goompa and piranha
    void insertGoodies(char goodie, int x, int y); //add goodies to blocks
    void insertProjectile(int projectile, int x, int y, int direction); //add projectiles
    void changeLevel();
    void endgame();
    bool fallOff(int x, int y);
    bool overlapPeach(Actor* a);
    Peach* getPeach();
    void peachBonkothers(int x, int y);
    void damageMonsters(Actor* b);
private:
    Peach* m_peach;
    vector<Actor*> m_actor;
    bool nextLevel;
    bool win;
};
#endif // STUDENTWORLD_H_
