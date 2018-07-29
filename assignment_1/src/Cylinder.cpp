//=============================================================================
//
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//
//=============================================================================

//== INCLUDES =================================================================

#include "Cylinder.h"
#include "SolveQuadratic.h"
#include "Plane.h"

#include <array>
#include <cmath>

//== IMPLEMENTATION =========================================================

bool
Cylinder::
intersect(const Ray&  _ray,
          vec3&       _intersection_point,
          vec3&       _intersection_normal,
          double&     _intersection_t) const
{
    /** \todo
     * - compute the first valid intersection `_ray` with the cylinder
     *   (valid means in front of the viewer: t > 0)
     * - store intersection point in `_intersection_point`
     * - store ray parameter in `_intersection_t`
     * - store normal at _intersection_point in `_intersection_normal`.
     * - return whether there is an intersection with t > 0
    */

    //default init arrays
    //cuz it's floating points operation, there may be duplicate values t (with slightly difference caused by error) between plane and side, so we use size of 4 instead of 2.
    std::array	<double	, 4> intersect_t_arr;
    std::array	<vec3	, 4> intersect_point_arr;
    std::array	<vec3	, 4> intersection_normal_arr;
   
    //(d.d - (d.a)^2)t^2 + 2[d.(o-c)-(d.a)((o-c).a)]t + (o-c)^2 - ((o-c).a)^2 - radius^2 = 0
    vec3 o_c = _ray.origin - center;
	
	//(d.d - (d.a)^2)
    double A = dot(_ray.direction,_ray.direction) - dot(axis,_ray.direction) * dot(axis,_ray.direction);
    //2[d.(o-c)-(d.a)((o-c).a)]
    double B = 2 * ( dot(_ray.direction,o_c) - dot(_ray.direction,axis) * dot(o_c,axis) );
    //(o-c)^2 - ((o-c).a)^2 - radius^2
    double C = dot(o_c,o_c) - dot(o_c,axis) * dot(o_c,axis) - radius * radius;

    std::array<double, 2> sol = {0,0};
    size_t sol_num = solveQuadratic(A,B,C,sol);
    int intersect_num = 0;
    
    for (int i = 0; i < sol_num; i++) {
		if (sol[i] >= 0 
			&& dot((_ray(sol[i]) - center), axis) < height/2 
			&& dot((_ray(sol[i]) - center), axis) > -height/2) {
			intersect_t_arr[intersect_num] = sol[i];
			intersect_point_arr[intersect_num] = _ray(sol[i]);
			intersection_normal_arr[intersect_num] 
				= normalize(_ray(sol[i]) - center - (dot(_ray.direction,axis)*sol[i]+dot(o_c,axis)) * axis);
			if (dot(_ray.direction,intersection_normal_arr[intersect_num]) > 0)
				intersection_normal_arr[intersect_num] = -intersection_normal_arr[intersect_num];
			intersect_num++;
		}
	}
	
//	if (sol_num == 2) {
//		if (sol[0] > 0 && sol[1] > 0) {
//			double minSol = (sol[0]<sol[1])?sol[0]:sol[1];
//			double maxSol = (sol[0]>sol[1])?sol[0]:sol[1];
//			
//			if ((dot((_ray(minSol) - center), axis) > height/2 || dot((_ray(minSol) - center), axis) < -height/2) 
//			 && (dot((_ray(maxSol) - center), axis) < height/2 || dot((_ray(maxSol) - center), axis) > -height/2)) {
//				 intersection_normal_arr[0] = -intersection_normal_arr[0];
//			}
//		}
//	}
//	
		
	if (intersect_num == 0) return false;

    //final select
    int index = 0; // argmin
    double cur_min = intersect_t_arr[0];
    for (int i = 0; i < intersect_num; i++) 
        if (intersect_t_arr[i] < cur_min) index = i;

    _intersection_t = intersect_t_arr[index];
    _intersection_normal = intersection_normal_arr[index];
    _intersection_point = intersect_point_arr[index];
    return true;
}
