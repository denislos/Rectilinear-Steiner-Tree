

#ifndef MINIMUM_SPANNING_TREE_H
#define MINUMUM_SPANNING_TREE_H

#include <cstdint>
#include <ostream>
#include <unordered_map>

#include <grid_point.h>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>


class MinimumSpanningTree
{
private:
    class ConstEdgeIterator;

public:
    MinimumSpanningTree(const PointSet& points);
    MinimumSpanningTree(const MinimumSpanningTree& mst, const PointSet& points, const GridPoint& point);

    inline auto get_cost() const { return cost; }

    friend std::ostream& operator<<(std::ostream& os, const MinimumSpanningTree& mst);

    std::size_t get_vertex_degree(const GridPoint& point) const;

    using edge_output_type = std::pair<GridPoint, GridPoint>;
    using const_iterator   = ConstEdgeIterator;

    const_iterator begin() const;
    const_iterator end() const;

private:
    std::size_t cost = 0; 

    struct coord_t {
        using kind = boost::vertex_property_tag;
    };

    using VertexProperty = boost::property<coord_t, GridPoint>;
    using EdgeProperty   = boost::property<boost::edge_weight_t, std::size_t>; 
    
    using Graph = boost::adjacency_list<boost::listS,
                                        boost::vecS,
                                        boost::undirectedS,
                                        VertexProperty,
                                        EdgeProperty>;
    
    using edge_t   = boost::graph_traits<Graph>::edge_descriptor;  
    using vertex_t = boost::graph_traits<Graph>::vertex_descriptor;

    Graph adjacency_list;
    std::unordered_map<GridPoint, vertex_t, GridPointHasher> vertices;


    class CycleSearchResultHandler
    {
    public:
        void set_result(std::size_t weight, const edge_t& edge)
        {
            max_weight = weight;
            max_weighted_edge = edge;
        }

        auto get_result() const { return std::make_pair(max_weight, max_weighted_edge); }
    
    private:
        std::size_t max_weight = 0;
        edge_t max_weighted_edge;
    };


    class MaxEdgeCycleVisitor : public boost::default_dfs_visitor
    {
    public:
        MaxEdgeCycleVisitor(const GridPoint& target)
            : target(target)
        { }

        void set_result_handler(CycleSearchResultHandler* handler) {
            result_handler = handler;
        }

        void finish_vertex(const vertex_t& vertex, const Graph& graph);
        void tree_edge(const edge_t& edge, const Graph& graph);

    private:
        const GridPoint& target;
        bool is_route_found = false;

        CycleSearchResultHandler* result_handler = nullptr;

        using route_edge_t = std::pair<std::size_t, edge_t>;

        struct RouteEdgeCompare {
            inline bool operator()(const route_edge_t& lhs, const route_edge_t& rhs) const
            {
                return lhs.first < rhs.first;
            }
        };

        std::vector<route_edge_t> route;
    };

    constexpr static const std::size_t NUM_REGIONS = 4;
    std::size_t get_region(const GridPoint& w, const GridPoint& p) const;

    using edge_iterator_t = boost::graph_traits<Graph>::edge_iterator;

    class ConstEdgeIterator
    {
    public:
        using value_type = edge_output_type;
        using different_type = std::ptrdiff_t;
        using const_pointer = const value_type*;
        using const_reference = const value_type&;
        using iterator_category = std::forward_iterator_tag;

    private:
        using iterator = ConstEdgeIterator;
        friend class MinimumSpanningTree;

    public:
        ConstEdgeIterator() = default;

        iterator& operator++() { ++it; return *this; }
        iterator operator++(int) { auto retv = *this; ++(*this); return retv; }
        inline bool operator==(iterator other) const { return it == other.it; }
        inline bool operator!=(iterator other) const { return !(*this == other); }

        edge_output_type operator*() const;

    private:
        ConstEdgeIterator(const Graph* const graph, const edge_iterator_t& it)
            : graph(graph)
            , it(it)
        { }

        const Graph* const graph = nullptr;
        edge_iterator_t it;
    };
};

inline std::ptrdiff_t delta_cost(const MinimumSpanningTree& lhs, const MinimumSpanningTree& rhs)
{
    return lhs.get_cost() - rhs.get_cost();
}

#endif // MINIMUM_SPANNING_TREE_H