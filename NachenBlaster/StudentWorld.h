#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <vector>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    //Constructor
    StudentWorld(std::string assetDir);
    //Destructor
    virtual ~StudentWorld();
    //Populates the game
    virtual int init();
    //Controls actions in one tick
    virtual int move();
    //Deletes all objects and resets the world
    virtual void cleanUp();
    //Deletes dead objects
    void removeDeadObjects();
    //Subtracts number_aliens and remaining_aliens
    void killAlien();
    //Subtracts number_aliens
    void subtractAlien();
    //Decides whether or not to add alien and which alien to add
    void toAddNewAlien();
    //Returns maximum number of aliens that can be on screen
    int maxAliensOnScreen() const;
    //Returns remainingAliensToKill
    int getRemainingAliensToKill() const;
    //Prints status line
    void updateStatusLine();
    //Adds a specific object at a specific X and Y
    void addObject(int ID, double startX, double startY);
    //Returns whether or not actor1 and actor2 can collide based on ID
    bool canCollide(Actor* actor1, Actor* actor2);
    //Returns the image ID of collided object or -1
    int checkCollision(Actor* actor);
    //Return m_player
    NachenBlaster* getPlayer() const;
private:
    //Vector of Actor pointers
    vector<Actor*> m_actors;
    //Number of aliens to kill to advance to next level
    int remainingAliensToKill;
    //Number of aliens on screen
    int number_aliens;
    //NachenBlaster pointer
    NachenBlaster* m_player;
};

#endif // STUDENTWORLD_H_
