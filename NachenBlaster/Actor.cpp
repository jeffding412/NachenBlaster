#include "Actor.h"
#include "StudentWorld.h"
#include <cstdlib>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

///////////////////////////////////////////////////
////////////////////// Actor //////////////////////
///////////////////////////////////////////////////
Actor::Actor(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* world)
:GraphObject(imageID, startX, startY, dir, size, depth)
{
    m_id = imageID;
    m_world = world;
    m_Status = true;
}

Actor::~Actor()
{}

StudentWorld* Actor::getWorld() const
{
    return m_world;
}

bool Actor::isAlive() const
{
    return m_Status;
}

int Actor::getImageID() const
{
    return m_id;
}

void Actor::setDead()
{
    m_Status = false;
}

///////////////////////////////////////////////////
////////////////////// Star ///////////////////////
///////////////////////////////////////////////////
Star::Star(double startX, double startY, StudentWorld* world)
:Actor(IID_STAR, startX, startY, 0, 0.01*randInt(5, 50), 3, world)
{}

Star::~Star()
{}

void Star::move()
{
    moveTo(getX()-1, getY());
    if (getX() <= -1) {
        setDead();
    }
}

void Star::doSomething()
{
    move();
}

///////////////////////////////////////////////////
////////////////////// Ship ///////////////////////
///////////////////////////////////////////////////
Ship::Ship(int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* world, int startHealth)
:Actor(imageID, startX, startY, dir, size, depth, world)
{
    m_Health = startHealth;
}

Ship::~Ship()
{}

int Ship::getHealth() const
{
    return m_Health;
}

void Ship::decreaseHealth(int amount)
{
    m_Health -= amount;
}

void Ship::increaseHealth(int amount)
{
    if (m_Health + amount > 50) {
        m_Health = 50;
    }
    else {
        m_Health += amount;
    }
}

///////////////////////////////////////////////////
///////////// NachenBlaster ///////////////////////
///////////////////////////////////////////////////
NachenBlaster::NachenBlaster(StudentWorld* world)
:Ship(IID_NACHENBLASTER, 0, 128, 0, 1.0, 0, world, 50)
{
    m_CabbagePoints = 30;
    m_torpedoes = 0;
}

NachenBlaster::~NachenBlaster()
{}

void NachenBlaster::doSomething()
{
    if (isAlive()) {
        if (getWorld()->getKey(m_keyValue)) {
            move();
        }
        int collisionTarget = getWorld()->checkCollision(this);
        if (collisionTarget != -1) {
            sufferDamage(collisionTarget);
            getWorld()->playSound(SOUND_BLAST);
        }
        if (getCabbage() < 30) {
            m_CabbagePoints++;
        }
    }
}

void NachenBlaster::move()
{
    switch (m_keyValue) {
        case KEY_PRESS_UP:
            if (getY()+6 < VIEW_HEIGHT) {
                moveTo(getX(), getY()+6);
            }
            break;
        case KEY_PRESS_DOWN:
            if (getY()-6 >= 0) {
                moveTo(getX(), getY()-6);
            }
            break;
        case KEY_PRESS_LEFT:
            if (getX()-6 >= 0) {
                moveTo(getX()-6, getY());
            }
            break;
        case KEY_PRESS_RIGHT:
            if (getX()+6 < VIEW_WIDTH) {
                moveTo(getX()+6, getY());
            }
            break;
        case KEY_PRESS_SPACE:
            if (getCabbage() >= 5) {
                m_CabbagePoints -= 5;
                getWorld()->addObject(IID_CABBAGE, getX(), getY());
            }
            break;
        case KEY_PRESS_TAB:
            if (getTorpedoes() != 0) {
                m_torpedoes--;
                getWorld()->addObject(IID_TORPEDO, getX(), getY());
            }
            break;
        default:
            break;
    }
}

