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

        [[maybe_unused]]  void SetPosition(const glm::vec3 &pos);

        [[maybe_unused]] [[nodiscard]] glm::vec3 GetPosition() const;

        [[maybe_unused]] [[nodiscard]] glm::vec3 &getPositionMeta();

        [[maybe_unused]]  void setRotation(const glm::vec3 &rot);

        [[maybe_unused]] [[nodiscard]] glm::vec3 GetRotation() const;

        [[maybe_unused]] [[nodiscard]] glm::vec3 &getRotationMeta();

        [[maybe_unused]] void SetScale(const glm::vec3 &s);

        [[maybe_unused]] [[nodiscard]] glm::vec3 getScale() const;

        [[maybe_unused]] [[nodiscard]] glm::vec3 &getScaleMeta();

        [[maybe_unused]] [[nodiscard]] glm::mat4 GetTranslationMat() const;

        [[maybe_unused]] [[nodiscard]] glm::mat4 GetScaleMat() const;

        [[maybe_unused]] [[nodiscard]] glm::mat4 GetRotationMat() const;

        [[maybe_unused]] [[nodiscard]] glm::mat4 GetModelMat4() const;

        glm::mat4 GetTransformMat() const;
    };

} // namespace dawn_engine