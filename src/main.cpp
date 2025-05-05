#include <iostream>
#include <stdfloat>
#include <chrono>

#include "window/window.hpp"
#include "scene/scene.hpp"
#include "renderer/util/buffer/gl_buffer.hpp"
#include "renderer/util/shader/gl_shader.hpp"
#include "renderer/util/vao/gl_vao.hpp"
#include "renderer/texture/gl_texture.hpp"

#include "scene/object/components/transform.hpp"

#include "audio/al_device.hpp"
#include "audio/al_buffer.hpp"
#include "audio/al_sound.hpp"
#include "audio/al_listener.hpp"

#include <cmath>
#include <thread>

using namespace GMTKEngine;

int main() {
    Window window = Window("GMTKEngine", {1280, 720});

    Scene manager = Scene();
    GLTexture tex = GLTexture();

    std::float32_t verts[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f
    };

    uint32_t indices[] = {
        0, 1, 2,
        1, 2, 3
    };


    GLBuffer buff = GLBuffer((uint8_t*)verts, sizeof(verts), GLBuffer::VERTEX, GLBuffer::RARELY);
    GLBuffer index_buff = GLBuffer((uint8_t*)indices, sizeof(indices), GLBuffer::INDEX, GLBuffer::RARELY);

    GLAttribPointer attributes[] = {
        {&buff, 0, 2, GL_FLOAT, 8, 0}

    };

    GLShader shader = GLShader("test_shader", "test_shaders/tri.vert", "test_shaders/tri.frag"); // niggas in paris
    GLVAO vao = GLVAO(attributes, 1);

    //Test sound or smth
    ALDevice audio;
    if (!audio.isValid()) {
        ERROR("OpenAL Device Error, exiting");
        return 1;
    }

    ALBuffer buffer;
    if (buffer.loadFromFile("file_example_WAV_10MG.wav")) {
        //TODO: Actually make this shit work :(
        ALSound sound(buffer);
        sound.setPosition(glm::vec3(0.f, 0.f, 0.f));
        sound.setLooping(true);
        sound.setGain(100.f);
        alListenerf(AL_GAIN, 100.0f);
        alSourcei(sound.getSourceID(), AL_SOURCE_RELATIVE, AL_TRUE); // Disable 3D attenuation
        alSource3f(sound.getSourceID(), AL_POSITION, 0.0f, 0.0f, 0.0f);
        alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
        alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
        float ori[] = {0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f};
        alListenerfv(AL_ORIENTATION, ori);

        ALListener::setPosition(glm::vec3(0.f, 0.f, 0.f));
        ALListener::setOrientation(glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f));

        sound.play();

        ALint state;
        alGetSourcei(sound.getSourceID(), AL_SOURCE_STATE, &state);
        if (state != AL_PLAYING) {
            ERROR("Sound is not playing! AL state = " + state);
        } else {
            LOG("Sound playing!");
        }

        LOG("Played sound!");
    } else {
        DBG("Loading sound into buffer failed");
    }

    /*ALCdevice* device = alcOpenDevice(nullptr);
    if (!device) {
        ERROR("Failed to open OpenAL device");
        return 1;
    }

    ALCcontext* context = alcCreateContext(device, nullptr);
    if (!context || !alcMakeContextCurrent(context)) {
        std::cerr << "Failed to create OpenAL context\n";
        alcCloseDevice(device);
        return 1;
    }

    std::cout << "Using OpenAL device: " << alcGetString(device, ALC_DEVICE_SPECIFIER) << "\n";

    // 2. Generate sine wave PCM data
    constexpr int sampleRate = 44100;
    constexpr float durationSec = 2.0f;
    constexpr float frequency = 440.0f; // A4
    constexpr int samples = static_cast<int>(sampleRate * durationSec);

    std::vector<short> pcm(samples);
    for (int i = 0; i < samples; ++i) {
        float t = static_cast<float>(i) / sampleRate;
        pcm[i] = static_cast<short>(std::sin(2.0f * M_PI * frequency * t) * 32767.0f);
    }

    // 3. Create buffer and load data
    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, AL_FORMAT_MONO16, pcm.data(), static_cast<ALsizei>(pcm.size() * sizeof(short)), sampleRate);

    // 4. Create source and play
    ALuint source;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    alSourcef(source, AL_GAIN, 1.0f);
    alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);

    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    float orientation[] = {0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f};
    alListenerfv(AL_ORIENTATION, orientation);

    alSourcePlay(source);

    std::cout << "Playing 2-second sine wave (440 Hz)...\n";

    std::this_thread::sleep_for(std::chrono::seconds(2));

    // 5. Cleanup
    alSourceStop(source);
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);*/

    while ( !window.should_close() ) {
        glBindVertexArray(vao.get_vao());
        glUseProgram(shader.get_program());

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buff.get_buffer());

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        window.update();
    }

}