void NachenBlaster::sufferDamage(int collidedID)
{
    switch (collidedID) {
        case IID_TURNIP:
            decreaseHealth(2);
            break;
        case IID_TORPEDO:
            decreaseHealth(8);
            break;
        case IID_SMALLGON:
        case IID_SMOREGON:
            decreaseHealth(5);
            break;
        case IID_SNAGGLEGON:
            decreaseHealth(15);
            break;
        case IID_LIFE_GOODIE:
        case IID_REPAIR_GOODIE:
        case IID_TORPEDO_GOODIE:
            goodiePickedUp(collidedID);
            break;
        default:
            break;
    }
    if (getHealth() <= 0) {
        setDead();
    }
}

int NachenBlaster::getTorpedoes() const
{
    return m_torpedoes;
}

int NachenBlaster::getCabbage() const
{
    return m_CabbagePoints;
}

void NachenBlaster::addTorpedoes()
{
    m_torpedoes += 5;
}

void NachenBlaster::goodiePickedUp(int collidedID)
{
    getWorld()->increaseScore(100);
    getWorld()->playSound(SOUND_GOODIE);
    switch (collidedID) {
        case IID_LIFE_GOODIE:
            getWorld()->incLives();
            break;
        case IID_REPAIR_GOODIE:
            increaseHealth(10);
            break;
        case IID_TORPEDO_GOODIE:
            addTorpedoes();
            break;
        default:
            break;
    }
}

///////////////////////////////////////////////////
//////////////// Projectile ///////////////////////
///////////////////////////////////////////////////
Projectile::Projectile(int imageID, double startX, double startY, int dir, StudentWorld* world)
:Actor(imageID, startX, startY, dir, 0.5, 1, world)
{}

Projectile::~Projectile()
{}

void Projectile::doSomething()
{
    if (isAlive()) {
        if (isOffScreen()) {
            setDead();
        }
        move();
    }
}

bool Projectile::isOffScreen() const
{
    if (getX() >= VIEW_WIDTH || getX() < 0) {
        //for some reason, the 2nd thign was getY()
        return true;
    }
    return false;
}

///////////////////////////////////////////////////
/////////////////// Cabbage ///////////////////////
///////////////////////////////////////////////////
Cabbage::Cabbage(double startX, double startY, StudentWorld* world)
:Projectile(IID_CABBAGE, startX, startY, 0, world)
{}

Cabbage::~Cabbage()
{}

void Cabbage::move()
{
    moveTo(getX()+8, getY());
    setDirection(getDirection()+20);
}

///////////////////////////////////////////////////
/////////////////// Torpedo ///////////////////////
///////////////////////////////////////////////////
Torpedo::Torpedo(double startX, double startY, int dir, StudentWorld* world)
:Projectile(IID_TORPEDO, startX, startY, dir, world)
{}

Torpedo::~Torpedo()
{}

void Torpedo::move()
{
    if (getDirection() == 0) {
        moveTo(getX()+8, getY());
    }
    else {
        moveTo(getX()-8, getY());
    }
}

///////////////////////////////////////////////////
//////////////////// Turnip ///////////////////////
///////////////////////////////////////////////////
Turnip::Turnip(double startX, double startY, StudentWorld* world)
:Projectile(IID_TURNIP, startX, startY, 0, world)
{}

Turnip::~Turnip()
{}

void Turnip::move()
{
    moveTo(getX()-6, getY());
    setDirection(getDirection()+20);
}

///////////////////////////////////////////////////
///////////////////// Alien ///////////////////////
///////////////////////////////////////////////////
Alien::Alien(int imageID, double startX, double startY, StudentWorld* world, int startHealth)
:Ship(imageID, startX, startY, 0, 1.5, 1, world, startHealth)
{
    m_travel_speed = 2.0;
    m_flight_plan_length = 0;
    m_direction = 0; //0 is going straight, 1 is going up, 2 is going down
}

Alien::~Alien()
{}

