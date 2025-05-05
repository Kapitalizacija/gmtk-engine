#include "al_buffer.hpp"
#include <iostream>
#include <vector>
#include <miniaudio.h>

namespace GMTKEngine {
    //Stolen code from different wikis and cobbled together - DcruBro
    ALBuffer::ALBuffer() {
        alGenBuffers(1, &mBuffer);
    }

    ALBuffer::~ALBuffer() {
        alDeleteBuffers(1, &mBuffer);
    }

    bool ALBuffer::loadFromFile(const std::string &fileName) {
        ma_decoder decoder;
        ma_decoder_config config = ma_decoder_config_init(ma_format_f32, 0, 0);

        if (ma_decoder_init_file(fileName.c_str(), &config, &decoder) != MA_SUCCESS) {
            ERROR("Failed to initialize decoder for file: " << fileName);
            return false;
        }
        
        ma_uint64 totalFrameCount = 0;
        if (ma_decoder_get_length_in_pcm_frames(&decoder, &totalFrameCount) != MA_SUCCESS || totalFrameCount == 0) {
            ERROR("Failed to get frame length from file: " << fileName);
            ma_decoder_uninit(&decoder);
            return false;
        }
        
        std::vector<float> floatBuffer(totalFrameCount * decoder.outputChannels);
        ma_uint64 framesRead = totalFrameCount;
        if (ma_decoder_read_pcm_frames(&decoder, floatBuffer.data(), totalFrameCount, &framesRead) != MA_SUCCESS) {
            ERROR("Failed to read PCM frames from file: " << fileName);
            ma_decoder_uninit(&decoder);
            return false;
        }
        
        std::vector<short> s16Buffer(framesRead * decoder.outputChannels);
        for (size_t i = 0; i < s16Buffer.size(); ++i) {
            float clamped = std::max(-1.0f, std::min(1.0f, floatBuffer[i]));
            s16Buffer[i] = static_cast<short>(clamped * 32767.0f);
        }
        
        ALenum format;
        if (decoder.outputChannels == 1)
            format = AL_FORMAT_MONO16;
        else if (decoder.outputChannels == 2)
            format = AL_FORMAT_STEREO16;
        else {
            ERROR("Unsupported channel count: " << decoder.outputChannels);
            ma_decoder_uninit(&decoder);
            return false;
        }
        
        alBufferData(mBuffer, format, s16Buffer.data(),
                     static_cast<ALsizei>(s16Buffer.size() * sizeof(short)),
                     decoder.outputSampleRate);
        
        ma_decoder_uninit(&decoder);
        return true;
    }
}