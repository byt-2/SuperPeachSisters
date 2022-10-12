#include "Actor.h"
#include "StudentWorld.h"
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
/// ACTOR IMPLEMENTATION
Actor::Actor(StudentWorld* sw, int imageid, int x, int y, int direction = 0, double size = 1.0, int depth = 0)
 : GraphObject(imageid, x, y), m_world(sw)
{
    m_dead = false;
}

StudentWorld* Actor::getWorld()
{
    return m_world;
}

void Actor::sufferDamageIfDamageable()
{
}

void Actor::setDead()
{
    m_dead = true;
}

bool Actor::isDead() const
{
    return m_dead;
}
/// PEACH IMPLEMENTATION
Peach::Peach(StudentWorld* sw, int x, int y)
    : Actor(sw, IID_PEACH, x, y, 0, 1.0, 0), m_hp(1), tempinvincible(0), startime(0), flower(false), mushroom(false), star(false)
{
}

void Peach::bonk()
{
    getWorld() -> playSound(SOUND_PLAYER_BONK);
}

void Peach::sufferDamageIfDamageable()
{
    if (isInvincible() || tempinvincible > 0)
    {
        return;
    }
    else
    {
        m_hp--;
        tempinvincible = 10;
        if (hasShootPower())
        {
            flower = false;
        }
        if (hasJumpPower())
        {
            mushroom = false;
        }
        if (m_hp > 0)
        {
            getWorld() -> playSound(SOUND_PLAYER_HURT);
        }
        if (m_hp == 0)
        {
            setDead();
        }
    }
}

void Peach::doSomething()
{
    if (isDead())
    {
        return;
    }
    if (tempinvincible > 0)
    {
        tempinvincible--;
    }
    if (startime > 0)
    {
        startime--;
        if (startime == 0)
        {
            star = false;
        }
    }
   
    if (time_to_charge_before_next_fire > 0)
    {
        time_to_charge_before_next_fire--;
    }
    if (m_jumpdist > 0)
    {
        if (getWorld() -> block(getX(), getY() + 4))
        {
            getWorld() -> peachBonkothers(getX(), getY()+4);
            m_jumpdist = 0;
        }
        else
        {
            moveTo(getX(), getY() + 4);
            m_jumpdist--;
        }
    }
    if (m_jumpdist == 0 && !getWorld() -> block(getX(), getY() - 4))
    {
        moveTo(getX(), getY() - 4);
    }
    int key;
    if (getWorld()->getKey(key))
    {
        switch (key)
        {
          case KEY_PRESS_LEFT:
                setDirection(180);
               if (!getWorld() -> block(getX() - 4, getY()))
                moveTo(getX() - 4, getY());
                else
                    bonk();
                break;
            case KEY_PRESS_RIGHT:
                setDirection(0);
               if (!getWorld() -> block(getX() + 4, getY()))
                moveTo(getX() + 4, getY());
                else
                    bonk();
                break;
            case KEY_PRESS_UP:
                if (getWorld() -> block(getX(), getY()-1))
                {
                if (hasJumpPower())
                {
                    m_jumpdist = 12;
                }
                else
                {
                    m_jumpdist = 8;
                }
                getWorld() -> playSound(SOUND_PLAYER_JUMP);
                }
              break;
            case KEY_PRESS_SPACE:
                if (hasShootPower() && time_to_charge_before_next_fire == 0)
                {
                    getWorld() -> playSound(SOUND_PLAYER_FIRE);
                    time_to_charge_before_next_fire = 8;
                if(getDirection()==0)
                {
                    getWorld()->insertProjectile(1, getX()+4, getY(), 0);
                }
                else if(getDirection()==180)
                {
                    getWorld()->insertProjectile(1, getX()-12, getY(), 180);
                }
                }
            default:
                break;
        }
    }
}

void Peach::gainInvincibility()
{
    star = true;
    startime = 150;
}

void Peach::gainShootPower()
{
    flower = true;
}

void Peach::gainJumpPower()
{
    mushroom = true;
}

bool Peach::isInvincible() const
{
    return star;
}

bool Peach::hasShootPower() const
{
    return flower;
}

bool Peach::hasJumpPower() const
{
    return mushroom;
}

/// OBSTACLE IMPLEMENTATION
Obstacle::Obstacle(StudentWorld* sw, int imageid, int x, int y, int direction, double size, int depth)
    :Actor(sw, imageid, x, y, direction, size, depth)
{
}

