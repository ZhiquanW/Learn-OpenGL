//
// Created by zhiquan on 6/9/22.
//

#ifndef HELLOWORLD_DAWN_MATERIAL_H
#define HELLOWORLD_DAWN_MATERIAL_H
# define MAX_BONE_INFLUENCE 4

#include "../../include/common_includes.h"
#include "../../include/shader_uniform_variable.h"
#include "shader_program_table.h"
#include "opengl_shader_program.h"
#include "core/dawn_texture.h"

namespace dawn_engine {

    enum DawnMaterialType {
        Phong, Texture2D, CubeMap
    };

    class DawnMaterial {
    private:
    protected:
        ShaderInfo shader_info_ = ShaderTable::default_shader_info;
        glm::vec3 ambient_ = glm::vec3(0.0f);
        glm::vec3 diffuse_ = glm::vec3(1.0f);
        glm::vec3 specular_ = glm::vec3(1.0f);
        float shininess_ = 3.0f;
        bool opaque_ = true;
        float transparency_ = 0.0f;
        std::vector<std::shared_ptr<DawnTexture>> diffuse_textures_;
        std::vector<std::shared_ptr<DawnTexture>> specular_textures_;
        std::vector<std::shared_ptr<DawnTexture>> normal_textures_;
        std::vector<std::shared_ptr<DawnTexture>> cube_map_textures_;
        DawnMaterialType material_type_ = DawnMaterialType::Phong;


    public:
        DawnMaterial() = default;

        DawnMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);

        DawnMaterial(std::vector<std::shared_ptr<DawnTexture>> diffuse_textures,
                     std::vector<std::shared_ptr<DawnTexture>> specular_textures,
                     std::vector<std::shared_ptr<DawnTexture>> normal_textures);

        DawnMaterial(std::vector<std::shared_ptr<DawnTexture>> cube_map_textures);

        ~DawnMaterial() = default;

        void setOpaque(bool opaque);

        bool GetOpaque() const;

        bool &getOpaqueRef();

        void setTransparency(float alpha);

        [[nodiscard]] float GetTransparency() const;

        [[maybe_unused]] float &getTransparencyRef();

        [[nodiscard]] glm::vec3 GetAmbient() const;

        [[nodiscard]] glm::vec3 &getAmbientColorRef();

        void setAmbientColor(glm::vec3 value);

        [[nodiscard]] glm::vec3 GetDiffuse() const;

        [[nodiscard]] glm::vec3 &getDiffuseColorRef();

        void setDiffuseColor(glm::vec3 value);

        [[nodiscard]] glm::vec3 GetSpecular() const;

        [[nodiscard]] glm::vec3 &getSpecularColorRef();

        void setSpecularColor(glm::vec3 value);

        [[nodiscard]] float GetShininess() const;

        [[nodiscard]] float &getShininessRef();

        void setShininess(float value);

        bool EnabledLightingMaps() const;

        [[nodiscard]] std::vector<std::shared_ptr<ShaderUniformVariableBase>> getUniforms() const;

        void bindTextures() const;

        const ShaderInfo GetShaderInfo() const;

        std::vector<std::shared_ptr<DawnTexture>> GetDiffuseTextures() const;

        std::vector<std::shared_ptr<DawnTexture>> GetSpecularTextures() const;

        std::vector<std::shared_ptr<DawnTexture>> GetNormalTextures() const;

        std::vector<std::shared_ptr<DawnTexture>> GetCubeMapTextures() const;

        DawnMaterialType GetMaterialType() const;
    };


}
#endif //HELLOWORLD_DAWN_MATERIAL_H
