//
// Created by zhiquan on 6/23/22.
//

#ifndef HELLOWORLD_GL_TRANSFORMER_H
#define HELLOWORLD_GL_TRANSFORMER_H

#include "core/dawn_texture.h"


inline unsigned int texture2GL(const dawn_engine::DawnTexture *dTexture) {
    unsigned int textureID;
    if (dTexture->GetData()) {
        GLenum format;
        if (dTexture->getChannel() == 1) {
            format = GL_RED;
        } else if (dTexture->getChannel() == 3) {
            format = GL_RGB;
        } else if (dTexture->getChannel() == 4) {
            format = GL_RGBA;
        }
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, (int) format, dTexture->getWidth(), dTexture->getHeight(), 0, format, GL_UNSIGNED_BYTE, dTexture->GetData());
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        std::cout << fmt::format("Failed to transform Dawn Texture Data {} to GPU Memory (OpenGL)", dTexture->GetName()) << std::endl;
    }
    return textureID;
}

#endif //HELLOWORLD_GL_TRANSFORMER_H
