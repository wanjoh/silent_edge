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
    auto getTurretMode() -> TurretMode;
    auto getTurretType() -> TurretType;
    auto getTurretControllers() -> std::list<Player>;
    auto getAvailableTargets() -> std::list<Player>;

private:
    TurretMode mode_;
    TurretType type_;
    std::list<Player> turret_controllers_;
    std::list<Player> available_targets_;
};
