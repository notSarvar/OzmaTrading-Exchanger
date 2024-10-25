#pragma once

#include "order.h"
#include <functional>
#include <iomanip>
#include <sstream>
#include <string>

std::string GenerateAuthHash(const Order &order, const std::string &userKey);