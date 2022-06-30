//
// Created by zhiquan on 6/9/22.
//

#include "dawn_material.h"
#include <utility>

namespace dawn_engine {

    DawnMaterial::DawnMaterial(std::vector<std::shared_ptr<DawnTexture>> diffuse_textures,
                               std::vector<std::shared_ptr<DawnTexture>> specular_textures,
                               std::vector<std::shared_ptr<DawnTexture>> normal_textures)
            : diffuse_textures_(std::move(diffuse_textures)), specular_textures_(std::move(specular_textures)), normal_textures_(std::move(normal_textures)),
              material_type_(DawnMaterialType::Texture2D), shader_info_(ShaderTable::default_shader_info) {
    }


    DawnMaterial::DawnMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
            : ambient_(ambient), diffuse_(diffuse), specular_(specular), shininess_(shininess), material_type_(DawnMaterialType::Phong),
              shader_info_(ShaderTable::default_shader_info) {
    }

    DawnMaterial::DawnMaterial(std::vector<std::shared_ptr<DawnTexture>> cube_map_textures) : cube_map_textures_(std::move(cube_map_textures)),
                                                                                              material_type_(DawnMaterialType::CubeMap) {
        this->shader_info_ = ShaderTable::skybox_shader_info;
    }


    glm::vec3 DawnMaterial::GetAmbient() const {
        return this->ambient_;
    }

    glm::vec3 &DawnMaterial::getAmbientColorRef() {
        return this->ambient_;
    }

    void DawnMaterial::setAmbientColor(glm::vec3 value) {
        this->ambient_ = value;

    }

    glm::vec3 DawnMaterial::GetDiffuse() const {
        return this->diffuse_;
    }

    glm::vec3 &DawnMaterial::getDiffuseColorRef() {
        return this->diffuse_;
    }

    void DawnMaterial::setDiffuseColor(glm::vec3 value) {
        this->diffuse_ = value;

    }

    glm::vec3 DawnMaterial::GetSpecular() const {
        return this->specular_;
    }

    glm::vec3 &DawnMaterial::getSpecularColorRef() {
        return this->specular_;
    }

    void DawnMaterial::setSpecularColor(glm::vec3 value) {
        this->specular_ = value;

    }

    float DawnMaterial::GetShininess() const {
        return this->shininess_;
    }

    float &DawnMaterial::getShininessRef() {
        return this->shininess_;
    }

    void DawnMaterial::setShininess(float value) {
        this->shininess_ = value;
    }

    bool DawnMaterial::EnabledLightingMaps() const {
        return !this->diffuse_textures_.empty();
    }


//    void DawnMaterial::bindTextures() const {
//        int textureUnitIdx = 0;
//        for (auto texID: this->diffuse_tex_ids_) {
//            glActiveTexture(GL_TEXTURE0 + textureUnitIdx++);
//            glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
//        }
//        for (auto texID: this->specular_tex_ids_) {
//            glActiveTexture(GL_TEXTURE0 + textureUnitIdx++);
//            glBindTexture(GL_TEXTURE_2D, texID);
//        }
//        for (auto texID: this->normal_tex_ids_) {
//            glActiveTexture(GL_TEXTURE0 + textureUnitIdx++);
//            glBindTexture(GL_TEXTURE_2D, texID);
//        }
//    }

    void DawnMaterial::setOpaque(bool opaque) {
        this->opaque_ = opaque;
    }

    bool DawnMaterial::GetOpaque() const {
        return this->opaque_;
    }

    bool &DawnMaterial::getOpaqueRef() {
        return this->opaque_;
    }

    void DawnMaterial::setTransparency(float alpha) {
        this->transparency_ = alpha;
    }

    float DawnMaterial::GetTransparency() const {
        return this->transparency_;
    }

    [[maybe_unused]] float &DawnMaterial::getTransparencyRef() {
        return this->transparency_;
    }

    const ShaderInfo DawnMaterial::GetShaderInfo() const {
        return this->shader_info_;
    }

    std::vector<std::shared_ptr<DawnTexture>> DawnMaterial::GetDiffuseTextures() const {
        return this->diffuse_textures_;
    }

    std::vector<std::shared_ptr<DawnTexture>> DawnMaterial::GetSpecularTextures() const {
        return this->specular_textures_;
    }

    std::vector<std::shared_ptr<DawnTexture>> DawnMaterial::GetNormalTextures() const {
        return this->normal_textures_;
    }

    std::vector<std::shared_ptr<DawnTexture>> DawnMaterial::GetCubeMapTextures() const {
        return this->cube_map_textures_;
    }

    DawnMaterialType DawnMaterial::GetMaterialType() const {
        return this->material_type_;
    }


}
