#pragma once

#include "order.h"
#include <iomanip>
#include <sstream>
#include <string>
#include <functional>

std::string generateAuthHash(const Order &order, const std::string &userKey);