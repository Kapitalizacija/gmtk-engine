#include "al_buffer.hpp"

namespace GMTKEngine {
    //Stolen code from different wikis and cobbled together - DcruBro
    ALBuffer::ALBuffer() {
        alGenBuffers(1, &mBuffer);
    }

    ALBuffer::~ALBuffer() {
        alDeleteBuffers(1, &mBuffer);
    }

    bool ALBuffer::loadFromFile(const std::string &fileName) {
        ma_result res;
        ma_decoder dec;
        ma_decoder_config conf = ma_decoder_config_init(ma_format_f32, 0, 0);

        res = ma_decoder_init_file(fileName.c_str(), &conf, &dec);
        if (res != MA_SUCCESS) {
            ERROR("Failed to load audio file " + fileName);
            return false;
        }

        ma_uint64 frameCount = 0;
        res = ma_decoder_get_length_in_pcm_frames(&dec, &frameCount);
        if (res != MA_SUCCESS || frameCount == 0) {
            ERROR("Failed to get audio length");
            ma_decoder_uninit(&dec);
            return false;
        }

        std::vector<float> f32Data(frameCount * dec.outputChannels);
        ma_uint64 framesRead = frameCount;
        res = ma_decoder_read_pcm_frames(&dec, f32Data.data(), frameCount, &framesRead);
        if (res != MA_SUCCESS || framesRead == 0) {
            ERROR("Failed to read audio data");
            ma_decoder_uninit(&dec);
            return false;
        }

        // Convert float to signed 16-bit PCM
        std::vector<short> s16Data(framesRead * dec.outputChannels);
        for (size_t i = 0; i < s16Data.size(); ++i) {
            float sample = f32Data[i];
            if (sample > 1.0f) sample = 1.0f;
            if (sample < -1.0f) sample = -1.0f;
            s16Data[i] = static_cast<short>(sample * 32767.0f);
        }

        ALenum format;
        if (dec.outputChannels == 1)
            format = AL_FORMAT_MONO16;
        else if (dec.outputChannels == 2)
            format = AL_FORMAT_STEREO16;
        else {
            ERROR("Unsupported channel count: " + dec.outputChannels);
            ma_decoder_uninit(&dec);
            return false;
        }

        alBufferData(mBuffer, format, s16Data.data(),
                     static_cast<ALsizei>(s16Data.size() * sizeof(short)),
                     dec.outputSampleRate);

        ma_decoder_uninit(&dec);
        return true;
    }
}