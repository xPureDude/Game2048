#pragma once

#include <random>

class Random
{
public:
    Random();
    ~Random() = default;

    std::int32_t GetInt(std::int32_t min, std::int32_t max);
    std::double_t GetDouble(std::double_t min, std::double_t max);

    //    std::double_t GetNormal(std::double_t mean, std::double_t stddev);

private:
    std::random_device m_device;
    std::mt19937 m_engine;
};