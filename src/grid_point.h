

#ifndef GRID_POINT_H
#define GRID_POINT_H

#include <utility>
#include <types.h>
#include <vector>
#include <ostream>


struct GridPoint
{
public:
    GridPoint() = default;
    GridPoint(coord_type x, coord_type y)
        : x(x)
        , y(y)
    { }

    inline auto get_x() const { return x; }
    inline auto get_y() const { return y; }

    inline bool operator==(const GridPoint& rhs) const
    { 
        return get_x() == rhs.get_x() && get_y() == rhs.get_y();
    }

    friend std::ostream& operator<<(std::ostream& os, const GridPoint& point)
    {
        return os << "(" << point.get_x() << ", " << point.get_y() << ")";
    }

private:
    coord_type x = 0;
    coord_type y = 0;
};


static inline std::size_t distance(const GridPoint& lhs, const GridPoint& rhs)
{
    return std::abs(rhs.get_x() - lhs.get_x()) + std::abs(rhs.get_y() - lhs.get_y());
}

static inline bool is_same_horizontal(const GridPoint& lhs, const GridPoint& rhs)
{
    return lhs.get_y() == rhs.get_y();
}


static inline bool is_same_vertical(const GridPoint& lhs, const GridPoint& rhs)
{
    return lhs.get_x() == rhs.get_x();
}


struct GridPointHasher
{
    std::size_t operator()(const GridPoint& p) const
    {
        return std::hash<coord_type>()(p.get_x()) ^ std::hash<coord_type>()(p.get_y());
    }
};

using PointSet = std::vector<GridPoint>;

#endif // GRID_POINT_H