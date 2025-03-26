#pragma once

#include <any>
#include <functional>

using CallbackType = std::function<void(const std::any&)>;

#define BindCallback(callback) std::bind((callback), this, std::placeholders::_1)
