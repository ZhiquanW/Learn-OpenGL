#include "light_module.h"
#include "core/game_object.h"
#include <cmath>

namespace dawn_engine {

    LightModule::LightModule(LightType light_type)
            : BaseModule(), light_type_(light_type), ambient_(glm::vec3(0.1f)), diffuse_(glm::vec3(1.0f)),
              specular_(glm::vec3(1.0f)) {}

    LightModule::LightModule(LightType light_type, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
            : BaseModule(), light_type_(light_type), ambient_(ambient), diffuse_(diffuse), specular_(specular) {}

    LightModule::LightModule(LightType light_type, glm::vec3 color)
            : BaseModule(), light_type_(light_type), ambient_(color), diffuse_(color), specular_(color) {}

    LightModule::~LightModule() = default;


// base light
    std::vector<std::shared_ptr<ShaderUniformVariableBase>>
    LightModule::getUniforms(const std::string &name) const {
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> light_uniforms;
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".ambient_",
                                                                   this->ambient_));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".diffuse",
                                                                   this->diffuse_));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<glm::vec3>>(name + ".specular",
                                                                   this->specular_));
        return light_uniforms;
    }

    glm::vec3 LightModule::GetAmbient() const {
        return this->ambient_;
    }

    glm::vec3 &LightModule::GetAmbientMeta() {
        return this->ambient_;
    }

    glm::vec3 LightModule::GetDiffuse() const {
        return this->diffuse_;
    }

    glm::vec3 &LightModule::GetDiffuseMeta() {
        return this->diffuse_;
    }

    glm::vec3 LightModule::GetSpecular() const {
        return this->specular_;
    }

    glm::vec3 &LightModule::GetSpecularMeta() {
        return this->specular_;
    }

    void LightModule::SetAmbient(glm::vec3 ambient) {
        this->ambient_ = ambient;
    }

    void LightModule::SetDiffuse(glm::vec3 diffuse) {
        this->diffuse_ = diffuse;

    }

    [[maybe_unused]] void LightModule::SetSpecular(glm::vec3 specular) {
        this->specular_ = specular;

    }

    GLTexture LightModule::GetShadowMapTexture() const {
        return this->depth_map_;
    }

    LightType LightModule::GetLightType() const {
        return this->light_type_;
    }

    std::string LightModule::GetModuleType() {
        return quote(LightModule);
    }


