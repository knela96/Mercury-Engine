#pragma once

#include <Windows.h>

#include "typedefs.h"

#include <type_traits>
#include <stdexcept>

namespace mmgr {

    /*
     * WARNING:
     * Do not add any virtual methods to `pointer` class.
     * Any arbitrary value of class `pointer`
     * MUST have a value of `pointer::ptr` as its first bytes.
     */

    // A wrapper for for convinient pointer manipulations
    class pointer {
    public:
        pointer(uintptr_t pointer = 0);
        pointer(void* pointer);

        DWORD protect(size_t size, DWORD new_prot, DWORD *old_prot = nullptr);
        DWORD get_protection() const;

        bool is_valid() const;

        template<
            typename T,
            typename = typename std::enable_if_t<std::is_integral_v<T>, T>
        >
            inline pointer operator+(T offset) const { return ptr + offset; }

        template<
            typename T,
            typename = typename std::enable_if_t<std::is_integral_v<T>, T>
        >
            inline pointer operator-(T offset) const { return ptr - offset; }

        template<
            typename T,
            typename = typename std::enable_if_t<std::is_integral_v<T>, T>
        >
            inline pointer operator+=(T offset) { return ptr += offset; }

        template<
            typename T,
            typename = typename std::enable_if_t<std::is_integral_v<T>, T>
        >
            inline pointer operator-=(T offset) { return ptr -= offset; }

        inline pointer operator--() { return --ptr; }
        inline pointer operator--(int) { return ptr--; }
        inline pointer operator++() { return ++ptr; }
        inline pointer operator++(int) { return ptr++; }

        inline bool operator==(nullptr_t rhs) const { return ptr == 0; }

        template<
            typename T,
            typename = typename std::enable_if_t<std::is_integral_v<T> || std::is_same_v<T, pointer>, T>
        >
            inline bool operator==(T rhs) const { return ptr == rhs; }

        template<
            typename T,
            typename = typename std::enable_if_t<std::is_integral_v<T> || std::is_same_v<T, pointer>, T>
        >
            inline bool operator!=(T rhs) const { return ptr != rhs; }

        template<
            typename T,
            typename = typename std::enable_if_t<std::is_integral_v<T> || std::is_same_v<T, pointer>, T>
        >
            inline bool operator<(T rhs) const { return ptr < rhs; }

        template<
            typename T,
            typename = typename std::enable_if_t<std::is_integral_v<T> || std::is_same_v<T, pointer>::value, T>
        >
            inline bool operator>(T rhs) const { return ptr > rhs; }

        template<
            typename T,
            typename = typename std::enable_if_t<std::is_integral_v<T> || std::is_same<T, pointer>, T>
        >
            inline bool operator<=(T rhs) const { return ptr <= rhs; }

        template<
            typename T,
            typename = typename std::enable_if_t<std::is_integral_v<T> || std::is_same<T, pointer>, T>
        >
            inline bool operator>=(T rhs) const { return ptr >= rhs; }

        pointer operator*() const;

        inline operator uintptr_t() const { return ptr; }

        template<typename T>
        inline operator T*() const { return (T*)ptr; }

        template<typename T>
        inline pointer operator<<(const T &src) {
            auto old_prot = protect(sizeof(T), PAGE_EXECUTE_READWRITE);
            *(T*)ptr = src;
            protect(sizeof(T), old_prot);
            return *this + sizeof(T);
        }

        template<typename T>
        inline pointer operator>>(const T &dest) {
            memcpy((void*)&dest, (void*)ptr, sizeof(T));
            return *this + sizeof(T);
        }

        template<typename T>
        inline T& value() const { return *(T*)ptr; }

        static constexpr size_t size() { return sizeof(uintptr_t); }

    private:
        uintptr_t ptr;
    };

    template<>
    inline pointer::operator void*() const {
        return (void*)ptr;
    }

}
