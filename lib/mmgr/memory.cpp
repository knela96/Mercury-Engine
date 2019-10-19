#include "memory.h"
#include "module.h"

#include <vector>
#include <algorithm>
#include <system_error>

using namespace mmgr;

const SYSTEM_INFO memory::sys_info = [] {
    SYSTEM_INFO sys_info;
    GetSystemInfo(&sys_info);
    return sys_info;
}(); 

memory::memory() :
    memory(
        sys_info.lpMinimumApplicationAddress,
        sys_info.lpMaximumApplicationAddress
    )
{
}

memory::memory(pointer begin, pointer end, bool continuous) :
    _begin(begin),
    _end(end),
    _continuous(continuous)
{
}

std::vector<memory> memory::regions() const {
    MEMORY_BASIC_INFORMATION mi;
    std::vector<memory> regions;

    auto min_ptr = begin();
    if(min_ptr < sys_info.lpMinimumApplicationAddress)
        min_ptr = sys_info.lpMinimumApplicationAddress;

    auto max_ptr = end();
    if(max_ptr > sys_info.lpMaximumApplicationAddress)
        max_ptr = sys_info.lpMaximumApplicationAddress;

    for(pointer base = min_ptr; base < max_ptr; ) {
        if(!VirtualQuery(base, &mi, sizeof(mi)))
            throw std::system_error(GetLastError(), std::system_category());

        auto base_end = pointer(mi.BaseAddress) + mi.RegionSize;
        if(base_end > max_ptr)
            base_end = max_ptr;

        if(mi.AllocationProtect != 0 &&
           mi.Protect != 0 && mi.Protect != PAGE_NOACCESS && !(mi.Protect & PAGE_GUARD) &&
           mi.State == MEM_COMMIT) {
            regions.emplace_back(base, base_end);
        }

        base = base_end;
    }

    return regions;
}

std::vector<pointer> memory::find(const char *data, size_t length) const {
    std::vector<pointer> matches;

    for(auto &&region : regions()) {
        auto p = region.begin();

        while(true) {
            for(; p < region.end(); ++p)
                if(p.value<char>() == *data)
                    break;
            if(p + length >= region.end()) {
                break;
            }
            if(!memcmp(p + 1, data + 1, length - 1)) {
                matches.emplace_back(p);
            }
            ++p;
        }
    }

    return matches;
}

std::vector<pointer> memory::find(const std::string &str) const {
    return find(str.c_str(), str.length() + 1);
}

std::vector<pointer> memory::find(const std::wstring &str) const {
    return find(reinterpret_cast<const char*>(str.c_str()), (str.length() + 1) * 2);
}

pointer memory::find_single(const char *data, size_t length, pointer start, search_direction dir) const {
    int shift = +1;
    if(dir == backward)
        shift = -1;

    if(start == nullptr) {
        if(dir == forward)
            start = begin();
        else
            start = end();
    }
    
    auto regs = regions();
    if(dir == backward)
        reverse(regs.begin(), regs.end());
    auto region = std::lower_bound(
        regs.begin(),
        regs.end(),
        start,
        [](memory &region, pointer address) -> bool {
            return region.end() <= address;
        }
    );
    if(region == regs.end())
        return nullptr;

    if(dir == forward)
        *region = memory(start, region->end());
    else
        *region = memory(region->begin(), start);

    for(; region != regs.end(); ++region) {
        bool found = true;
        pointer p;
        pointer p_end;
        if(dir == forward) {
            p = region->begin();
            p_end = region->end() - length;
        } else {
            p = region->end() - length;
            p_end = region->begin();
        }

        while(true) {
            for(; p != p_end; p += shift)
                if(p.value<char>() == *data)
                    break;
            if(p == p_end) {
                found = false;
                break;
            }
            if(!memcmp(p + 1, data + 1, length - 1))
                break;
            p += shift;
        }

        if(found)
            return p;
    }

    return nullptr;
}

pointer memory::find_first(const char *data, size_t length) const {
    return find_single(data, length);
}