// direction light
    DirectionalLightModule::DirectionalLightModule()
            : LightModule(LightType::DirectionalLight,
                          glm::vec3(0.0f),
                          glm::vec3(1.0f),
                          glm::vec3(1.0f)),
              direction_(glm::vec3(-1.0f)) {
        this->depth_map_ = {GLTextureTarget::Texture2D, GLTextureAttachment::DepthAttachment,
                            AllocateGLTexture(GLTextureFormat::DepthComponent, this->DEFAULT_DEPTH_MAP_SIZE),
                            this->DEFAULT_DEPTH_MAP_SIZE};
    }

    DirectionalLightModule::DirectionalLightModule(glm::vec3 dir)
            : LightModule(LightType::DirectionalLight, glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f)),
              direction_(dir) {
        this->depth_map_ = {GLTextureTarget::Texture2D, GLTextureAttachment::DepthAttachment,
                            AllocateGLTexture(GLTextureFormat::DepthComponent, this->DEFAULT_DEPTH_MAP_SIZE),
                            this->DEFAULT_DEPTH_MAP_SIZE};


    }

    DirectionalLightModule::DirectionalLightModule(glm::vec3 direction, glm::vec3 color)
            : LightModule(LightType::DirectionalLight, color), direction_(direction) {
        this->depth_map_ = {GLTextureTarget::Texture2D, GLTextureAttachment::DepthAttachment,
                            AllocateGLTexture(GLTextureFormat::DepthComponent, this->DEFAULT_DEPTH_MAP_SIZE),
                            this->DEFAULT_DEPTH_MAP_SIZE};

    }

    DirectionalLightModule::DirectionalLightModule(glm::vec3 direction, glm::vec3 ambient,
                                                   glm::vec3 diffuse,
                                                   glm::vec3 specular)
            : LightModule(LightType::DirectionalLight, ambient, diffuse, specular), direction_(direction) {
        this->depth_map_ = {GLTextureTarget::Texture2D, GLTextureAttachment::DepthAttachment,
                            AllocateGLTexture(GLTextureFormat::DepthComponent, this->DEFAULT_DEPTH_MAP_SIZE),
                            this->DEFAULT_DEPTH_MAP_SIZE};

    }

    DirectionalLightModule::DirectionalLightModule(glm::vec3 dir, float left, float right, float bottom, float top,
                                                   float z_near, float z_far)
            : LightModule(LightType::DirectionalLight, glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f)),
              direction_(dir),
              box_range_{left, right, bottom, top},
              z_range_{z_near, z_far} {
        this->depth_map_ = {GLTextureTarget::Texture2D, GLTextureAttachment::DepthAttachment,
                            AllocateGLTexture(GLTextureFormat::DepthComponent, this->DEFAULT_DEPTH_MAP_SIZE),
                            this->DEFAULT_DEPTH_MAP_SIZE};

    }

    DirectionalLightModule::DirectionalLightModule(glm::vec3 dir, float left, float right, float bottom, float top,
                                                   float z_near, float z_far, glm::vec3 color) :
            LightModule(LightType::DirectionalLight, color),
            direction_(dir),
            box_range_{left, right, bottom, top},
            z_range_{z_near, z_far} {
        this->depth_map_ = {GLTextureTarget::Texture2D, GLTextureAttachment::DepthAttachment,
                            AllocateGLTexture(GLTextureFormat::DepthComponent, this->DEFAULT_DEPTH_MAP_SIZE),
                            this->DEFAULT_DEPTH_MAP_SIZE};

    }

    DirectionalLightModule::DirectionalLightModule(glm::vec3 dir, float left, float right, float bottom, float top,
                                                   float z_near, float z_far, glm::vec3 ambient, glm::vec3 diffuse,
                                                   glm::vec3 specular) : LightModule(LightType::DirectionalLight,
                                                                                     ambient, diffuse, specular),
                                                                         direction_(dir),
                                                                         box_range_{left, right, bottom, top},
                                                                         z_range_{z_near, z_far} {
        this->depth_map_ = {GLTextureTarget::Texture2D, GLTextureAttachment::DepthAttachment,
                            AllocateGLTexture(GLTextureFormat::DepthComponent, this->DEFAULT_DEPTH_MAP_SIZE),
                            this->DEFAULT_DEPTH_MAP_SIZE};

    }


    DirectionalLightModule::~DirectionalLightModule() = default;;

    glm::vec3 DirectionalLightModule::GetDirection() const { return glm::normalize(this->direction_); }

    glm::vec3 &DirectionalLightModule::GetDirectionRef() {
        this->direction_ = glm::normalize(this->direction_);
        return this->direction_;
    }

    glm::vec3 DirectionalLightModule::GetTransformedDirection() const {
        return glm::vec3(this->GetAttachedGameObject()->GetModule<TransformModule>()->GetRotationMat() *
                         glm::vec4(this->GetDirection(), 1.0f));
    }

    void DirectionalLightModule::SetDirection(const glm::vec3 &dir) { this->direction_ = dir; }

    std::vector<std::shared_ptr<ShaderUniformVariableBase>>
    DirectionalLightModule::getUniforms(const uint32_t idx) const {
        std::string indexed_name = fmt::format("directional_lights[{}]", idx);
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> light_uniforms = LightModule::getUniforms(
                indexed_name);
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<glm::vec3>>(indexed_name + ".direction",
                                                                   this->direction_));
        return light_uniforms;
    }

    float DirectionalLightModule::GetLeft() const {
        return this->box_range_[0];
    }

    float DirectionalLightModule::GetRight() const {
        return this->box_range_[1];

    }

    float DirectionalLightModule::GetBottom() const {
        return this->box_range_[2];
    }

    float DirectionalLightModule::GetTop() const {
        return this->box_range_[3];

    }

    float DirectionalLightModule::GetZNear() const {
        return this->z_range_[0];
    }

    float DirectionalLightModule::GetZFar() const {
        return this->z_range_[1];
    }

    float &DirectionalLightModule::GetLeftRef() {
        return this->box_range_[0];

    }

    float &DirectionalLightModule::GetRightRef() {
        return this->box_range_[1];

    }

    float &DirectionalLightModule::GetBottomRef() {
        return this->box_range_[2];

    }

    float &DirectionalLightModule::GetTopRef() {
        return this->box_range_[3];

    }

    float &DirectionalLightModule::GetZNearRef() {
        return this->z_range_[0];
    }

    float &DirectionalLightModule::GetZFarRef() {
        return this->z_range_[1];
    }

    float *DirectionalLightModule::GetBoxRangeRef() {
        return this->box_range_;
    }

    float *DirectionalLightModule::GetZRangeRef() {
        return this->z_range_;
    }

    glm::mat4 DirectionalLightModule::GetLightSpaceTransformMat() const {
        auto transform_module = this->owner_ptr_->GetModule<TransformModule>();
        auto light_projection = glm::ortho(this->GetLeft(),
                                           this->GetRight(),
                                           this->GetBottom(),
                                           this->GetTop(),
                                           this->GetZNear(),
                                           this->GetZFar());
        auto light_view = glm::lookAt(
                transform_module->GetPosition(),
                transform_module->GetPosition() + this->GetTransformedDirection(),
                glm::vec3(0.0, 1.0, 0.0));
        return light_projection * light_view;
    }

    std::string DirectionalLightModule::GetModuleName() {
        return quote(DirectinalLightModule);
    }


