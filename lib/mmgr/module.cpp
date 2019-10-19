#include "module.h"

#include <Windows.h>
#include <Psapi.h>

using namespace mmgr;

module::module(const std::string &name) :
    name(name)
{
    MODULEINFO info;

    auto hModule = GetModuleHandleA(name.c_str());
    if (!hModule) {
        _begin = nullptr;
        _end = nullptr;
        _continuous = false;
        return;
    }
    auto result = GetModuleInformation(
        GetCurrentProcess(),
        hModule,
        &info,
        sizeof(info));

    if(!result) {
        _begin = nullptr;
        _end = nullptr;
        _continuous = false;
        return;
    }

    _begin = info.lpBaseOfDll;
    _end = _begin + info.SizeOfImage;
    _continuous = true;
}

bool module::is_valid() const {
    return begin() != 0 && end() != 0;
}

const std::map<std::string, ::mmgr::section>& module::sections() {
    if(_sections.size() == 0) {
        IMAGE_DOS_HEADER *dosHeader = begin();
        if(!memory::is_valid_address(dosHeader, sizeof(IMAGE_DOS_HEADER)))
            return _sections;

        IMAGE_NT_HEADERS *ntHeaders = begin() + dosHeader->e_lfanew;
        if(!memory::is_valid_address(ntHeaders, sizeof(IMAGE_NT_HEADERS)))
            return _sections;

        IMAGE_SECTION_HEADER *sections = IMAGE_FIRST_SECTION(ntHeaders);
        if(!memory::is_valid_address(sections, sizeof(IMAGE_SECTION_HEADER) * ntHeaders->FileHeader.NumberOfSections))
            return _sections;

        for(int i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++) {
            auto &header = sections[i];
            auto s = ::mmgr::section(begin(), header);
            _sections.emplace(s.name, std::move(s));
        }
    }
    return _sections;
}

const ::mmgr::section* module::section(const std::string &name) {
    if(_sections.size() == 0)
        sections(); // sections must be cached before searching any.
    auto sit = _sections.find(name);
    if(sit != _sections.end())
        return &sit->second;
    return nullptr;
}

void module::clean_sections() {
    _sections.clear();
}

const ::mmgr::section* module::operator[](const std::string &name) {
    return section(name);
}
