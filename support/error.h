

#ifndef SUPPORT_ERROR_H
#define SUPPORT_ERROR_H

#include <unordered_map>
#include <string>
#include <iostream>


namespace support {
    
    enum class ErrorType {
        #define DEFINE_ERROR_TYPE(TYPE, MESSAGE) \
            TYPE,
    
        #include "error_types.def"

            MAX_ERROR_TYPE
    
        #undef DEFINE_ERROR_TYPE
    };

    extern const std::unordered_map<ErrorType, std::string> error_messages;

    static inline void raise_error(ErrorType error_type)
    {
        std::cerr << error_messages.at(error_type) << std::endl;
        std::exit(EXIT_FAILURE);
    }

} // namespace support

#endif // SUPPORT_ERROR_H