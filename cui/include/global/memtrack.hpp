#ifndef CUI_MEMTRACK_HPP
#define CUI_MEMTRACK_HPP

#include <global/memcounter.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <new>

void* operator new(std::size_t sz) {
	cui::memcounter += sz;
	auto ptr = (size_t*)std::malloc(sz + sizeof(std::size_t));
	ptr[0] = sz;
	return (void*)(ptr + 1);
}

void* operator new[](std::size_t sz) {
	cui::memcounter += sz;
	auto ptr = (size_t*)std::malloc(sz + sizeof(std::size_t));
	ptr[0] = sz;
	return (void*)(ptr + 1);
}

void operator delete(void* ptr) noexcept {
	auto full_ptr = (size_t*)ptr;
	cui::memcounter -= full_ptr[-1];
	std::free((void*)(full_ptr - 1));
}

void operator delete[](void* ptr) noexcept {
	auto full_ptr = (size_t*)ptr;
	cui::memcounter -= full_ptr[-1];
	std::free((void*)(full_ptr - 1));
}

void operator delete(void* ptr, std::size_t sz) noexcept {
	auto full_ptr = (size_t*)ptr;
	cui::memcounter -= sz;
	std::free((void*)(full_ptr - 1));
}

void operator delete[](void* ptr, std::size_t sz) noexcept {
	auto full_ptr = (size_t*)ptr;
	cui::memcounter -= sz;
	std::free((void*)(full_ptr - 1));
}

#endif	  // CUI_MEMTRACK_HPP