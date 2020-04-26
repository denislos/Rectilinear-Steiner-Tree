

#include <minimum_spanning_tree.h>


#include <vector>
#include <array>
#include <utility>
#include <tuple>
#include <iostream>


MinimumSpanningTree::MinimumSpanningTree(const PointSet& points)
{
    const auto size = points.size();

    std::vector<std::size_t> distances(size, DIST_INF);
    std::vector<GridPoint> parents(size);
    
    vertices.reserve(size);

    std::vector<bool> is_present_array(size, false);

    std::size_t cnt = 0;
    distances[cnt] = 0;

    for (std::size_t i = 0; i < size - 1; i++)
    {
        is_present_array[cnt] = true;
        
        for (std::size_t j = 0; j < size; j++)
            if (j != cnt)
            {
                auto dst = distance(points[cnt], points[j]);
                if (!is_present_array[j] && dst < distances[j])
                {
                    distances[j] = dst;
                    parents[j] = points[cnt];
                }
            }

        auto min_dst = DIST_INF;
        std::size_t min_pos = 0;

        for (std::size_t j = 0; j < size; j++)
            if (distances[j] < min_dst && !is_present_array[j])
            {
                min_dst = distances[j];
                min_pos = j;
            }
        
        cnt = min_pos;
        
        vertex_t p, v;
        auto result = vertices.find(parents[cnt]);
        if (result != vertices.end())
        {
            p = result->second;
        }
        else
            p = vertices[parents[cnt]] = boost::add_vertex(parents[cnt], adjacency_list);

        result = vertices.find(points[cnt]);
        if (result != vertices.end())
        {
            v = result->second;
        }
        else
            v = vertices[points[cnt]] = boost::add_vertex(points[cnt], adjacency_list);

        const auto dst = distance(parents[cnt], points[cnt]);
        cost += dst;

        boost::add_edge(p, v, dst, adjacency_list);
    }
}


MinimumSpanningTree::MinimumSpanningTree(const MinimumSpanningTree& old_mst,
                                         const PointSet& points,
                                         const GridPoint& new_point)
    : MinimumSpanningTree(old_mst)
{
    std::array<std::tuple<std::size_t, bool, GridPoint>, NUM_REGIONS> closest_points;
    
    closest_points.fill(std::make_tuple(DIST_INF, false, GridPoint()));

    for (const auto& point : points)
    {
        auto& [closest_dst, is_touched, closest_point] = closest_points[get_region(point, new_point)];

        const auto dst = distance(point, new_point);
        if (dst < closest_dst)
        {
            closest_dst = dst;
            closest_point = point;
            is_touched = true;
        }
    }

    auto new_v = boost::add_vertex(new_point, adjacency_list);
    vertices[new_point] = new_v;

    for (const auto& [closest_dst, is_touched, closest_point] : closest_points)
        if (is_touched)
        {
            if (boost::degree(new_v, adjacency_list) != 0)
            {
                auto result_handler = CycleSearchResultHandler(); 
            
                auto vis = MaxEdgeCycleVisitor(closest_point);
                vis.set_result_handler(&result_handler);

                boost::depth_first_search(adjacency_list,
                                          boost::visitor(vis).
                                          root_vertex(new_v));

                const auto [max_weight, max_weighted_edge] = result_handler.get_result();

                if (max_weight > closest_dst)
                {
                    boost::remove_edge(max_weighted_edge, adjacency_list);
                    boost::add_edge(new_v, vertices[closest_point], closest_dst, adjacency_list);

                    cost = (cost + closest_dst) - max_weight;
                }
            }
            else
            {
                boost::add_edge(new_v, vertices[closest_point], closest_dst, adjacency_list);
                cost += closest_dst;
            }
        }
}


void MinimumSpanningTree::MaxEdgeCycleVisitor::finish_vertex([[maybe_unused]]const MinimumSpanningTree::vertex_t& vertex, 
                                                             [[maybe_unused]]const MinimumSpanningTree::Graph& graph)
{
    if (!is_route_found)
        route.pop_back();
}



void MinimumSpanningTree::MaxEdgeCycleVisitor::tree_edge(const MinimumSpanningTree::edge_t& edge, 
                                                         const MinimumSpanningTree::Graph& graph)
{
    if (!is_route_found)
    {
        auto weight_property = boost::get(boost::edge_weight, graph);
        const auto weight = boost::get(weight_property, edge);

        route.push_back(std::make_pair(weight, edge));

        auto coordinates = boost::get(MinimumSpanningTree::coord_t(), graph);

        if (boost::get(coordinates, boost::target(edge, graph)) == target)
        {
            is_route_found = true;
            const auto [max_weight, edge] = *std::max_element(route.begin(), route.end(), RouteEdgeCompare());
            result_handler->set_result(max_weight, edge);
        }
    }
}


std::size_t MinimumSpanningTree::get_region(const GridPoint& w, const GridPoint& p) const
{
    /*
     0
    2 1
     3
    */

    const std::size_t l1_v = (w.get_y() + p.get_x() < w.get_x() + p.get_y());
    const std::size_t l2_v = (w.get_y() + w.get_x() < p.get_y() + p.get_x());

    return (l2_v << 1) | l1_v;
}


std::size_t MinimumSpanningTree::get_vertex_degree(const GridPoint& point) const
{
    return boost::degree(vertices.at(point), adjacency_list);
}



std::ostream& operator<<(std::ostream& os, const MinimumSpanningTree& mst)
{
    auto coordinates = boost::get(MinimumSpanningTree::coord_t(), mst.adjacency_list);
    for (auto [ei, ei_end] = boost::edges(mst.adjacency_list); ei != ei_end; ++ei)
    {
        os << boost::get(coordinates, boost::source(*ei, mst.adjacency_list))
           << " -> "
           << boost::get(coordinates, boost::target(*ei, mst.adjacency_list)) << "\n";
    }

    return os;
}


MinimumSpanningTree::const_iterator MinimumSpanningTree::begin() const
{
    auto ei = boost::edges(adjacency_list).first;
    return const_iterator(&adjacency_list, ei);
}


MinimumSpanningTree::const_iterator MinimumSpanningTree::end() const
{
    auto ei_end = boost::edges(adjacency_list).second;
    return const_iterator(&adjacency_list, ei_end);
}


MinimumSpanningTree::edge_output_type MinimumSpanningTree::ConstEdgeIterator::operator*() const
{
    auto coordinates = boost::get(coord_t(), *graph);

    const auto& source_point = boost::get(coordinates, boost::source(*it, *graph));
    const auto& target_point = boost::get(coordinates, boost::target(*it, *graph));

    return std::make_pair(source_point, target_point);
}