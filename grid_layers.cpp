

#include <grid_layers.h>



const std::unordered_map<PointLayer, std::string> point_layer_strings = {
    #define DEFINE_POINT_LAYER(LAYER, STR_LAYER) \
        {PointLayer::LAYER, STR_LAYER},
        
    #include "point_layers.def"

        {PointLayer::MAX_POINT_LAYER, "WRONG_POINT_LAYER"}
        
    #undef DEFINE_POINT_LAYER
};


const std::unordered_map<SegmentLayer, std::string> segment_layer_strings = {
    #define DEFINE_SEGMENT_LAYER(LAYER, STR_LAYER) \
        {SegmentLayer::LAYER, STR_LAYER},
        
    #include "segment_layers.def"

        {SegmentLayer::MAX_SEGMENT_LAYER, "WRONG_SEGMENT_LAYER"}
        
    #undef DEFINE_SEGMENT_LAYER
};
