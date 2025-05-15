#pragma once

#include <memory>

//DISABLE_COPY macro
#ifndef DISABLE_COPY
#define DISABLE_COPY(T)										\
			explicit T(const T&) = delete;					\
			T& operator=(const T&) = delete;
#endif

//DISABLE_MOVE macro
#ifndef DISABLE_MOVE
#define DISABLE_MOVE(T)										\
			explicit T(T&&) = delete;						\
			T& operator=(T&&) = delete;
#endif

//DISABLE_COPY_AND_MOVE macro
#ifndef DISABLE_COPY_AND_MOVE
#define DISABLE_COPY_AND_MOVE(T) DISABLE_COPY(T) DISABLE_MOVE(T)
#endif

namespace GMTKEngine {
	template <typename T>
    struct WeakPtrObjectHash {
        std::size_t operator()(const std::weak_ptr<T> weakPtr) const noexcept {
            return std::hash<std::shared_ptr<T>>{}(weakPtr.lock());
        }
    };
    
    template <typename T>
    struct WeakPtrObjectEquals {
        std::size_t operator()(const std::weak_ptr<T> lhs, const std::weak_ptr<T> rhs) const noexcept {
            return !lhs.owner_before(rhs) && !rhs.owner_before(lhs);
        }
    };
}