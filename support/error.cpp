

#include "error.h"

namespace support {

    const std::unordered_map<ErrorType, std::string> error_messages = {
        #define DEFINE_ERROR_TYPE(TYPE, MESSAGE) \
            {ErrorType::TYPE, MESSAGE},
        
        #include "error_types.def"

            {ErrorType::MAX_ERROR_TYPE, "Unknown error"}
        
        #undef DEFINE_ERROR_TYPE
    };
}