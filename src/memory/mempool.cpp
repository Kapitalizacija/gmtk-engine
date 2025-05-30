#include "mempool.hpp"

namespace Sierra::Memory {
	Mempool::Mempool(uint32_t sz) {
		mPool = new uint8_t[sz];
		mCurrentSize = 0;
		mMaxSize = sz;
	}

	Mempool::~Mempool() {
		if (mPool) delete[] mPool;
	}

	uint8_t* Mempool::allocate(uint32_t sz) {
		//TODO: This is inefficient. The current setup will
		//cause the effective available memory to be allocated
		//to decay over time, if continuously smaller amounts
		//of memory are allocated.

		//Check if we can reuse a bound
		for (auto it = mFreedBounds.begin(); it != mFreedBounds.end(); ++it) {
			if (it->second - it->first >= sz) {
				//We can reuse
				auto sec = it->first + sz;
				mBoundPtrs.emplace_back(it->first, sec);
				mFreedBounds.erase(it);
				return it->first;
			}
		}

		
		if (mCurrentSize + sz > mMaxSize) {
			ERROR("This allocation would exceed the max mempool size");
			return nullptr;
		}

		uint8_t *setPtr = mAvailPtr;
		mAvailPtr += sz;
		
		mBoundPtrs.emplace_back(setPtr, mAvailPtr);

		return setPtr;
	}

	bool Mempool::free(uint8_t* ptr, bool overwrite) {
		for (auto it = mBoundPtrs.begin(); it != mBoundPtrs.end(); ++it) {
			if (it->first == ptr) {
				mFreedBounds.emplace_back(it->first, it->second);
				mBoundPtrs.erase(it);
			
				if (overwrite) {
					size_t len = it->second - it->first;
					std::memset(it->first, 0x00, len);
				}
			
				return true;
			}
		}
		
		WARN("Pointer not found in allocations");
		return false;
	}

	std::span<uint8_t> Mempool::access(uint8_t* ptr) {
		for (auto it = mBoundPtrs.begin(); it != mBoundPtrs.end(); ++it) {
			if (it->first == ptr) {
				return std::span<uint8_t>(it->first, it->second - it->first);
			}
		}

		return std::span<uint8_t>(); //Return empty span
	}

	bool Mempool::write(uint8_t* ptr, const uint8_t* src, size_t sz) {
		for (auto it = mBoundPtrs.begin(); it != mBoundPtrs.end(); ++it) {
			if (it->first == ptr) {
				if (!ptr || !src || it->second - it->first > sz) return false;
				
				size_t maxWritable = it->second - it->first;
				size_t copySize = std::min(sz, maxWritable);

				std::memcpy(ptr, src, copySize);
				return true;
			}
		}

		return false;
	}
}
