#include "camera.hpp"

#include "player.hpp"

void CastEngine::Camera::Follow(Player &player)
{
    mPos = player.GetPos();
    mDir = player.GetDir();
}