void Alien::doSomething()
{
    if (isAlive()) {
        if (getX() < 0) {
            setDead();
            getWorld()->subtractAlien();
        }
        int collisionTarget = getWorld()->checkCollision(this);
        if (collisionTarget != -1) {
            sufferDamage(collisionTarget);
            if (collisionTarget == IID_NACHENBLASTER) {
                getWorld()->getPlayer()->sufferDamage(getImageID());
            }
            if (!isAlive()) {
                deathSequence();
                getWorld()->killAlien();
            }
            else {
                getWorld()->playSound(SOUND_BLAST);
            }
        }
        if (needNewFlightPlan()) {
            if (getY() >= VIEW_HEIGHT-1) {
                setDirection(2);
            }
            else if (getY() <= 0) {
                setDirection(1);
            }
            else if (getFlightPlanLength() == 0) {
                switch (randInt(0, 2)) {
                    case 0:
                        setDirection(0);
                        break;
                    case 1:
                        setDirection(1);
                        break;
                    case 2:
                        setDirection(2);
                        break;
                    default:
                        break;
                }
            }
            setFlightPlanLength(randInt(1, 32));
        }
        if (withinRangeNB()) {
            if (!withinRangeAction())
                move();
        }
        else {
            move();
        }
    }
}

void Alien::move()
{
    switch (getDirection()) {
        case 0:
            moveTo(getX()-getTravelSpeed(), getY());
            break;
        case 1:
            moveTo(getX()-getTravelSpeed(), getY()+getTravelSpeed());
            break;
        case 2:
            moveTo(getX()-getTravelSpeed(), getY()-getTravelSpeed());
            break;
        default:
            break;
    }
    m_flight_plan_length--;
}

void Alien::sufferDamage(int collidedID)
{
    switch (collidedID) {
        case IID_CABBAGE:
            decreaseHealth(2);
            break;
        case IID_TORPEDO:
            decreaseHealth(8);
            break;
        case IID_NACHENBLASTER:
            decreaseHealth(100);
            break;
        default:
            break;
    }
    if (getHealth() <= 0) {
        setDead();
    }
}

double Alien::getTravelSpeed() const
{
    return m_travel_speed;
}

int Alien::getFlightPlanLength() const
{
    return m_flight_plan_length;
}

int Alien::getDirection() const
{
    return m_direction;
}

bool Alien::needNewFlightPlan() const
{
    return (getFlightPlanLength() == 0 || getY() >= VIEW_HEIGHT-1 || getY() <= 0);
}

void Alien::setFlightPlanLength(int length)
{
    m_flight_plan_length = length;
}

void Alien::setTravelSpeed(double speed)
{
    m_travel_speed = speed;
}

void Alien::setDirection(int direction)
{
    m_direction = direction;
}

bool Alien::withinRangeNB() const
{
    double NBYCoord = getWorld()->getPlayer()->getY();
    return (getWorld()->getPlayer()->getX() < getX()) && (abs(NBYCoord - getY()) <= 4);
}

///////////////////////////////////////////////////
////////////////// Smallgon ///////////////////////
///////////////////////////////////////////////////
Smallgon::Smallgon(double startX, double startY, StudentWorld* world)
:Alien(IID_SMALLGON, startX, startY, world, 5 * (1 + (world->getLevel() - 1) * 0.1))
{}

Smallgon::~Smallgon()
{}

bool Smallgon::withinRangeAction()
{
    int oneInHoweverMany = (20/(getWorld()->getLevel())) + 5;
    if (randInt(1, oneInHoweverMany) == 1) {
        getWorld()->addObject(IID_TURNIP, getX(), getY());
        return true;
    }
    return false;
}

void Smallgon::deathSequence()
{
    getWorld()->increaseScore(250);
    getWorld()->playSound(SOUND_DEATH);
    getWorld()->addObject(IID_EXPLOSION, getX(), getY());
}

///////////////////////////////////////////////////
////////////////// Smoregon ///////////////////////
///////////////////////////////////////////////////
Smoregon::Smoregon(double startX, double startY, StudentWorld* world)
:Alien(IID_SMOREGON, startX, startY, world, 5 * (1 + (world->getLevel() - 1) * 0.1))
{}

Smoregon::~Smoregon()
{}

bool Smoregon::withinRangeAction()
{
    int oneInHoweverMany = (20/(getWorld()->getLevel())) + 5;
    int actionNumber = randInt(1, oneInHoweverMany);
    if (actionNumber == 1) {
        getWorld()->addObject(IID_TURNIP, getX(), getY());
        return true;
    }
    else if (actionNumber == 2) {
        setDirection(0);
        setFlightPlanLength(VIEW_WIDTH);
        setTravelSpeed(5);
    }
    return false;
}

