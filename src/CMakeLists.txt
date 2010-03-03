set(VORONOI_SRCS
main.cpp
gui.cpp
coastline.cpp
fortunesAlgorithm.cpp
voronoiDiagram.cpp
eventCalender.cpp
)

add_executable(voronoi ${VORONOI_SRCS})

set_target_properties(voronoi PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

target_link_libraries(voronoi
${SDL_LIBS}
${GLU_LIBRARY})