pointer memory::find_first(const std::string &str) const {
    return find_first(str.c_str(), str.length());
}

pointer memory::find_next(const char *data, size_t length, pointer start) const {
    return find_single(data, length, start, forward);
}

pointer memory::find_next(const std::string &str, pointer start) const {
    return find_next(str.c_str(), str.length(), start);
}

pointer memory::find_prev(const char *data, size_t length, pointer start) const {
    return find_single(data, length, start, backward);
}

pointer memory::find_prev(const std::string &str, pointer start) const {
    return find_prev(str.c_str(), str.length(), start);
}

pointer memory::find_last(const char *data, size_t length) const {
    return find_single(data, length, nullptr, backward);
}

pointer memory::find_last(const std::string &str) const {
    return find_last(str.c_str(), str.length());
}

std::vector<pointer> memory::find_by_pattern(const char *pattern, const char *mask) const {
    size_t length;
    std::vector<pointer> matches;

    // fix dummy mask (if it begins with 00's)
    while(*mask == '\x00' && *pattern != '\x00') {
        mask++;
        pattern++;
    }
    if(*pattern == '\x00' && *mask != '\xFF')
        return matches;
    length = pattern_length(pattern, mask);

    for(auto &&region : regions()) {
        auto p = region.begin();

        while(true) {
            for(; p < region.end(); ++p)
                if(p.value<char>() == *pattern)
                    break;
            if(p + length >= region.end()) {
                break;
            }
            if(pattern_matches(p + 1, pattern + 1, mask + 1)) {
                matches.emplace_back(p);
            }
            ++p;
        }
    }

    return matches;
}

pointer memory::find_single_by_pattern(const char *pattern, const char *mask, pointer start, search_direction dir) const {
    // fix dummy mask (if it begins with 00's)
    while(*mask == '\x00' && *pattern != '\x00') {
        mask++;
        pattern++;
    }
    if(*pattern == '\x00' && *mask != '\xFF')
        return nullptr;
    size_t length = pattern_length(pattern, mask);

    int shift = +1;
    if(dir == backward)
        shift = -1;
    
    if(start == nullptr) {
        if(dir == forward)
            start = begin();
        else
            start = end();
    }

    auto regs = regions();
    if(dir == backward)
        reverse(regs.begin(), regs.end());
    auto region = lower_bound(
        regs.begin(),
        regs.end(),
        start,
        [](memory &region, pointer address) -> bool {
            return region.end() <= address;
        }
    );
    if(region == regs.end())
        return nullptr;

    if(dir == forward)
        *region = memory(start, region->end());
    else
        *region = memory(region->begin(), start);

    for(; region != regs.end(); ++region) {
        bool found = true;
        pointer p;
        pointer p_end;
        if(dir == forward) {
            p = region->begin();
            p_end = region->end() - length;
        } else {
            p = region->end() - length;
            p_end = region->begin();
        }

        while(true) {
            for(; p != p_end; p += shift)
                if(p.value<char>() == *pattern)
                    break;
            if(p == p_end) {
                found = false;
                break;
            }
            if(pattern_matches(p + 1, pattern + 1, mask + 1))
                break;
            p += shift;
        }

        if(found)
            return p;
    }

    return nullptr;
}

pointer memory::find_first_by_pattern(const char *pattern, const char *mask) const {
    return find_single_by_pattern(pattern, mask);
}

pointer memory::find_next_by_pattern(const char *pattern, const char *mask, pointer start) const {
    return find_single_by_pattern(pattern, mask, start);
}
                         
pointer memory::find_prev_by_pattern(const char *pattern, const char *mask, pointer start) const {
    return find_single_by_pattern(pattern, mask, start, backward);
}
                         
pointer memory::find_last_by_pattern(const char *pattern, const char *mask) const {
    return find_single_by_pattern(pattern, mask, nullptr, backward);
}


std::vector<pointer> memory::find_references(pointer ptr) const {
    return find((char*)&ptr, ptr.size());
}

