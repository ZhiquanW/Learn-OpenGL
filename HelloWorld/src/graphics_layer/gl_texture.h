//
// Created by zhiquan on 7/20/22.
//

#ifndef HELLOWORLD_GL_TEXTURE_H
#define HELLOWORLD_GL_TEXTURE_H

enum GLTextureType {
    None,
    Texture2D,
    CubeMap
};

class GLTexture {
public:
    unsigned int id = 0;
    GLTextureType type = None;
    unsigned int unit_idx = 0;
    GLTexture() = default;
    GLTexture(unsigned int id, GLTextureType type) :
            id(id),
            type(type) {

    }
};


#endif //HELLOWORLD_GL_TEXTURE_H
