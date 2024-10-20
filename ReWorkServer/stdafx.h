#pragma once
#include <iostream>
#include <thread>
#include <atomic>
#include <concurrent_unordered_map.h>
#include <unordered_map>
#include <array>
#include <queue>
#include <random>
#include <cstring>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

#include "protocol.h"

using namespace std;
using boost::asio::ip::tcp;
