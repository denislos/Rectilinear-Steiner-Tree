

#include <hanan_grid.h>
#include <unordered_set>

PointSet construct_hanan_grid_points(const PointSet& points,
                                     coord_type begin_x, coord_type end_x,
                                     coord_type begin_y, coord_type end_y)
{
    std::unordered_set<GridPoint, GridPointHasher> point_set(points.begin(), points.end());

    PointSet hanan_grid_points((end_x - begin_x + 1) * (end_y - begin_y + 1) - points.size());

    std::size_t cnt = 0;

    for (coord_type i = begin_x; i <= end_x; i++)
        for (coord_type j = begin_y; j <= end_y; j++)
        {
            const auto hanan_grid_point = GridPoint(i, j);
            if (point_set.find(hanan_grid_point) == point_set.end())
                hanan_grid_points[cnt++] = hanan_grid_point;
        }
    
    return hanan_grid_points;
}