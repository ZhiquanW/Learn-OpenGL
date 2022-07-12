#pragma once

#include "base_module.h"

#include "../../include/common_includes.h"
#include "../../include/shader_uniform_variable.h"
#include "transform_module.h"
#include "core/game_object.h"

namespace dawn_engine {
    class LightModule : public BaseModule {

    private:
    protected:
        glm::vec3 ambient_;
        glm::vec3 diffuse_;
        glm::vec3 specular_;
    public:
        static const std::size_t type;


        LightModule();

        explicit LightModule(glm::vec3 color);

        LightModule(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

        ~LightModule() override;

        [[nodiscard]] glm::vec3 GetAmbient() const;


        glm::vec3 &GetAmbientMeta();

        void SetAmbient(glm::vec3 ambient);

        [[nodiscard]] glm::vec3 GetDiffuse() const;

        glm::vec3 &GetDiffuseMeta();

        void SetDiffuse(glm::vec3 diffuse);

        [[nodiscard]] glm::vec3 GetSpecular() const;

        glm::vec3 &GetSpecularMeta();

        void SetSpecular(glm::vec3 specular);

        [[nodiscard]] virtual std::vector<std::shared_ptr<ShaderUniformVariableBase>>
        getUniforms(const std::string &name) const;
    };

    class DirectionalLightModule : public LightModule {

    private:

    protected:
    public:
        glm::vec3 direction;

        static const std::size_t type;

        DirectionalLightModule();

        explicit DirectionalLightModule(glm::vec3 dir);

        DirectionalLightModule(glm::vec3 dir, glm::vec3 color);

        DirectionalLightModule(glm::vec3 dir, glm::vec3 ambient, glm::vec3 diffuse,
                               glm::vec3 specular);

        ~DirectionalLightModule() override;


        [[maybe_unused]] [[nodiscard]] virtual std::vector<std::shared_ptr<ShaderUniformVariableBase>>
        getUniforms(uint32_t idx) const;

        [[maybe_unused]] [[nodiscard]] glm::vec3 GetDirection() const;

        glm::vec3 &getDirectionMeta();

        void setDirection(const glm::vec3 &dir);
    };

    class PointLightModule : public LightModule {

    private:
    protected:
    public:
        static const std::size_t type;

        float constant;

        float linear;

        float quadratic;

        PointLightModule();

        PointLightModule(glm::vec3 color, float constant, float linear, float quadratic);

        PointLightModule(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                         float constant, float linear,
                         float quadratic);

        [[maybe_unused]] [[nodiscard]] virtual std::vector<std::shared_ptr<ShaderUniformVariableBase>>
        getUniforms(uint32_t idx) const;

        float getConstant() const;

        float &getConstantMeta();

        void setConstant(const float &c);

        float getLinear() const;

        float &getLinearMeta();

        void setLinear(const float &l);

        float getQuadratic() const;

        float &getQuadraticMeta();

        void setQuadratic(const float &q);
    };

    class SpotLightModule : public PointLightModule {
    private:
    protected:

    public:
        glm::vec3 direction;
        float innerRange;//in degree
        float outerRange;
        static const std::size_t type;

        SpotLightModule();

        SpotLightModule(glm::vec3 dir, float innerRange, float outerRange);

        SpotLightModule(glm::vec3 dir, float innerRange, float outerRange, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

        [[maybe_unused]] [[nodiscard]] glm::vec3 getDirection() const;

        glm::vec3 &getDirectionMeta();

        void setDirection(const glm::vec3 &dir);

        float getOuterRange() const;

        float &getOuterRangeMeta();

        void setOuterRange(const float &range);

        [[maybe_unused]] [[nodiscard]] float getInnerRange() const;

        [[maybe_unused]] [[nodiscard]] float &getInnerRangeMeta();

        [[maybe_unused]]  void setInnerRange(const float &range);


        [[maybe_unused]] [[nodiscard]] std::vector<std::shared_ptr<ShaderUniformVariableBase>>
        getUniforms(uint32_t idx) const override;


    };
} // namespace dawn_engine
