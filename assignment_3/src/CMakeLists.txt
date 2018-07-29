file(GLOB SRCS_COMMON Cylinder.cpp Mesh.cpp Plane.cpp Scene.cpp Sphere.cpp vec3.cpp)
file(GLOB SRCS raytrace.cpp ${SRCS_COMMON})
file(GLOB HDRS ./*.h)

add_executable(raytrace raytrace.cpp ${SRCS_COMMON} ${HDRS})
add_executable(debug_aabb debug_aabb.cpp ${SRCS_COMMON} ${HDRS})
