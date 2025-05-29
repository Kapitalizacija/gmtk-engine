#include "delta_time.hpp"

namespace Sierra {
    void DelaTime::update() {
        if (!mLast)
            mLast = CLOCK_MS;

        mDt = (CLOCK_MS - mLast) / 1000.0f;

        mDt = std::min(mDt, MAX_DT);
    }

    float DelaTime::get() {
        return mDt;
    }

}