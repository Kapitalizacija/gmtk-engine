template<typename T>
GLBuffer::GLBuffer(Type buffer_type, T data, size_t size, Usage buffer_usage): size(0), type(buffer_type) {
    if (buffer_type == Type::UNDEFINED) {
        WARN("Tried to create a buffer of undefined type");
    }
    
    this->size = size;
    create_buffer();
    matchType(buffer_type);
    uploadData(data, size, buffer_usage);
}

template<typename T>
void GLBuffer::uploadData(T data, size_t size, Usage buffer_usage) {
    static_assert(std::is_pointer<T>::value);

    if ( !assert_valid() ) {
        return;
    }

    this->size = size;

    GLuint usage;
    switch (buffer_usage) {
        case Usage::STREAM:
            usage = GL_STREAM_DRAW;
            break;
        case Usage::OFTEN:
            usage = GL_DYNAMIC_DRAW;
            break;
        case Usage::RARELY:
            usage = GL_STATIC_DRAW;
            break;
    }

    glBindBuffer(glType, buff);
    glBufferData(glType, size, data, usage);
    glBindBuffer(glType, 0);
}
    
