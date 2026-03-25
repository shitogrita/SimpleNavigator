#pragma once

#include <memory>

template <typename T>
class MyAllocator {
public:
	using allcator_type = MyAllocator;
	using value_type = T;
	using reference = T&;
	using pointer = T*;
	using const_pointer = const T*;
	using const_reference = const T&;
	using size_type = size_t;


	MyAllocator() noexcept = default;

	template <typename U>
	MyAllocator(const MyAllocator<U>&) noexcept {}

	T* allocate(std::size_t count) {
		return static_cast<T*>(::operator new(count * sizeof(T)));
	}

	void deallocate(T* ptr, std::size_t /* count */) {
		::operator delete(ptr);
	}

	template <typename U, typename... Args>
	void construct(U* ptr, Args&&... args) {
		new(ptr) U(std::forward<Args>(args)...);
	}

	template <typename U>
	void destroy(U* ptr) {
		ptr->~U();
	}
};

template <typename T, typename U>
bool operator==(const MyAllocator<T>&, const MyAllocator<U>&) { return true; }

template <typename T, typename U>
bool operator!=(const MyAllocator<T>&, const MyAllocator<U>&) { return false; }