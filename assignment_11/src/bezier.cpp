#include "bezier.h"
#include <utility>
#include <cmath>
#include <array>

// Calculate a point one of the Bezier curve segments
// @param bp_offset  index into bezier_control_points_ of the first of four
//                   control points defining the Bezier segment we want to evaluate.
// @param t          parametric distance along the curve at which to evaluate
vec3 PiecewiseBezier::eval_bezier(int bp_offset, float t) const {
    /** \todo
     * Assignment 11, Task 2a:
     * Evaluate the cubic Bezier polygon defined by control points
     * bezier_control_points_[bp_offset..bp_offset + 4] at parameter t in [0, 1]
     **/
    vec3 b01 = (1 - t) * bezier_control_points_[bp_offset] + t * bezier_control_points_[bp_offset + 1];
    vec3 b11 = (1 - t) * bezier_control_points_[bp_offset+1] + t * bezier_control_points_[bp_offset + 2];
    vec3 b21 = (1 - t) * bezier_control_points_[bp_offset+2] + t * bezier_control_points_[bp_offset + 3];
    
    vec3 b02 = (1 - t) * b01 + t * b11;
    vec3 b12 = (1 - t) * b11 + t * b21;
    return (1 - t) * b02 + t * b12;
}

// Calculate a tangent at point at one of the Bezier curve segments
// @param bp_offset  index into bezier_control_points_ of the first of four
//                   control points defining the Bezier segment we want to compute
//                   the tangent at
// @param t          parametric distance along the curve at which to evaluate
vec3 PiecewiseBezier::eval_bezier_tangent(int bp_offset, float t) const {
    /** \todo
     * Assignment 11, Task 3a:
     * Evaluate the tangent vector at "t" of the cubic Bezier polygon
     * defined by control points bezier_control_points_[bp_offset..bp_offset + 4]
     * Recall, the tangent vector for a curve c(t) is given by the derivative of
     * this curve, c'(t).
     **/
    const vec3& P0 = bezier_control_points_[bp_offset];
    const vec3& P1 = bezier_control_points_[bp_offset+1];
    const vec3& P2 = bezier_control_points_[bp_offset+2];
    const vec3& P3 = bezier_control_points_[bp_offset+3];

    return  -3*(1-t)*(1-t)*P0 + 3*(1-t)*(1-t)*P1 - 6*t*(1-t)*P1 - 3*t*t*P2 + 6*t*(1-t)*P2 + 3*t*t*P3;
}

std::vector<vec3> PiecewiseBezier::control_polygon_to_bezier_points(std::vector<vec3> const& cp) {
    std::vector<vec3> bezier_pts;
    /** \todo
     * Assignment 11, Task 1:
     * Determine the Bezier control points from the uniform cubic spline
     * control points "cp" as visualized in Lecture 12 slide 22.
     * Note, the last control point of one Bezier segment is shared with the first control
     * point of the next, and you should not generate the duplicate control points.
     * This means, Bezier segment 0 should be controlled by bezier_pts 0, 1, 2, 3;
     *                    segment 1 should be controlled by bezier_pts 3, 4, 5, 6;
     *                    segment 2 should be controlled by bezier_pts 6, 7, 8, 9;
     * and so on.
     **/
    //points:[0,cp.size()-1]
    //edges:[0,cp.size()-2]
    vec3 prev_control_point = 1.0 / 3 *cp[0] + 2.0 / 3 * cp[1];
    size_t numSegments = cp.size() - 3;
    for(int i = 1; i <= numSegments; i++) {
        //give the 1,2,3 bezier_point of ith edge (between point i and i+1)
        vec3 second_control_point = 1.0 / 3 * cp[i+1] + 2.0 / 3 * cp[i];
        //push 1st point
        bezier_pts.push_back(0.5 * prev_control_point + 0.5 * second_control_point);
        //push 2nd point
        bezier_pts.push_back(second_control_point);
        //push 3rd point and renew prev_control_point
        prev_control_point = 1.0 / 3 * cp[i] + 2.0 / 3 * cp[i+1];
        bezier_pts.push_back(prev_control_point);
    }
    
    vec3 temp_point = 1.0 / 3 * cp[cp.size()-1] + 2.0 / 3 * cp[cp.size()-2];
    bezier_pts.push_back(0.5 * prev_control_point + 0.5 * temp_point);
    
    //bezier_pts.resize(3 * numSegments + 1);
    std::cout << bezier_pts[0] <<  std::endl;
    std::cout << bezier_pts[bezier_pts.size()-1] <<  std::endl;
		
    return bezier_pts;
}


vec3 PiecewiseBezier::eval_piecewise_bezier_curve(float t) const {
    /** \todo
     *  Assignment 11, Task 2b:
     *  The argument t is in the interval [0, 1] which corresponds to the whole curve.
     *  The whole curve is defined by piecewise Bezier curves, which are each in turn
     *  parameterized by t_s in [0, 1]. Select appropriate control points and map the value t
     *  such that you evaluate the correct point on the correct Bezier segment. Note that
     *  the current code only evaluates the very first Bezier segment.
     */
    size_t numBezierPiece = (bezier_control_points_.size() - 1) / 3;
    float delta = 1.0 / numBezierPiece;
    int pieceInd = t/delta; //when t = 1 this doesnt work
    if(t == 1) pieceInd = numBezierPiece - 1;

    return eval_bezier(3 * pieceInd, (t - delta * pieceInd)/delta);
}

vec3 PiecewiseBezier::operator()(float t) const {
    return eval_piecewise_bezier_curve(t);
}

vec3 PiecewiseBezier::tangent(float t) const {
    /** \todo
     *  Assignment 11, Task 3b:
     *  The argument t is in the interval [0, 1] which corresponds to the whole curve.
     *  The whole curve is defined by piecewise Bezier curves, which are each in turn
     *  parameterized by t_s in [0, 1]. Select appropriate control points and map the value t
     *  such that you evaluate the correct point on the correct Bezier segment. Note that
     *  the current code only evaluates the very first Bezier segment.
     *  Also, remember to use the chain rule when computing the tangent!
     */
    size_t numBezierPiece = (bezier_control_points_.size() - 1) / 3;
    float delta = 1.0 / numBezierPiece;
    int pieceInd = t/delta; //when t = 1 this doesnt work
    if(t == 1) pieceInd = numBezierPiece - 1;

    return eval_bezier_tangent(3 * pieceInd, (t - delta * pieceInd)/delta)*float(numBezierPiece);
}

void PiecewiseBezier::set_control_polygon(const std::vector<vec3> &control_polygon, bool loop) {
    control_polygon_ = control_polygon;
    if(loop) {
        control_polygon_.push_back(control_polygon[0]);
        control_polygon_.push_back(control_polygon[1]);
        control_polygon_.push_back(control_polygon[2]);
    }

    bezier_control_points_ = control_polygon_to_bezier_points(control_polygon_);
}
