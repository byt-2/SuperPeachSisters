#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
    Actor(StudentWorld* sw, int imageid, int x, int y, int direction, double size, int depth);
    virtual void doSomething() = 0;
    StudentWorld* getWorld();
    virtual bool blockothers(){return false;}
    virtual bool isBlock(){return false;}
    virtual void bonk() = 0;
    // Do what the spec says happens when damage is inflicted on this actor.
    virtual void sufferDamageIfDamageable();
    virtual bool damageable(){return false;}
    // Mark this actor as dead.
    void setDead();
    // Is this actor dead?
    bool isDead() const;
private:
    StudentWorld* m_world;
    bool m_dead;
};

class Peach : public Actor
{
public:
    Peach(StudentWorld* sw, int x, int y);
    void setHP(int hp)
    {
        m_hp = hp;
    }
    virtual void doSomething();
    virtual void bonk();
    virtual void sufferDamageIfDamageable();
    virtual bool damageable(){return true;} 
    void gainInvincibility();
    // Grant Peach Shoot Power.
    void gainShootPower();
    // Grant Peach Jump Power.
    void gainJumpPower();
    // Is Peach invincible?
    bool isInvincible() const;
    // Does Peach have Shoot Power?
    bool hasShootPower() const;
    // Does Peach have Jump Power?
    bool hasJumpPower() const;
    
private:
    int m_hp;
    int tempinvincible;
    int startime;
    int time_to_charge_before_next_fire;
    bool flower;
    bool mushroom;
    bool star;
    int m_jumpdist;
};

//block and pipe are obstacles
class Obstacle : public Actor
{
public:
    Obstacle(StudentWorld* sw, int imageid, int x, int y, int direction, double size, int depth);
    virtual void doSomething()
    {
    }
    virtual bool blockothers(){return false;}
    virtual bool damageable(){return false;}
    virtual void bonk()
    {
    }
    virtual bool isBlock(){return true;}
};

class Block : public Obstacle
{
public:
    Block(StudentWorld* sw, int x, int y, char goodie);
    virtual bool blockothers(){return true;}
    virtual void bonk();
private:
    char m_goodie;
    bool bonked;
};

class Pipe: public Obstacle
{
public:
    Pipe(StudentWorld* sw, int x, int y);
    virtual bool blockothers(){return true;}
    virtual void bonk();
};

//flag and mario are levelender
class LevelEnder : public Actor
{
public:
    LevelEnder(StudentWorld* sw, int imageid, int x, int y, int direction, double size, int depth);
    virtual void doSomething();
    virtual bool damageable(){return false;}
    virtual void bonk()
    {
    }
    virtual void advance() = 0;
};

class Flag : public LevelEnder
{
public:
    Flag(StudentWorld* sw, int x, int y);
    virtual void advance();
};

class Mario : public LevelEnder
{
public:
    Mario(StudentWorld* sw, int x, int y);
    virtual void advance();
};

//flower, mushroom, star are goodies
class Goodies : public Actor
{
public:
    Goodies(StudentWorld* sw, int imageid, int x, int y, int direction, double size, int depth);
    virtual void doSomething();
    virtual bool blockothers(){return false;}
    virtual void addScore() = 0;
    virtual void powerUp() = 0;
    virtual bool damageable(){return false;}
    virtual void bonk()
    {
    }
};

class Flower : public Goodies
{
public:
    Flower(StudentWorld* sw, int x, int y);
    virtual void addScore()
    {
        getWorld() -> increaseScore(50);
    }
    virtual void powerUp()
    {
        getWorld() -> getPeach() -> gainShootPower();
    }
};

class Mushroom : public Goodies
{
public:
    Mushroom(StudentWorld* sw, int x, int y);
    virtual void addScore()
    {
        getWorld() -> increaseScore(75);
    }
    virtual void powerUp()
    {
        getWorld() -> getPeach() -> gainJumpPower();
    }
};

class Star : public Goodies
{
public:
    Star(StudentWorld* sw, int x, int y);
    virtual void addScore()
    {
        getWorld() -> increaseScore(100);
    }
    virtual void powerUp()
    {
        getWorld() -> getPeach() -> gainInvincibility();
    }
};

//piranhafireball, peachfireball, and shell are projectiles
class Projectile : public Actor
{
public:
    Projectile(StudentWorld* sw, int imageID, int x, int y, int direction, double size, int depth);
    virtual void doSomething();
    virtual bool damageable(){return false;}
    virtual bool blockothers(){return true;}
    virtual void bonk()
    {
    }
    virtual void shoot() = 0;
private:
    int dir;
};

class PiranhaFireball : public Projectile
{
public:
    PiranhaFireball(StudentWorld* sw, int x, int y, int direction);
    virtual void shoot()
    {
        if (getWorld() -> overlapPeach(this))
        {
            getWorld() -> getPeach() -> sufferDamageIfDamageable();
        }
    }
};

class PeachFireball : public Projectile
{
public:
    PeachFireball(StudentWorld* sw, int x, int y, int direction);
    virtual void shoot()
    {
        if (getWorld() -> overlapMonsters(this))
        {
            getWorld() -> damageMonsters(this);
        }
    }
};

class Shell : public Projectile
{
public:
    Shell(StudentWorld* sw, int x, int y, int dir);
    virtual void shoot()
    {
        if (getWorld() -> overlapMonsters(this))
        {
            getWorld() -> damageMonsters(this);
        }
    }
};

//goomba, koopa, piranha are enemies
class Enemy: public Actor
{
public:
    Enemy(StudentWorld* sw, int imageid, int x, int y, int direction, double size, int depth);
    virtual void doSomething();
    virtual bool blockothers(){return true;}
    virtual bool damageable(){return true;}
    virtual void sufferDamageIfDamageable();
    virtual void bonk()
    {
    }
    virtual void deadKoopa(){}
};

class Goomba: public Enemy
{
public:
    Goomba(StudentWorld* sw, int x, int y, int direction);
};

class Koopa: public Enemy
{
public:
    Koopa(StudentWorld* sw, int x, int y, int direction);
    void deadKoopa();
};

class Piranha: public Enemy
{
public:
    Piranha(StudentWorld* sw, int x, int y, int direction);
    virtual void doSomething();
private:
    int firedelay;
};

//class
#endif // ACTOR_H_
