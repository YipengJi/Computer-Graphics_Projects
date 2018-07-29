#include "ShadowViewer.h"
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ShadowViewer mesh.off" << std::endl;
        exit(-1);
    }

    ShadowViewer window("Shadow Viewer", 640, 480);
    window.loadMesh(argv[1]);
    return window.run();
}
