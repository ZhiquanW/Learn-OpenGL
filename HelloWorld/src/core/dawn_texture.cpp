//
// Created by zhiquan on 6/23/22.
//

#include "dawn_texture.h"
#include "stb_image.h"

namespace dawn_engine {
    DawnTexture::DawnTexture(const std::string &path, bool flip) {
        stbi_set_flip_vertically_on_load(flip);
        this->data = stbi_load(path.c_str(), &this->width, &this->height, &this->channel, 0);
        if (!data) {
            std::cout << "Texture failed to load at path: " << path << std::endl;
        }
        this->name = std::filesystem::path(path).stem();
    }

    DawnTexture::~DawnTexture() {
        stbi_image_free(this->data);
    }

    int DawnTexture::getWidth() const {
        return this->width;
    }

    int DawnTexture::getHeight() const {
        return this->height;
    }

    int DawnTexture::getChannel() const {
        return this->channel;
    }

    unsigned char *DawnTexture::GetData() const {
        return this->data;
    }

    std::string DawnTexture::GetName() const {
        return this->name;
    }

}
