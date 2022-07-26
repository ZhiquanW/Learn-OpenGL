//
// Created by zhiquan on 7/20/22.
//

#ifndef HELLOWORLD_GL_TEXTURE_H
#define HELLOWORLD_GL_TEXTURE_H

enum GLTextureTarget {
    Texture2D = GL_TEXTURE_2D,
    CubeMap = GL_TEXTURE_CUBE_MAP,
};
enum GLTextureAttachment{
    ColorAttachment = GL_COLOR_ATTACHMENT0,
    DepthAttachment = GL_DEPTH_ATTACHMENT,
};

class GLTexture {
public:
    GLTextureTarget target = GLTextureTarget::Texture2D;
    GLTextureAttachment attachment = GLTextureAttachment::ColorAttachment;
    unsigned int id = 0;
    glm::vec2 resolution{};

    GLTexture() = default;

    GLTexture(GLTextureTarget target, GLTextureAttachment attachment, unsigned int id, glm::vec2 resolution = glm::vec2(4096)) :
            id(id),
            target(target),
            attachment(attachment),
            resolution(resolution) {
    }

};


#endif //HELLOWORLD_GL_TEXTURE_H
