#pragma once

#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <string>
#include "order.h"

std::string generateAuthHash(const Order& order, const std::string& userKey);