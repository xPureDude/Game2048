#include "Component.hpp"

void PositionComponent::Reset() {}

bool PositionComponent::LoadFromXml(tinyxml2::XMLElement* e)
{
    return true;
}

void MoveComponent::Reset() {}

bool MoveComponent::LoadFromXml(tinyxml2::XMLElement* e)
{
    return true;
}