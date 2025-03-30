#include "Random.hpp"

Random::Random()
    : m_engine(m_device())
{
}

std::int32_t Random::GetInt(std::int32_t min, std::int32_t max)
{
    std::uniform_int_distribution<> dist(min, max);
    return dist(m_engine);
}

std::double_t Random::GetDouble(std::double_t min, std::double_t max)
{
    std::uniform_real_distribution<> dist(min, max);
    return dist(m_engine);
}