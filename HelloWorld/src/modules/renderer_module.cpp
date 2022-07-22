#include "renderer_module.h"
#include "transform_module.h"
#include "core/game_object.h"
#include <utility>
#include "utils/glsl_utils.h"

namespace dawn_engine {

// Visual Shape Module
    const std::size_t RendererModule::type = BaseModule::GetHashModuleName(quote(RendererModule));


    RendererModule::RendererModule(DawnModel model) : BaseModule(), model_(std::move(model)) {
        this->activations.resize(model_.GetMeshNum());
        std::fill(this->activations.begin(), this->activations.end(), true);
        GenerateGLRenderObjects();
    }

    void RendererModule::GenerateGLRenderObjects() {
        std::unordered_map<std::string, unsigned int> texture_id_map = {};
        for (unsigned int i = 0; i < this->model_.GetMeshesRef().size(); ++i) {
            auto mesh = this->model_.GetMeshesRef()[i];
            auto gl_obj = AllocateGLVertexData(mesh.GetVerticesRef(), mesh.GetIndicesRef());

            if (mesh.GetMaterialPtr()->GetMaterialType() == DawnMaterialType::Texture2D) {
                std::vector<unsigned int> diffuse_gl_texture_ids = AllocateGLTextureDataFiltered(texture_id_map,
                                                                                                 mesh.GetMaterialPtr()->GetDiffuseTextures());
                std::vector<unsigned int> specular_gl_texture_ids = AllocateGLTextureDataFiltered(texture_id_map,
                                                                                                  mesh.GetMaterialPtr()->GetSpecularTextures());
                std::vector<unsigned int> normal_gl_texture_ids = AllocateGLTextureDataFiltered(texture_id_map,
                                                                                                mesh.GetMaterialPtr()->GetNormalTextures());
//                this->gl_render_objects.push_back(
//                        std::make_shared<GLRenderObject>(gl_obj[0], gl_obj[1], gl_obj[2],
//                                                         mesh.GetIndicesNum(),
//                                                         diffuse_gl_texture_ids, specular_gl_texture_ids, normal_gl_texture_ids,
//                                                         mesh.GetMaterialPtr().GetShaderInfo()));
                this->render_obj_map.insert({i, std::make_shared<GLRenderObject>(gl_obj[0], gl_obj[1], gl_obj[2],
                                                                                 mesh.GetIndicesNum(),
                                                                                 diffuse_gl_texture_ids, specular_gl_texture_ids, normal_gl_texture_ids,
                                                                                 mesh.GetMaterialPtr()->GetShaderInfo())});
            } else if (mesh.GetMaterialPtr()->GetMaterialType() == DawnMaterialType::Phong) {
                this->render_obj_map.insert(
                        {i, std::make_shared<GLRenderObject>(gl_obj[0], gl_obj[1], gl_obj[2],
                                                             mesh.GetIndicesNum(),
                                                             mesh.GetMaterialPtr()->GetShaderInfo())});
            } else if (mesh.GetMaterialPtr()->GetMaterialType() == DawnMaterialType::CubeMap) {
                unsigned int cube_map_gl_texture_id = LoadCubeMap2GL(mesh.GetMaterialPtr()->GetCubeMapTextures());
                this->render_obj_map.insert({i, std::make_shared<GLRenderObject>(gl_obj[0], gl_obj[1], gl_obj[2],
                                                                                 mesh.GetIndicesNum(),
                                                                                 cube_map_gl_texture_id,
                                                                                 mesh.GetMaterialPtr()->GetShaderInfo())});
            } else if (mesh.GetMaterialPtr()->GetMaterialType() == DawnMaterialType::Pure) {
                GLRenderElement render_element = GLRenderElement::TRIANGLE;
                if (mesh.GetIndicesNum() == 2) {
                    render_element = GLRenderElement::LINE;
                    glLineWidth(16);
                }

                this->render_obj_map.insert(
                        {i, std::make_shared<GLRenderObject>(render_element,
                                                             gl_obj[0],
                                                             gl_obj[1],
                                                             gl_obj[2],
                                                             mesh.GetIndicesNum(),
                                                             mesh.GetMaterialPtr()->GetShaderInfo())});
            }
        }
    }

//    std::vector<unsigned int>
//    RendererModule::AllocateGLTextureDataFiltered(std::unordered_map<std::string, unsigned int> &loaded_texture_id_map, const std::vector<std::shared_ptr<DawnTexture>> &
//    textures) {
//        std::vector<unsigned int> gl_texture_ids = {};
//        for (const auto &tex: textures) {
//            if (loaded_texture_id_map.find(tex->GetName()) != loaded_texture_id_map.end()) {
//                gl_texture_ids.push_back(loaded_texture_id_map.at(tex->GetName()));
//            } else {
//                unsigned gl_tex_id = LoadTexture2GL(tex, true);
//                gl_texture_ids.push_back(gl_tex_id);
//                loaded_texture_id_map.insert({tex->GetName(), gl_tex_id});
//            }
//        }
//        return gl_texture_ids;
//    }

//    void RendererModule::render(GLShaderProgram *shaderProgram) const {
//        shaderProgram->activate();
//        for (unsigned int meshIdx = 0; meshIdx < this->meshes.size(); ++meshIdx) {
//            if (this->activations[meshIdx]) {
//                shaderProgram->SetUniform("model_mat", this->GetAttachedGameObject()->GetModule<TransformModule>()->GetModelMat());
//                this->meshes[meshIdx].render(shaderProgram);
//            }
//        }
//    }
//



