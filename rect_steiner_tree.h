

#ifndef RECT_STEINER_TREE_H
#define RECT_STEINER_TREE_H

#include <ostream>
#include <unordered_set>

#include <grid_point.h>
#include <minimum_spanning_tree.h>

class RectSteinerTree
{
public:
    RectSteinerTree(const PointSet& points, const PointSet& hanan_grid_points);

    friend inline std::ostream& operator<<(std::ostream& os, const RectSteinerTree& rst)
    {
        return os << rst.mst;
    }

    using edge_type = MinimumSpanningTree::edge_output_type;
    using const_iterator = MinimumSpanningTree::const_iterator;

    const_iterator begin() const { return mst.begin(); }
    const_iterator end() const { return mst.end(); }

    const auto& get_rst_points() const { return rst_points; }
    const auto& get_terminal_points() const { return terminal_points; }

    std::size_t num_rst_points() const { return rst_points.size(); }

private:
    MinimumSpanningTree mst;

    PointSet terminal_points;
    PointSet rst_points;
};


#endif // RECT_STEINER_TREE_H