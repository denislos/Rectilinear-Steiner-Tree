

#ifndef HANAN_GRID_H
#define HANAN_GRID_H


#include <grid_point.h>


PointSet construct_hanan_grid_points(const PointSet& points,
                                     coord_type begin_x, coord_type end_x,
                                     coord_type begin_y, coord_type end_y);


#endif // HANAN_GRID_H