    std::vector<DawnMesh> RendererModule::getMeshes() const {
        return this->model_.GetMeshes();
    }

    std::vector<DawnMesh> &RendererModule::getMeshesRef() {
        return this->model_.GetMeshesRef();
    }

    std::deque<bool> RendererModule::getActivations() const {
        return this->activations;
    }

    std::deque<bool> &RendererModule::getActivationsRef() {
        return this->activations;
    }

//    void RendererModule::activateMesh(int idx) {
//        idx = std::clamp((int) idx, 0, (int) this->meshes_.size());
//        this->activations[idx] = true;
//
//    }

//    RendererModule::RendererModule(std::vector<float> vertexData) : BaseModule(), vertexData(std::move(vertexData)), material(Material()) {
//        this->InitGLData();
//    }

//    RendererModule::RendererModule(std::vector<float> vertexData, Material material)
//            : BaseModule(), vertexData(std::move(vertexData)), material(material) {
//        this->InitGLData();
//    }

    RendererModule::~RendererModule() {

    }

    void RendererModule::render() const {
        for (const auto &render_pair: this->render_obj_map) {
            render_pair.second->render();
        }
    }

//    void RendererModule::render(const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &external_uniforms) const {
//        for (const auto &gl_render_obj: this->gl_render_objects) {
//            gl_render_obj->render(ExtractUniforms("model_mat", this->GetAttachedGameObject()->GetModule<TransformModule>()));
//        }
//    }


//    std::vector<std::shared_ptr<GLRenderObject>> &RendererModule::GetGLRenderObjects() {
//        return this->gl_render_objects;
//    }

    std::unordered_map<unsigned int, std::shared_ptr<GLRenderObject>> &RendererModule::GetGLRenderObjectMap() {
        return this->render_obj_map;
    }

    DawnMesh &RendererModule::GetMesh(unsigned int idx) {
        return this->model_.GetMeshesRef()[idx];
    }

    DawnModel &RendererModule::GetModelRef() {
        return this->model_;
    }



//    void RendererModule::InitGLData() {
//        glGenVertexArrays(1, &this->glVAO);
//        glBindVertexArray(this->glVAO);
//        glGenBuffers(1, &this->glVBO);
//        glBindBuffer(GL_ARRAY_BUFFER, this->glVBO);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(this->meshes_[0]) * this->meshes_.size(), this->meshes_.data(),
//                     GL_STATIC_DRAW);
//        const int posLen = 3;
//        const int normalLen = 3;
//        const int vertexSize = posLen + normalLen;
//        const int data_offsets[] = {0, posLen};
//        glVertexAttribPointer(0, posLen, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float), (void *) 0);
//        glEnableVertexAttribArray(0);
//        glVertexAttribPointer(1, normalLen, GL_FLOAT, GL_FALSE, vertexSize * sizeof(float),
//                              (void *) ((data_offsets[1]) * sizeof(float)));
//        glEnableVertexAttribArray(1);
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//    }

//    std::vector<float> RendererModule::getVertexData() const { return this->vertexData; }
//
//    Material RendererModule::GetMaterialPtr() const { return this->material; }
//
//    Material &RendererModule::GetMaterialRef() { return this->material; }
//
//    uint32_t RendererModule::getVertexDataSize() const { return this->vertexData.size(); }

//    void RendererModule::setAsRenderTarget() const { glBindVertexArray(this->glVAO); }


} // namespace dawn_engine