// point light
    PointLightModule::PointLightModule() : LightModule(LightType::PointLight), constant(1.0), linear(0.1),
                                           quadratic(0.08) {}

    PointLightModule::PointLightModule(glm::vec3 color, float constant, float linear,
                                       float quadratic)
            : LightModule(LightType::PointLight, color), constant(constant), linear(linear), quadratic(quadratic) {}

    PointLightModule::PointLightModule(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                                       float constant,
                                       float linear,
                                       float quadratic)
            : LightModule(LightType::PointLight, ambient, diffuse, specular), constant(constant), linear(linear),
              quadratic(quadratic) {}

    std::vector<std::shared_ptr<ShaderUniformVariableBase>>
    PointLightModule::getUniforms(const uint32_t idx) const {
        std::string indexed_name = fmt::format("point_lights[{}]", idx);
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> light_uniforms = LightModule::getUniforms(
                indexed_name);
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<float>>(indexed_name + ".constant",
                                                               this->constant));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<float>>(indexed_name + ".linear",
                                                               this->linear));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<float>>(indexed_name + ".quadratic",
                                                               this->quadratic));
        light_uniforms.emplace_back(std::make_shared<ShaderUniformVariable<glm::vec3>>(
                indexed_name + ".position",
                this->GetAttachedGameObject()->GetModule<TransformModule>()->GetPosition()));
        return light_uniforms;
    }

    float PointLightModule::getConstant() const {
        return this->constant;
    }

    float &PointLightModule::GetConstantMeta() {
        return this->constant;
    }

    void PointLightModule::setConstant(const float &c) {
        this->constant = c;

    }

    float PointLightModule::getLinear() const {
        return this->linear;
    }

    float &PointLightModule::getLinearMeta() {
        return this->linear;
    }

    void PointLightModule::setLinear(const float &l) {
        this->linear = l;
    }

    float PointLightModule::getQuadratic() const {
        return this->quadratic;
    }

    float &PointLightModule::getQuadraticMeta() {
        return this->quadratic;
    }

    void PointLightModule::SetQuadratic(const float &q) {
        this->quadratic = q;
    }

    glm::mat4 PointLightModule::GetLightSpaceTransformMat() const {
        return glm::mat4();
    }

    std::string PointLightModule::GetModuleName() {
        return quote(PointLightModule);
    }


    SpotLightModule::SpotLightModule()
            : PointLightModule(), direction(glm::vec3(-1.0f)), innerRange(15), outerRange(30) {
    }

    SpotLightModule::SpotLightModule(glm::vec3 dir, float innerRange, float outerRange)
            : PointLightModule(), direction(dir), innerRange(innerRange), outerRange(outerRange) {
    }

    std::vector<std::shared_ptr<ShaderUniformVariableBase>>
    SpotLightModule::getUniforms(const uint32_t idx) const {
        std::string indexed_name = fmt::format("spot_lights[{}]", idx);
        std::vector<std::shared_ptr<ShaderUniformVariableBase>> light_uniforms = LightModule::getUniforms(
                indexed_name);
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<glm::vec3>>(indexed_name + ".direction",
                                                                   this->getDirection()));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<glm::vec3>>(indexed_name + ".position",
                                                                   this->GetAttachedGameObject()->GetModule<TransformModule>()->GetPosition()));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<float>>(indexed_name + ".linear",
                                                               this->getLinear()));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<float>>(indexed_name + ".constant",
                                                               this->getConstant()));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<float>>(indexed_name + ".quadratic",
                                                               this->getQuadratic()));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<float>>(indexed_name + ".inner_range",
                                                               this->getInnerRange()));
        light_uniforms.emplace_back(
                std::make_shared<ShaderUniformVariable<float>>(indexed_name + ".outer_range",
                                                               this->getOuterRange()));
        return light_uniforms;
    }

    glm::vec3 SpotLightModule::getDirection() const { return glm::normalize(this->direction); }

    glm::vec3 &SpotLightModule::getDirectionMeta() {
        this->direction = glm::normalize(this->direction);
        return this->direction;
    }

    void SpotLightModule::setDirection(const glm::vec3 &dir) { this->direction = dir; }

    float SpotLightModule::getInnerRange() const {
        return this->innerRange;
    }

    float &SpotLightModule::getInnerRangeMeta() {
        return this->innerRange;
    }

    void SpotLightModule::setInnerRange(const float &range) {
        this->innerRange = std::min(this->outerRange, range);
    }

    float SpotLightModule::getOuterRange() const {
        return this->outerRange;
    }

    float &SpotLightModule::getOuterRangeMeta() {
        return this->outerRange;
    }

    void SpotLightModule::setOuterRange(const float &range) {
        this->outerRange = std::max(this->innerRange, range);
    }

    glm::mat4 SpotLightModule::GetLightSpaceTransformMat() const {
        return glm::mat4();
    }

    std::string SpotLightModule::GetModuleType() {
        return quote(SpotLightModule);
    }


} // namespace dawn_engine