void Smoregon::deathSequence()
{
    getWorld()->increaseScore(250);
    getWorld()->playSound(SOUND_DEATH);
    getWorld()->addObject(IID_EXPLOSION, getX(), getY());
    if (randInt(1, 3) == 1) {
        if (randInt(1, 2) == 1) {
            getWorld()->addObject(IID_REPAIR_GOODIE, getX(), getY());
        }
        else {
            getWorld()->addObject(IID_TORPEDO_GOODIE, getX(), getY());
        }
    }
}

///////////////////////////////////////////////////
////////////////// Snagglegon /////////////////////
///////////////////////////////////////////////////
Snagglegon::Snagglegon(double startX, double startY, StudentWorld* world)
:Alien(IID_SNAGGLEGON, startX, startY, world, 10 * (1 + (world->getLevel() - 1) * 0.1))
{
    setDirection(2);
    setTravelSpeed(1.75);
}

Snagglegon::~Snagglegon()
{}

bool Snagglegon::withinRangeAction()
{
    int oneInHoweverMany = (15/(getWorld()->getLevel())) + 10;
    if (randInt(1, oneInHoweverMany) == 1) {
        getWorld()->addObject(-1, getX()-14, getY());
        return true;
    }
    return false;
}

void Snagglegon::deathSequence()
{
    getWorld()->increaseScore(1000);
    getWorld()->playSound(SOUND_DEATH);
    getWorld()->addObject(IID_EXPLOSION, getX(), getY());
    if (randInt(1, 6) == 1) {
        getWorld()->addObject(IID_LIFE_GOODIE, getX(), getY());
    }
}

///////////////////////////////////////////////////
//////////////////// Goodie ///////////////////////
///////////////////////////////////////////////////
Goodie::Goodie(int imageID, double startX, double startY, StudentWorld* world)
:Actor(imageID, startX, startY, 0, 0.5, 1, world)
{}

Goodie::~Goodie()
{}

void Goodie::doSomething()
{
    if (isAlive()) {
        if (getX() < 0 || getY() < 0 || getY() >= VIEW_HEIGHT) {
            setDead();
        }
        move();
    }
}

void Goodie::move()
{
    moveTo(getX()-0.75, getY()-0.75);
}

///////////////////////////////////////////////////
//////////////// Extra Life Goodie ////////////////
///////////////////////////////////////////////////
ExtraLifeGoodie::ExtraLifeGoodie(double startX, double startY, StudentWorld* world)
:Goodie(IID_LIFE_GOODIE, startX, startY, world)
{}

ExtraLifeGoodie::~ExtraLifeGoodie()
{}

///////////////////////////////////////////////////
////////////////// Repair Goodie //////////////////
///////////////////////////////////////////////////
RepairGoodie::RepairGoodie(double startX, double startY, StudentWorld* world)
:Goodie(IID_REPAIR_GOODIE, startX, startY, world)
{}

RepairGoodie::~RepairGoodie()
{}

///////////////////////////////////////////////////
//////////////// Torpedo Goodie ///////////////////
///////////////////////////////////////////////////
TorpedoGoodie::TorpedoGoodie(double startX, double startY, StudentWorld* world)
:Goodie(IID_TORPEDO_GOODIE, startX, startY, world)
{}

TorpedoGoodie::~TorpedoGoodie()
{}

///////////////////////////////////////////////////
////////////////// Explosion //////////////////////
///////////////////////////////////////////////////
Explosion::Explosion(double startX, double startY, StudentWorld* world)
:Actor(IID_EXPLOSION, startX, startY, 0, 1, 0, world)
{
    m_frameNumber = 0;
}

Explosion::~Explosion()
{}

void Explosion::doSomething()
{
    setSize(getSize()*1.5);
    move();
}

void Explosion::move()
{
    m_frameNumber++;
    if (m_frameNumber >= 4) {
        setDead();
    }
}
