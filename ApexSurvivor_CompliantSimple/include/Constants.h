#pragma once

// All important game parameters are kept here instead of being hidden in code.
// This makes the game easy to balance during presentation.
namespace Constants {
    constexpr unsigned WindowWidth = 1200;
    constexpr unsigned WindowHeight = 800;

    constexpr float PlayerSpeed = 260.0f;
    constexpr float PlayerBoostSpeed = 420.0f;
    constexpr int PlayerMaxHp = 100;

    constexpr float KartSpeed = 120.0f;
    constexpr float DroneSpeed = 170.0f;
    constexpr int KartDamage = 12;
    constexpr int DroneDamage = 8;

    constexpr float EnemySpawnTime = 1.4f;
    constexpr float ItemSpawnTime = 2.2f;

    constexpr int TelemetryToWin = 5;
}
