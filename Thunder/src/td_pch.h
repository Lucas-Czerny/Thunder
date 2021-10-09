#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <functional>
#include <algorithm>
#include <random>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

#include "Thunder/Core/Log.h"
#include "Thunder/Debug/Profiling.h"

#ifdef TD_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <shobjidl.h>
#endif