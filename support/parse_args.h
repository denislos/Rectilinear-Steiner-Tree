

#ifndef SUPPORT_PARSE_ARGS_H
#define SUPPORT_PARSE_ARGS_H

#include "error.h"

namespace support {
    
    static inline void check_argument_number(int argc, int num_passed_arguments)
    {
        if (argc != num_passed_arguments + 1)
            raise_error(ErrorType::NOT_ENOUGH_ARGUMENTS_PASSED);
    }

} // namespace support

#endif // SUPPORT_PARSE_ARGS_H