/// BLOCK IMPLEMENTATION
Block::Block(StudentWorld* sw, int x, int y, char goodie)
    :Obstacle(sw, IID_BLOCK, x, y, 0, 1, 2), m_goodie(goodie), bonked(false)
{
}


void Block::bonk()
{
    if (m_goodie != 'b' && !bonked)
    {
        getWorld() -> playSound(SOUND_POWERUP_APPEARS);
        if (m_goodie == 'f')
        {
            getWorld() -> insertGoodies('f', getX(), getY()+8);
        }
        else if (m_goodie == 'm')
        {
            getWorld() -> insertGoodies('m', getX(), getY()+8);
        }
        else if (m_goodie == 's')
        {
            getWorld() -> insertGoodies('s', getX(), getY()+8);
        }
    }
    else
    {
        getWorld() -> playSound(SOUND_PLAYER_BONK);
    }
    bonked = true;
}

/// PIPE IMPLEMENTATION
Pipe::Pipe(StudentWorld* sw, int x, int y)
    : Obstacle(sw, IID_PIPE, x, y, 0, 1.0, 2)
{
}

void Pipe::bonk()
{
    getWorld() -> playSound(SOUND_PLAYER_BONK);
}

/// LEVELENDER IMPLEMENTATION
LevelEnder::LevelEnder(StudentWorld* sw, int imageid, int x, int y, int direction, double size, int depth)
    :Actor(sw, imageid, x, y, direction, size, depth)
{
}

void LevelEnder::doSomething()
{
    if (isDead())
        return;
    if (getWorld() -> overlapPeach(this))
    {
        getWorld() -> increaseScore(1000);
        setDead();
        advance();
    }
}

/// FLAG IMPLEMENTATION
Flag::Flag(StudentWorld* sw, int x, int y)
    :LevelEnder(sw, IID_FLAG, x, y, 0, 1, 1)
{
}

void Flag::advance()
{
    getWorld() -> changeLevel();
}

/// MARIO IMPLEMENTATION
Mario::Mario(StudentWorld* sw, int x, int y)
    :LevelEnder(sw, IID_MARIO, x, y, 0, 1, 1)
{
}

void Mario::advance()
{
    getWorld() -> endgame();
}

/// GOODIES IMPLEMENTATION
Goodies::Goodies(StudentWorld* sw, int imageid, int x, int y, int direction, double size, int depth)
    :Actor(sw, imageid, x, y, direction, size, depth)
{
}

void Goodies::doSomething()
{
    if (isDead())
        return;
    if (getWorld() -> overlapPeach(this))
    {
        addScore();
        powerUp();
        setDead();
        getWorld() -> playSound(SOUND_PLAYER_POWERUP);
        return;
    }
    if (!getWorld() -> block(getX(), getY()-1))
    {
        moveTo(getX(), getY()-2);
    }
    if (getDirection() == 180)
    {
        if (getWorld() -> block(getX() - 2, getY()))
        {
            setDirection(0);
            return;
        }
        moveTo(getX()-2, getY());
    }
    else if (getDirection() == 0)
    {
        if (getWorld() -> block(getX() + 2, getY()))
        {
            setDirection(180);
            return;
        }
        moveTo(getX()+2, getY());
    }
}

/// FLOWER IMPLEMENTATION
Flower::Flower(StudentWorld* sw, int x, int y)
    : Goodies(sw, IID_FLOWER, x, y, 0, 1.0, 1)
{
}

/// MUSHROOM IMPLEMENTATION
Mushroom::Mushroom(StudentWorld* sw, int x, int y)
    : Goodies(sw, IID_MUSHROOM, x, y, 0, 1.0, 1)
{
}

/// STAR IMPLEMENTATION
Star::Star(StudentWorld* sw, int x, int y)
    : Goodies(sw, IID_STAR, x, y, 0, 1.0, 1)
{
}

/// PROJECTILE IMPLEMENTATION
Projectile::Projectile(StudentWorld* sw, int imageid, int x, int y, int direction, double size, int depth)
    :Actor(sw, imageid, x, y, direction, size, depth), dir(direction)
{
}

void Projectile::doSomething()
{
    if (isDead())
        return;
    if (!getWorld() -> block(getX(), getY()-1))
    {
        moveTo(getX(), getY()-2);
    }
    if (dir == 180)
    {
        if (getWorld() -> block(getX() -2, getY()))
        {
            setDead();
            return;
        }
        moveTo(getX() -2, getY());
    }
    else if (dir == 0)
    {
        if (getWorld() -> block(getX() +2, getY()))
        {
            setDead();
            return;
        }
        moveTo(getX() +2, getY());
    }
    shoot();
}

