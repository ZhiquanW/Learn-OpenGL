//
// Created by zhiquan on 6/23/22.
//

#ifndef HELLOWORLD_DAWN_TEXTURE_H
#define HELLOWORLD_DAWN_TEXTURE_H

#include "common_includes.h"

namespace dawn_engine {


    class DawnTexture {
    protected:
    private:
        std::string name = "New Texture";
        int width = 0;
        int height = 0;
        int channel = 0;
        unsigned char *data = nullptr;
    public:
        explicit DawnTexture(const std::string &path, bool flip = false);

        ~DawnTexture();

        [[nodiscard]] std::string GetName() const;

        [[nodiscard]] int getWidth() const;

        [[nodiscard]] int getHeight() const;

        [[nodiscard]] int getChannel() const;

        [[nodiscard]] unsigned char *GetData() const;


    };

}

#endif //HELLOWORLD_DAWN_TEXTURE_H
