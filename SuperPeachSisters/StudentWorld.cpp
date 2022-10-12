#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), nextLevel(false), win(false)
{
}

int StudentWorld::init()
{
    Level lev(assetPath());
    ostringstream oss;
    oss.fill('0');
    int levelnum = getLevel();
    oss << "level" << setw(2) << levelnum << ".txt";
    string level = oss.str();
    Level::LoadResult result = lev.loadLevel(level);
    if (result == Level::load_fail_file_not_found)
     cerr << "Could not find level01.txt data file" << endl;
     else if (result == Level::load_fail_bad_format)
     cerr << "level01.txt is improperly formatted" << endl;
     else if (result == Level::load_success)
     {
     cerr << "Successfully loaded level" << endl;
         for (int i = 0; i < GRID_WIDTH; i++)
         {
             for (int k = 0; k < GRID_HEIGHT; k++)
             {
                 Level::GridEntry ge;
                 ge = lev.getContentsOf(i, k); // x=5, y=10
                 switch (ge)
                 {
                     case Level::empty:
                      cout << "Location 5,10 is empty" << endl;
                      break;
                      case Level::koopa:
                      cout << "Location 5,10 starts with a koopa" << endl;
                         m_actor.push_back(new Koopa(this, i*SPRITE_WIDTH, k*SPRITE_HEIGHT, randomDirection() ));
                      break;
                      case Level::goomba:
                      cout << "Location 5,10 starts with a goomba" << endl;
                         m_actor.push_back(new Goomba(this, i*SPRITE_WIDTH, k*SPRITE_HEIGHT, randomDirection() ));
                      break;
                      case Level::peach:
                      cout << "Location 5,10 is where Peach starts" << endl;
                         m_peach = new Peach(this, i*SPRITE_WIDTH, k*SPRITE_HEIGHT);
                      break;
                      case Level::flag:
                      cout << "Location 5,10 is where a flag is" << endl;
                        m_actor.push_back(new Flag(this, i*SPRITE_WIDTH, k*SPRITE_HEIGHT));
                      break;
                      case Level::block:
                      cout << "Location 5,10 holds a regular block" << endl;
                         m_actor.push_back(new Block(this, i*SPRITE_WIDTH, k*SPRITE_HEIGHT, 'b'));
                      break;
                      case Level::star_goodie_block:
                         cout << "Location 5,10 has a star goodie block" << endl;
                         m_actor.push_back(new Block(this, i*SPRITE_WIDTH, k*SPRITE_HEIGHT, 's'));
                      break;
                     case Level::piranha:
                     cout << "Location 5,10 starts with a piranha" << endl;
                        m_actor.push_back(new Piranha(this, i*SPRITE_WIDTH, k*SPRITE_HEIGHT, randomDirection()));
                     break;
                     case Level::mushroom_goodie_block:
                     cout << "Location 5,10 has a mushroom goodie block" << endl;
                         m_actor.push_back(new Block(this, i*SPRITE_WIDTH, k*SPRITE_HEIGHT, 'm'));
                     break;
                     case Level::flower_goodie_block:
                     cout << "Location 5,10 has a flower goodie block" << endl;
                         m_actor.push_back(new Block(this, i*SPRITE_WIDTH, k*SPRITE_HEIGHT, 'f'));
                     break;
                     case Level::pipe:
                     cout << "Location 5,10 has a piep" << endl;
                        m_actor.push_back(new Pipe(this, i*SPRITE_WIDTH, k*SPRITE_HEIGHT));
                     break;
                     case Level::mario:
                     cout << "Location 5,10 is where mario is" << endl;
                       m_actor.push_back(new Mario(this, i*SPRITE_WIDTH, k*SPRITE_HEIGHT));
                     break;
                }
             }
         }
     }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    if (!m_peach -> isDead())
    {
    m_peach -> doSomething();
    }
    if (m_peach -> isDead())
    {
        playSound(SOUND_PLAYER_DIE);
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    for (int i = 0; i < m_actor.size(); i++)
    {
        m_actor[i] -> doSomething();
    }
    
    vector<Actor*>:: iterator it;   //delete the dead actors
    for (it = m_actor.begin(); it != m_actor.end(); )
    {
        if ((*it) -> isDead())
        {
        delete *it;
        it = m_actor.erase(it);
        }
        else
        {
            it++;
        }
    }
    
    if (nextLevel)
    {
        playSound(SOUND_FINISHED_LEVEL);
        nextLevel = false;
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    if (win)
    {
        playSound(SOUND_GAME_OVER);
        return GWSTATUS_PLAYER_WON;
    }
    
    ostringstream os;
    os << "Lives: " << getLives() << "  Level: " << getLevel() << "  Points: " << getScore();
    if (m_peach -> isInvincible())
        os << " StarPower!";
    if (m_peach -> hasShootPower())
        os << " ShootPower!";
    if (m_peach -> hasJumpPower())
        os << " JumpPower!";
    string gameText = os.str();
    setGameStatText(gameText);
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_peach;
    vector<Actor*>:: iterator it;
    for (it = m_actor.begin(); it != m_actor.end(); )
    {
        delete *it;
        it = m_actor.erase(it);
    }
}

bool StudentWorld::overlap(int a1, int b1, int a2, int b2)
{
    if ((a1-a2) > SPRITE_WIDTH - 1 || (a1-a2) < 1 - SPRITE_WIDTH || (b1-b2) > SPRITE_HEIGHT - 1 || (b1-b2) < 1 - SPRITE_HEIGHT)
        return false;
    return true;
}

bool StudentWorld::block(int x, int y)
{
    for (int i = 0; i < m_actor.size(); i++)
    {
        if(overlap(x, y, m_actor[i]->getX(), m_actor[i]->getY()) && m_actor[i]-> isBlock())
           return true;
    }
           return false;
}

bool StudentWorld::overlapMonsters(Actor* c)
{
    for (int i = 0; i < m_actor.size(); i++)
    {
        if(overlap(c -> getX(), c -> getY(), m_actor[i]->getX(), m_actor[i]->getY()) && m_actor[i]-> damageable() && !m_actor[i] -> isDead())
           return true;
    }
           return false;
}

int StudentWorld::randomDirection()
{
    if (randInt(0,180) < 90)
        return 0;
    else
        return 180;
}

void StudentWorld::insertGoodies(char goodie, int x, int y)
{
    if (goodie == 'f')
    {
        m_actor.push_back(new Flower(this, x, y));
    }
    else if (goodie == 'm')
    {
        m_actor.push_back(new Mushroom(this, x, y));
    }
    else if (goodie == 's')
    {
        m_actor.push_back(new Star(this, x, y));
    }
}

void StudentWorld::insertProjectile(int projectile, int x, int y, int direction)
{
    if (projectile == 1)
    {
        m_actor.push_back(new PeachFireball(this, x, y, direction));
    }
    else if (projectile == 2)
    {
        m_actor.push_back(new PiranhaFireball(this, x, y, direction));
    }
    else if (projectile == 3)
    {
        m_actor.push_back(new Shell(this, x, y, direction));
    }
}

void StudentWorld::changeLevel()
{
    nextLevel = true;
}

void StudentWorld::endgame()
{
    win = true;
}

bool StudentWorld::fallOff(int x, int y)
{
    for(int i = 0; i < m_actor.size(); i++)
    {
        if (m_actor[i] -> blockothers())
        {
            if (m_actor[i] -> getX() <= x && m_actor[i]-> getX() + SPRITE_WIDTH - 1 >= x && m_actor[i]-> getY() <= y && m_actor[i]-> getY() + SPRITE_HEIGHT - 1 >= y)
            {
                return false;
            }
        }
    }
    return true;
}

bool StudentWorld::overlapPeach(Actor *a)
{
return overlap( a -> getX(), a -> getY(), m_peach -> getX(),m_peach -> getY());
}

Peach* StudentWorld::getPeach()
{
    return m_peach;
}

void StudentWorld::peachBonkothers(int x, int y)
{
    for (int i = 0; i < m_actor.size(); i++)
    {
        if (overlap(x, y, m_actor[i]->getX(), m_actor[i] -> getY()))
            m_actor[i] -> bonk();
    }
}

void StudentWorld::damageMonsters(Actor *b)
{
    for (int i = 0; i < m_actor.size(); i++)
    {
        if (overlap(b -> getX(), b -> getY(), m_actor[i]->getX(), m_actor[i] -> getY()) && !m_actor[i] -> isDead())
            m_actor[i] -> sufferDamageIfDamageable();
    }
}