/// PIRANHAFIREBALL IMPLEMENTATION
PiranhaFireball::PiranhaFireball(StudentWorld* sw, int x, int y, int direction)
    :Projectile(sw, IID_PIRANHA_FIRE, x, y, direction, 1.0, 1)
{
}

/// PEACHFIREBALL IMPLEMENTATION
PeachFireball::PeachFireball(StudentWorld* sw, int x, int y, int direction)
    :Projectile(sw, IID_PEACH_FIRE, x, y, direction, 1.0, 1)
{
}

/// SHELL IMPLEMENTATION
Shell::Shell(StudentWorld* sw, int x, int y, int direction)
    :Projectile(sw, IID_SHELL, x, y, direction, 1.0, 1)
{
}
/// Enemy IMPLEMENTATION
Enemy::Enemy(StudentWorld* sw, int imageid, int x, int y, int direction, double size, int depth)
    :Actor(sw, imageid, x, y, direction, size, depth)
{
}

void Enemy::doSomething()
{
if (isDead())
{
    return;
}
    
if (getDirection() == 0)
    {
        if (getWorld() -> block(getX()+1, getY()))
        {
            setDirection(180);
        }
        else if (getWorld() -> fallOff(getX() + SPRITE_WIDTH, getY() -1))
        {
            setDirection(180);
        }
    }
else if (getDirection() == 180)
    {
        if (getWorld() -> block(getX()-1, getY()))
        {
            setDirection(0);
        }
        else if (getWorld() -> fallOff(getX() -1, getY() -1))
        {
            setDirection(0);
        }
    }
if (getDirection() == 0)
{
    if (getWorld() -> block(getX()+1, getY()))
    {
        return;
    }
    else
    {
        moveTo(getX()+1, getY());
    }
}
if (getDirection() == 180)
    {
        if (getWorld() -> block(getX()-1, getY()))
        {
            return;
        }
        else
        {
            moveTo(getX()-1, getY());
        }
    }
if (getWorld() -> overlapPeach(this))
{
    getWorld() -> playSound(SOUND_PLAYER_KICK);
    getWorld() -> getPeach() -> sufferDamageIfDamageable();
}
}

void Enemy::sufferDamageIfDamageable()
{
    getWorld() -> increaseScore(100);
    setDead();
    deadKoopa();
}

/// GOOMPA IMPLEMENTATION
Goomba::Goomba(StudentWorld* sw, int x, int y, int direction)
    :Enemy(sw, IID_GOOMBA, x, y, direction, 1.0, 0)
{
}

/// KOOPA IMPLEMENTATION
Koopa::Koopa(StudentWorld* sw, int x, int y, int direction)
    :Enemy(sw, IID_KOOPA, x, y, direction, 1.0, 0)
{
}

void Koopa::deadKoopa()
{
        getWorld() -> insertProjectile(3, getX(), getY(), getDirection());
}

/// PIRANHA IMPLEMENTATION
Piranha::Piranha(StudentWorld* sw, int x, int y, int direction)
    :Enemy(sw, IID_PIRANHA, x, y, direction, 1.0, 0)
{
}

void Piranha::doSomething()
{
    if (isDead())
        return;
    increaseAnimationNumber();
    if (getWorld() -> overlapPeach(this))
    {
        getWorld() -> getPeach() -> sufferDamageIfDamageable();
        return;
    }
    if ((getY() - getWorld() -> getPeach() -> getY()) > 1.5 * SPRITE_HEIGHT || (getY() - getWorld() -> getPeach() -> getY()) <  -1.5 * SPRITE_HEIGHT)
        return;
    if (getX() - getWorld() -> getPeach() -> getX() < 0)
    {
        setDirection(0);
    }
    else if (getX() - getWorld() -> getPeach() -> getX() > 0)
    {
        setDirection(180);
    }
    if (firedelay > 0)
    {
        firedelay--;
    }
    if (firedelay == 0)
    {
        if ((getX() - getWorld() -> getPeach() -> getX() )> -8*SPRITE_WIDTH && (getX() - getWorld() -> getPeach() -> getX()) < 8*SPRITE_WIDTH)
        {
            getWorld() -> insertProjectile(2, getX(), getY(), getDirection());
            getWorld() -> playSound(SOUND_PIRANHA_FIRE);
            firedelay = 40;
        }
    }
}
