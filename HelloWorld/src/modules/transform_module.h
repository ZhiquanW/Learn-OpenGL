#pragma once

#include "base_module.h"

namespace dawn_engine {
    class TransformModule : public BaseModule {

    protected:
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

    public:
        static const std::size_t type;

        TransformModule();

        ~TransformModule() override;

        TransformModule(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

        [[maybe_unused]]  void setPosition(const glm::vec3 &pos);

        [[maybe_unused]] [[nodiscard]] glm::vec3 getPosition() const;

        [[maybe_unused]] [[nodiscard]] glm::vec3 &getPositionMeta();

        [[maybe_unused]]  void setRotation(const glm::vec3 &rot);

        [[maybe_unused]] [[nodiscard]] glm::vec3 getRotation() const;

        [[maybe_unused]] [[nodiscard]] glm::vec3 &getRotationMeta();

        [[maybe_unused]] void setScale(const glm::vec3 &s);

        [[maybe_unused]] [[nodiscard]] glm::vec3 getScale() const;

        [[maybe_unused]] [[nodiscard]] glm::vec3 &getScaleMeta();

        [[maybe_unused]] [[nodiscard]] glm::mat4 getTranslationMat4() const;

        [[maybe_unused]] [[nodiscard]] glm::mat4 getScaleMat4() const;

        [[maybe_unused]] [[nodiscard]] glm::mat4 getRotationMat4() const;

        [[maybe_unused]] [[nodiscard]] glm::mat4 getModelMat4() const;
    };

} // namespace dawn_engine