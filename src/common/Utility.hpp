#pragma once

#include <any>
#include <functional>
#include <string>
#include <vector>

namespace evt
{
class Base;
};

using VecString = std::vector<std::string>;
VecString SplitString(std::string& str, char c);

using CallbackType = std::function<void(const std::any&)>;
using EventCallbackType = std::function<void(evt::Base* e)>;

#define BindCallback(callback) std::bind((callback), this, std::placeholders::_1)