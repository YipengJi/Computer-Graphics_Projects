
#include "render_svg.h"
#include "misc.h"
#include <sstream>
#include <iomanip>

const double SVG_MAX_SIDE = 1024;

const double DISTANCE_EPS = 1e-3;
const double DISTANCE_EPS_SQ = DISTANCE_EPS*DISTANCE_EPS;

const double LINE_WIDTH = 0.04;

/*
	We draw the segments as elements of one svg path
	An XML library should be used for that, but here we construct the XML from strings
	because adding another library would add difficulty with the installation and portability.
*/
std::string draw_segments_to_svg_file(std::vector<Segment> const& segments) {

	
	auto bounding_box = segment_group_bounding_box(segments);
	vec2 top_left = bounding_box.first;
	vec2 bot_right = bounding_box.second;

	// add margin
	top_left -= {1, 1};
	bot_right += {1, 1};
	
	vec2 const size = bot_right - top_left;
	vec2 const img_size = size * (SVG_MAX_SIDE / std::max(size[0], size[1]));
	vec2 const viewbox_top_left = {top_left[0], -bot_right[1]};

	std::stringstream out_text;
	out_text << std::setprecision(4);
	out_text << "<svg version=\"1.1\" baseProfile=\"full\" xmlns=\"http://www.w3.org/2000/svg\"\n";
 	out_text << "width=\"" << img_size[0] << "\" height=\"" << img_size[1] << "\"\n";
	out_text << "viewBox=\"" << viewbox_top_left[0] << ' ' << viewbox_top_left[1] << ' ' << size[0] << ' ' << size[1] << "\"\n";
	out_text << ">\n";
	
	// White background
	out_text << "<rect x=\"" << viewbox_top_left[0] << "\" y=\"" << viewbox_top_left[1] << "\" ";
	out_text << "width=\"100%\" height=\"100%\" fill=\"white\" />\n";

	// Path containing the segments
	out_text << "<path stroke=\"green\" stroke-width=\"" << LINE_WIDTH << "\" stroke-linecap=\"round\" fill=\"none\"\n";
	out_text << "transform=\"scale(1 -1)\"\n"; // flip Y so that positive Y is up
	out_text << "d=\"";

	vec2 pen_pos{-0.5, -0.5};
	for(auto const& seg : segments) {
		// move the pen to the beginning of the segment if its not there already
		if(norm2(seg.first - pen_pos) > DISTANCE_EPS_SQ) {
			out_text << "	M " << seg.first[0] << ' ' << seg.first[1] << '\n';
		}
		
		// draw to the end of the segment
		pen_pos = seg.second;
		out_text << "	L " << pen_pos[0] << ' ' << pen_pos[1] << '\n';
	}
	out_text << "\"/>\n";
	
	out_text << "</svg>";
	return out_text.str();
}
