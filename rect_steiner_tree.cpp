
#include <vector>
#include <unordered_set>
#include <algorithm>

#include <rect_steiner_tree.h>
#include <config.h>

#include <omp.h>

#include <boost/sort/sort.hpp>


struct DeltaCostCompare
{
    inline bool operator()(const std::pair<std::ptrdiff_t, GridPoint>& lhs,
                           const std::pair<std::ptrdiff_t, GridPoint>& rhs) const
    {
        return lhs.first > rhs.first;
    }
};


RectSteinerTree::RectSteinerTree(const PointSet& points, 
                                 const PointSet& hanan_grid_points)
    : mst(MinimumSpanningTree(points))
    , terminal_points(points)
    , rst_points(points)
{
    std::unordered_set<GridPoint, GridPointHasher> terminal_points_set(points.begin(), points.end());

    std::vector<std::pair<std::ptrdiff_t, GridPoint>> delta_costs(hanan_grid_points.size());

    std::unordered_set<GridPoint, GridPointHasher> rst_point_set(points.cbegin(), points.cend());

    const auto hanan_grid_size = hanan_grid_points.size();

    while (true)
    {
        #pragma omp parallel for
        for (std::size_t i = 0; i < hanan_grid_size; i++)
            if (rst_point_set.find(hanan_grid_points[i]) == rst_point_set.end())
            {
                auto hanan_mst = MinimumSpanningTree(mst, rst_points, hanan_grid_points[i]);
                delta_costs[i] = std::make_pair(delta_cost(mst, hanan_mst), hanan_grid_points[i]);
            }

        boost::sort::block_indirect_sort(delta_costs.begin(), 
                                         delta_costs.end(), 
                                         DeltaCostCompare(),
                                         config::ParallelConfig::get()->get_num_threads());

        if (delta_costs.empty() || delta_costs.front().first <= 0)
            break;

        for (std::size_t i = 0; i < hanan_grid_size; i++)
            if (delta_costs[i].first > 0 && rst_point_set.find(delta_costs[i].second) == rst_point_set.end())
            {
                auto tmp_mst = MinimumSpanningTree(mst, rst_points, delta_costs[i].second);
                auto tmp_cost = delta_cost(mst, tmp_mst);

                if (tmp_cost >= delta_costs[i].first)
                {
                    mst = std::move(tmp_mst);
                    rst_points.push_back(delta_costs[i].second);
                    rst_point_set.emplace(delta_costs[i].second);
                }
            }

        const auto prev_size = rst_point_set.size();

        for (auto it = rst_point_set.begin(); it != rst_point_set.end(); ) {
            if (terminal_points_set.find(*it) == terminal_points_set.end() && mst.get_vertex_degree(*it) <= 2)
                rst_point_set.erase(it++);
            else
                ++it;
        }

        if (prev_size != rst_point_set.size())
        {
            rst_points = std::vector<GridPoint>(rst_point_set.cbegin(), rst_point_set.cend());
            mst = MinimumSpanningTree(rst_points);
        }
    }
}


