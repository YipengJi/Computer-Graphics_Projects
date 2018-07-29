#pragma once
#include <vector>
#include <string>
#include <functional>
#include <memory>
#include "vec.h"

class LindenmayerSystem;

void render_glfw(
	LindenmayerSystem &lsys,
    const std::string &out_path = "",
	size_t window_size = 1024
);
