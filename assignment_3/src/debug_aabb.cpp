//=============================================================================
//
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//
//=============================================================================

//== includes =================================================================

#include "StopWatch.h"
#include "Scene.h"
#include "Mesh.h"

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

/// Program entry point.
int main(int argc, char **argv) {
    // Parse input scene file/output path from command line arguments
    struct RaytraceJob { std::string scenePath, outPath; };
    std::vector<RaytraceJob> jobs;

    if (argc == 3)
        jobs.emplace_back(RaytraceJob{argv[1], argv[2]});
    else if ((argc == 2) && argv[1][0] == '0') {
        jobs = { {
            {"../scenes/mask/mask.sce",             "debug_aabb_mask.tga"},
            {"../scenes/toon_faces/toon_faces.sce", "debug_aabb_toon_faces.tga"},
            {"../scenes/office/office.sce",         "debug_aabb_office.tga"},
            {"../scenes/rings/rings.sce",           "debug_aabb_rings.tga"}
        } };
    }
    else {
        std::cerr << "Usage: " << argv[0] << " input.sce output.tga\n";
        std::cerr << "Or: " << argv[0] << " 0\n";
        std::cerr << std::flush;
        exit(1);
    }

    for (const auto &job : jobs) {
        std::cout << "Read scene '" << job.scenePath << "'..." << std::flush;
        Scene s(job.scenePath);
        std::cout << "\ndone (" << s.numObjects() << " objects)\n";

        const auto &c = s.getCamera();
        Image img(c.width, c.height);
        std::vector<size_t> numIntersected(c.width * c.height);
        for (int x=0; x<int(c.width); ++x) {
            for (int y=0; y<int(c.height); ++y) {
                Ray ray = c.primary_ray(x,y);

                for (const auto &o: s.getObjects()) {
                    if (auto mesh = dynamic_cast<const Mesh *>(o.get())) {
                        if (mesh->intersect_bounding_box(ray))
                            ++numIntersected[y * c.width + x];
                    }
                }
            }
        }
        size_t maxIntersectionCount = *std::max_element(numIntersected.begin(), numIntersected.end());
        for (int x=0; x<int(c.width); ++x)
            for (int y=0; y<int(c.height); ++y)
                img(x, y) = vec3(numIntersected[y * c.width + x] / float(maxIntersectionCount), 0, 0);

        img.write(job.outPath);
    }
}
