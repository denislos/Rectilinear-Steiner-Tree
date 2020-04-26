

#ifndef RECT_STEINER_TREE_FACTORY_H
#define RECT_STEINER_TREE_FACTORY_H

#include <string>   


#include <rect_steiner_tree.h>


struct RectSteinerTreeFactory
{
    RectSteinerTree* construct_from_file(const std::string& input_file);
};


#endif // RECT_STEINER_TREE_FACTORY_H