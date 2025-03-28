#pragma once

#include <any>
#include <functional>
#include <string>
#include <vector>

std::vector<std::string> SplitString(std::string& str, char c);

using CallbackType = std::function<void(const std::any&)>;

#define BindCallback(callback) std::bind((callback), this, std::placeholders::_1)