#pragma once

#include "typedefs.h"
#include "pointer.h"

#include <Windows.h>

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace mmgr {

    class module;

    class memory {
    public:
        enum search_direction {
            forward,
            backward,
        };

        memory();
        memory(pointer begin, pointer end, bool continuous = false);

        std::vector<memory> regions() const;

        inline pointer begin() const { return _begin; }
        inline pointer end() const { return _end; }
        inline bool continuous() const { return _continuous; }
        inline bool has(pointer address) const { return _begin <= address && address < _end; }

        std::vector<pointer> find(const char *data, size_t length) const;
        std::vector<pointer> find(const std::string &str) const;
        std::vector<pointer> find(const std::wstring &str) const;

        pointer find_single(const char *data, size_t length, pointer start = nullptr, search_direction dir = forward) const;

        pointer find_first(const char *data, size_t length) const;
        pointer find_first(const std::string &str) const;

        pointer find_next(const char *data, size_t length, pointer start) const;
        pointer find_next(const std::string &str, pointer start) const;

        pointer find_prev(const char *data, size_t length, pointer start) const;
        pointer find_prev(const std::string &str, pointer start) const;

        pointer find_last(const char *data, size_t length) const;
        pointer find_last(const std::string &str) const;

        std::vector<pointer> find_by_pattern(const char *pattern, const char *mask) const;

        pointer find_single_by_pattern(const char *pattern, const char *mask, pointer start = nullptr, search_direction dir = forward) const;

        pointer find_first_by_pattern(const char *pattern, const char *mask) const;
        pointer find_next_by_pattern(const char *pattern, const char *mask, pointer start) const;
        pointer find_prev_by_pattern(const char *pattern, const char *mask, pointer start) const;
        pointer find_last_by_pattern(const char *pattern, const char *mask) const;

        std::vector<pointer> find_references(pointer ptr) const;
        pointer find_first_reference(pointer ptr) const;
        pointer find_last_reference(pointer ptr) const;

        std::vector<pointer> find_references(std::vector<pointer> ptrs) const;
        std::vector<pointer> find_first_reference(std::vector<pointer> ptrs) const;
        std::vector<pointer> find_last_reference(std::vector<pointer> ptrs) const;

        std::vector<pointer> find_call_references(pointer func) const;

        const std::map<std::string, std::shared_ptr<::mmgr::module>> modules() const;
        std::shared_ptr<::mmgr::module> module(const std::string &name);

        void clean_modules();

        std::shared_ptr<::mmgr::module> operator[](const std::string &name);

        static bool is_valid_address(pointer ptr, size_t size = sizeof(pointer));
        static DWORD get_protection(pointer ptr);

        static size_t pattern_length(const char *pattern, const char *mask);
        static bool pattern_matches(const char *data, const char *pattern, const char *mask);

        static void redirect_call(pointer dest, pointer src);

    protected:
        pointer _begin;
        pointer _end;
        bool _continuous;

    private:
        static const SYSTEM_INFO sys_info;

        std::map<std::string, std::shared_ptr<::mmgr::module>> _modules;
    };

}
