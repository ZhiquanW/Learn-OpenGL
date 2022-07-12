#pragma once

#include "core/game_object.h"
#include "base_module.h"
#include "core/dawn_mesh.h"
#include "core/dawn_model.h"
#include "graphics_layer/gl_render_object.h"

namespace dawn_engine {

    class RendererModule : public BaseModule {
    private:

    protected:
        std::deque<bool> activations;
        DawnModel model_;
        std::unordered_map<unsigned int, std::shared_ptr<GLRenderObject>> render_obj_map;

    public:
        static const std::size_t type;

        RendererModule() = default;

        explicit RendererModule(DawnModel model);

        ~RendererModule() override;

        void GenerateGLRenderObjects();

//        std::vector<unsigned int>
//        AllocateGLTextureDataFiltered(std::unordered_map<std::string, unsigned int> &loaded_texture_id_map, const std::vector<std::shared_ptr<DawnTexture>> &textures);

        [[nodiscard]] std::vector<DawnMesh> getMeshes() const;

        [[nodiscard]] std::vector<DawnMesh> &getMeshesRef();

        [[nodiscard]] std::deque<bool> getActivations() const;


        [[nodiscard]] std::deque<bool> &getActivationsRef();

//
//        void render(const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &external_uniforms) const;
//
        void render() const;

        std::vector<std::shared_ptr<GLRenderObject>> &GetGLRenderObjects();

        std::unordered_map<unsigned int, std::shared_ptr<GLRenderObject>> &GetGLRenderObjectMap();

        DawnMesh &GetMesh(unsigned int idx);

        std::vector<DawnMesh> &GetMeshes();

        DawnModel &GetModelRef();


    };

} // namespace dawn_engine