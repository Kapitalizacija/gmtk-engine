#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <string>

namespace GMTKEngine {
    //Handler for the OpenAL device, via which audio buffers are played.
    class ALDevice {
        public:
            ALDevice(const std::string &deviceName = "");
            ~ALDevice();

            bool isValid() const;
        
        private:
            ALCdevice *mDevice;
            ALCcontext *mContext;
    };
}