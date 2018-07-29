
#pragma once
#include <string>
#include <vector>
#include "render_2d.h" // for definition of Segment

std::string draw_segments_to_svg_file(std::vector<Segment> const& segments);
