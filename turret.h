#pragma once

#include "entity.h"
#include "player.h"

class Turret : public Entity
{
public:
    enum class turret_mode {
        ACTIVE,
        DAMAGED,
        BROKEN
    };
    enum class turret_type {
        STRAIGHTSHOT,
        DIAGONALSHOT,
        ALTERNATING,
        LOCKON
    };
    std::string get_turret_mode();
    std::string get_turret_type();
    std::list<Player> get_turret_controllers();
    std::list<Player> get_available_targets();
private:
    turret_mode mode;
    turret_type type;
    std::list<Player> turret_controllers;
    std::list<Player> available_targets;
};
