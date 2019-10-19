#pragma once

#include <Windows.h>

#include "typedefs.h"
#include "pointer.h"
#include "memory.h"

#include <string>

namespace mmgr {

    class section : public memory {
    public:
        section(pointer module_base, const IMAGE_SECTION_HEADER &header);

        const IMAGE_SECTION_HEADER &header;
        const std::string name;
    };

}
