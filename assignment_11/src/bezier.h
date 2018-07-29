#ifndef BEZIER_H
#define BEZIER_H

#include <vector>
#include "glmath.h"

// A uniform cubic spline curve built from piecewise Bezier curves.
// This curve is defined on the interval [0, 1].
class PiecewiseBezier {
public:
    PiecewiseBezier() { }

    void set_control_polygon(const std::vector<vec3>& control_polygon, bool loop=false);

    // Evaluate the piecewise curve for t in [0, 1].
    // This involves determining which Bezier curve segment contains the
    // evaluation point and finding the parametric distance along this curve
    // segment corresponding to "t".
    vec3 operator()(float t) const;
    vec3 tangent(float t) const;

    // Number of Bezier curve segments making up the spline curve.
    int num_segments() const {
        // There is a Bezier curve segment for each interior edge of the spline
        // control polygon.
        return control_polygon_.size() - 3;
    }

    const std::vector<vec3> bezier_control_points() const { return bezier_control_points_; }

private:
    vec3 eval_piecewise_bezier_curve(       float t) const;
    vec3 eval_bezier        (int bp_offset, float t) const;
    vec3 eval_bezier_tangent(int bp_offset, float t) const;
    std::vector<vec3> control_polygon_to_bezier_points(std::vector<vec3> const& control_polygon);

    std::vector<vec3> control_polygon_;
    std::vector<vec3> bezier_control_points_;
};

#endif  // BEZIER_H