pointer memory::find_first_reference(pointer ptr) const {
    return find_first((char*)&ptr, ptr.size());
}

pointer memory::find_last_reference(pointer ptr) const {
    return find_last((char*)&ptr, ptr.size());
}

std::vector<pointer> memory::find_references(std::vector<pointer> ptrs) const {
    std::vector<pointer> matches;
    for(auto &&ptr : ptrs) {
        auto refs = find_references(ptr);
        matches.insert(
            matches.end(),
            std::make_move_iterator(refs.begin()),
            std::make_move_iterator(refs.end())
        );
    }
    return matches;
}

std::vector<pointer> memory::find_first_reference(std::vector<pointer> ptrs) const {
    std::vector<pointer> matches;
    for(auto &&ptr : ptrs)
        matches.push_back(find_first_reference(ptr));
    return matches;
}

std::vector<pointer> memory::find_last_reference(std::vector<pointer> ptrs) const {
    std::vector<pointer> matches;
    for(auto &&ptr : ptrs)
        matches.push_back(find_last_reference(ptr));
    return matches;
}

std::vector<pointer> memory::find_call_references(pointer func) const {
    const byte asm_instr_call = 0xE8;

    std::vector<pointer> matches;

    for(auto &&region : regions()) {
        auto p = region.begin();

        while(true) {
            for(; p < region.end(); ++p)
                if(p.value<byte>() == asm_instr_call)
                    break;
            if(p == region.end()) {
                break;
            }
            ++p;
            if(p.value<intptr_t>() == func - p - 5 + 1) { // CALL dest - (src + 5)
                matches.emplace_back(p - 1);
            }
        }
    }

    return matches;
}

const std::map<std::string, std::shared_ptr<::mmgr::module>> memory::modules() const {
    //TODO: enumerate all modules, and save into _modules
    return _modules;
}

std::shared_ptr<::mmgr::module> memory::module(const std::string &name) {
    auto mit = _modules.find(name);
    if(mit != _modules.end())
        return mit->second;

    auto m = std::make_shared<::module>(name);
    if(!m->is_valid())
        return nullptr;

    _modules.emplace(name, m);
    return m;
}

void memory::clean_modules() {
    _modules.clear();
}

std::shared_ptr<::mmgr::module> memory::operator[](const std::string &name) {
    return module(name);
}

bool memory::is_valid_address(pointer ptr, size_t size) {
    MEMORY_BASIC_INFORMATION mi;

    if(VirtualQuery(ptr, &mi, sizeof(mi)) == 0)
        return false;

    if(mi.State != MEM_COMMIT)
        return false;

    if(mi.Protect == PAGE_NOACCESS)
        return false;

    auto ptr_end = ptr + size;
    auto reg_end = (uintptr_t)mi.BaseAddress + mi.RegionSize;
    if(ptr_end > reg_end)
        return is_valid_address(reg_end, ptr_end - reg_end);

    return true;
}

DWORD memory::get_protection(pointer ptr) {
    MEMORY_BASIC_INFORMATION mi;
    if (!VirtualQuery(ptr, &mi, sizeof(mi)))
        throw std::system_error(GetLastError(), std::system_category());
    return mi.Protect;
}

size_t memory::pattern_length(const char *pattern, const char *mask) {
    size_t length = 0;
    while(*pattern != '\x00' || *mask != '\x00') {
        length++;
        pattern++;
        mask++;
    }
    return length;
}

bool memory::pattern_matches(const char *data, const char *pattern, const char *mask) {
    for(; *pattern != '\x00' || *mask != '\x00'; data++, pattern++, mask++)
        if((*data & *mask) != (*pattern & *mask))
            return false;
    return true;
}

void memory::redirect_call(pointer dest, pointer src) {
    const byte asm_instr_call = 0xE8;

    if(src.value<byte>() != asm_instr_call) {
        throw std::runtime_error("source is not 'call' instruction");
    }

    ++src;
    src << DWORD(dest - src - 5 + 1);
}
