#pragma once

#include "entity.hpp"
#include "player.hpp"

class Turret : public Entity
{
public:
    enum class TurretMode
    {
        ACTIVE,
        DAMAGED,
        BROKEN
    };
    enum class TurretType
    {
        STRAIGHTSHOT,
        DIAGONALSHOT,
        ALTERNATING,
        LOCKON
    };
    TurretMode getTurretMode();
    TurretType getTurretType();
    std::list<Player> getTurretControllers();
    std::list<Player> getAvailableTargets();
private:
    TurretMode mode_;
    TurretType type_;
    std::list<Player> turret_controllers_;
    std::list<Player> available_targets_;
};
