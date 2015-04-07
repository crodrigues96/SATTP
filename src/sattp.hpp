#pragma once
//---
#include <iostream>
#include <thread>
#include <fstream>
#include <sstream>
#include <ostream>
#if defined _WIN32
#include <direct.h>  // For get_current_working_dir()
#else
#include <unistd.h>  // For get_current_working_dir()
#endif
//---
#include "samp/sampgdk.h"
//---
#include "webserver/mongoose.h"
//---
#include "CWebserver.hpp"
//---
