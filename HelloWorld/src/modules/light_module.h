#pragma once

#include "common_includes.h"
#include "base_module.h"

#include "shader_uniform_variable.h"
#include "transform_module.h"
#include "graphics_layer/gl_texture.h"
#include "graphics_layer/gl_allocator.h"

namespace dawn_engine {
    enum LightType {
        AbstractLight,
        DirectionalLight,
        PointLight,
        SpotLight,
    };

    class LightModule : public BaseModule {

    private:
    protected:
        // todo: move below to GLTexture and delete depracted part
        const glm::vec2 DEFAULT_DEPTH_MAP_SIZE{4096};
        glm::vec3 ambient_;
        glm::vec3 diffuse_;
        glm::vec3 specular_;
        GLTexture depth_map_ = {};
        LightType light_type_ = LightType::AbstractLight;
    public:
        static const std::size_t type;


        explicit LightModule(LightType light_type);

        explicit LightModule(LightType light_type, glm::vec3 color);

        LightModule(LightType light_type, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

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

        GLTexture GetShadowMapTexture() const;

        LightType GetLightType() const;

        virtual glm::mat4 GetLightSpaceTransformMat() const = 0;
    };

    class DirectionalLightModule : public LightModule {

    private:

    protected:
        glm::vec3 direction_ = {1.0f, 0.0f, 0.0f};
        float box_range_[4] = {-10, 10, -10, 10}; // left,right,bottom,right
        float z_range_[2] = {0.001f, 1000.0f};
    public:

        static const std::size_t type;

        DirectionalLightModule();

        explicit DirectionalLightModule(glm::vec3 dir);

        DirectionalLightModule(glm::vec3 dir, glm::vec3 color);

        DirectionalLightModule(glm::vec3 dir, glm::vec3 ambient, glm::vec3 diffuse,
                               glm::vec3 specular);

        explicit DirectionalLightModule(glm::vec3 dir, float left, float right, float bottom, float top, float z_near,
                                        float z_far);

        DirectionalLightModule(glm::vec3 dir, float left, float right, float bottom, float top, float z_near,
                               float z_far, glm::vec3 color);

        DirectionalLightModule(glm::vec3 dir, float left, float right, float bottom, float top, float z_near,
                               float z_far, glm::vec3 ambient, glm::vec3 diffuse,
                               glm::vec3 specular);

        ~DirectionalLightModule() override;


        [[maybe_unused]] [[nodiscard]] virtual std::vector<std::shared_ptr<ShaderUniformVariableBase>>
        getUniforms(uint32_t idx) const;

        [[maybe_unused]] [[nodiscard]] glm::vec3 GetDirection() const;

        glm::mat4 GetLightSpaceTransformMat() const override;

        glm::vec3 GetTransformedDirection() const;

        glm::vec3 &GetDirectionRef();

        void SetDirection(const glm::vec3 &dir);

        float GetLeft() const;

        float &GetLeftRef();

        float GetRight() const;

        float &GetRightRef();

        float GetBottom() const;

        float &GetBottomRef();

        float GetTop() const;

        float &GetTopRef();

        float GetZNear() const;

        float &GetZNearRef();

        float GetZFar() const;

        float &GetZFarRef();

        float *GetBoxRangeRef();

        float *GetZRangeRef();
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

        void SetQuadratic(const float &q);

        glm::mat4 GetLightSpaceTransformMat() const override;
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

        SpotLightModule(glm::vec3 dir, float innerRange, float outerRange, glm::vec3 ambient, glm::vec3 diffuse,
                        glm::vec3 specular);

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

        glm::mat4 GetLightSpaceTransformMat() const override;


    };
} // namespace dawn_engine
