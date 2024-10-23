#pragma once

#include "order.h"
#include <iomanip>
#include <sstream>
#include <string>

std::string generateAuthHash(const Order &order, const std::string &userKey);