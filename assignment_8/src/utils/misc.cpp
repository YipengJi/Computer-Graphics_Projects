
#include "misc.h"
#include <string>
#include <iostream>
#include <fstream>

void write_string_to_file(std::string const& data, std::string const& file_path) {
	std::ofstream out_file;
	out_file.open(file_path);
    if (!out_file.is_open())
        throw std::runtime_error("Couldn't open " + file_path + " for writing");
    out_file << data << std::endl;
}

void segment_minmax(Segment const& s, int const& coord, double& minval, double& maxval) {
	minval = std::min(minval, std::min(s.first[coord], s.second[coord]));
	maxval = std::max(maxval, std::max(s.first[coord], s.second[coord]));
}

Segment segment_group_bounding_box(std::vector<Segment> const& segments) {
	vec2 top_left(0, 0), bot_right(0, 0);

	if( segments.size() > 0 ) {
		top_left = segments[0].first;
		bot_right = segments[0].first;

		for(auto const& s : segments) {
			segment_minmax(s, 0, top_left[0], bot_right[0]);
			segment_minmax(s, 1, top_left[1], bot_right[1]);
		}
	} else {
		// no segments
	}
	
	return {top_left, bot_right};
}
