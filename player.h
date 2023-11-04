#pragma once

#include "entity.h"
#include "weapon.h"

class Player : public Entity
{
public:
    float get_hp();
    float get_energy();
    float get_dpp();
    float get_score();
    void move(std::pair<int, int>);
private:
    float hp;
    float energy;
    float dpp;
    float score;
    std::list<Weapon> weapons;
};
