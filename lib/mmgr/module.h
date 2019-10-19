#pragma once

#include "typedefs.h"
#include "pointer.h"
#include "memory.h"
#include "section.h"

#include <string>
#include <map>

namespace mmgr {

    class module : public memory {
    public:
        module(const std::string &name);

        bool is_valid() const;

        const std::map<std::string, ::mmgr::section>& sections();
        const ::mmgr::section* section(const std::string &name);

        void clean_sections();

        const ::mmgr::section* operator[](const std::string &name);

        const std::string name;
    
    private:
        std::map<std::string, ::mmgr::section> _sections;
    };

}
