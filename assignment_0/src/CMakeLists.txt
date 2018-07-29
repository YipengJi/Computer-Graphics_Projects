file(GLOB SRCS ./*.cpp)
file(GLOB HDRS ./*.h)

add_executable(raytrace ${SRCS} ${HDRS})
