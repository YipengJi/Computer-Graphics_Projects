#include "render_2d.h"
#include <iostream>
#include <sstream>
#include "misc.h"
#include "../lsystem.h"
#include "LSystemViewer.h"

void render_glfw(LindenmayerSystem &lsys, const std::string &out_path, size_t window_size)
{
	bool const should_display_window = out_path.empty();

    LSystemViewer window(lsys, "Lindenmayer System", window_size, window_size, should_display_window);
    window.run(out_path);
}
