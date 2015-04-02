#pragma once
//---
#ifndef _WIN32
	#define __LINUX__
#endif
//---
#include <iostream>
#include <cstring>
#include <vector>
#include <stdarg.h>
#include <thread>
#include <fstream>
#include <sstream>
#include <ostream>
#include <Windows.h>
//---
#include "samp\sampgdk.h"
//---
#include "webserver\mongoose.h"
//---
extern struct mg_server *WServer;