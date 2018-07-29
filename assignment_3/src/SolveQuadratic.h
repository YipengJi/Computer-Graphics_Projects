//=============================================================================
//
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//   Author:  Julian Panetta (jpanetta), julian.panetta@gmail.com
//
//=============================================================================

#ifndef SOLVEQUADRATIC_H
#define SOLVEQUADRATIC_H
#include <cmath>
#include <array>

/// Numerically robust solution to (possibly degenerate) quadratic equations.
/// Avoids catastrophic cancellation and handles equations that have
/// degenerated to become linear or constant.
/// @param[in]   a,b,c    coefficients of ax^2 + bx + c == 0
/// @param[out]  solns    array holding between 0 and 2 solutions
/// @return      number of solutions found
inline size_t solveQuadratic(double a, double b, double c, std::array<double, 2> &solns) {
    // Handle degenerate (linear) case
    if (std::abs(a) < 1e-10) {
        if (std::abs(b) < 1e-10) return 0;
        solns[0] = - c / b;
        return 1;
    }

    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) return 0;

    // Avoid cancellation:
    // One solution doesn't suffer cancellation:
    //      a * x1 = 1 / 2 [-b - bSign * sqrt(b^2 - 4ac)]
    // "x2" can be found from the fact:
    //      a * x1 * x2 = c
    double a_x1 = -0.5 * (b + copysign(std::sqrt(discriminant), b));

    solns = { a_x1 / a, c / a_x1 };
    return 2;
}

#endif /* end of include guard: SOLVEQUADRATIC_H */
