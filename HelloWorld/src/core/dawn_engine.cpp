#include "dawn_engine.h"
#include <sstream>
#include <fstream>

namespace dawn_engine {

    DawnEngine::DawnEngine(uint32_t win_width, uint32_t win_height, const std::string &name)
            : deltaTime(1.0f / 60.0f), lastTime(0.0f), uiSystem(nullptr) {
        this->renderWindow = new RenderWindow(win_width, win_height, name);
        this->mainCamera = Camera();
        this->gameObjectPtrs = {};
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            throw std::runtime_error("failed to initialize glad");
            return;
        }
        glViewport(0, 0, (GLsizei) this->renderWindow->getWinWidth(),
                   (GLsizei) this->renderWindow->getWinHeight());
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        this->initShaderPrograms();
    }


    DawnEngine::~DawnEngine() {
        delete this->renderWindow;
        if (!this->uiSystem) {
            delete this->uiSystem;
        }
    }

    void DawnEngine::launch() {
        this->awake();
        this->start();
        if (this->uiSystem != nullptr) {
            this->uiSystem->start(this);
        }
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1.0f);
        while (!this->renderWindow->should_close()) {
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            auto currentTime = static_cast<GLfloat>(glfwGetTime());
            this->deltaTime = currentTime - this->lastTime;
            this->lastTime = currentTime;
            // Start the Dear ImGui frame
            this->renderWindow->process_inputs(&this->mainCamera, this->deltaTime);
            this->uniformUpdate();
            this->update();
            this->render();
            if (uiSystem != nullptr) {
                this->uiSystem->render(this);
            }
            this->renderWindow->swap_buffers();
            glfwPollEvents();

        }
    }

    void DawnEngine::addGameObject(GameObject *gObjPtr) {
        this->gameObjectPtrs.emplace_back(gObjPtr);
    }

    void DawnEngine::uniformUpdate() {
        // update camera uniforms
        this->setUniformInShaderPrograms({"default", "depth"}, extractUniforms("main_camera", mainCamera, this->renderWindow->getWinWidth(), this->renderWindow->getWinHeight()));
        // update light uniforms
        uint32_t dirLightNum = 0;
        uint32_t pointLightNum = 0;
        uint32_t spotLightNum = 0;
        for (auto gObj: this->gameObjectPtrs) {
            auto *dirLightM = gObj->getModule<DirectionalLightModule>();
            if (dirLightM != nullptr && dirLightM->getActivation()) {
                this->setUniformInShaderPrograms({"default", "depth"}, extractUniforms(fmt::format("directional_lights[{}]", dirLightNum++), dirLightM));
            }
            auto *pointLightM = gObj->getModule<PointLightModule>();
            if (pointLightM != nullptr && pointLightM->getActivation()) {
                this->setUniformInShaderPrograms({"default", "depth"}, extractUniforms(fmt::format("point_lights[{}]", pointLightNum++), pointLightM));
            }
            auto *spotLightM = gObj->getModule<SpotLightModule>();
            if (spotLightM != nullptr && spotLightM->getActivation()) {
                this->setUniformInShaderPrograms({"default", "depth"}, extractUniforms(fmt::format("spot_lights[{}]", spotLightNum++), spotLightM));
            }
        }
        this->setUniformInShaderPrograms({"default", "depth"}, {std::make_shared<ShaderUniformVariable<int>>("dir_lights_num", int(dirLightNum)),
                                                                std::make_shared<ShaderUniformVariable<int>>("point_lights_num", int(pointLightNum)),
                                                                std::make_shared<ShaderUniformVariable<int>>("spot_lights_num", int(spotLightNum))});
    }


    void DawnEngine::render() {
        struct renderMesh {
            DawnMesh mesh;
            glm::mat4 modelMat;
        };
        std::map<float, renderMesh> transparentRenderMap = {};
        for (auto gObj: this->gameObjectPtrs) {
            auto *meshM = gObj->getModule<MeshModule>();
            auto modelMat = gObj->getModule<TransformModule>()->getModelMat4();
            auto pos = gObj->getModule<TransformModule>()->getPosition();
            float zDis = glm::length(this->mainCamera.getPos() - pos);
            this->activeShader->activate();
            if (meshM and meshM->getActivation()) {
                for (const auto &mesh: meshM->getMeshesMeta()) {
                    if (mesh.getMaterial().getOpaque()) {
                        // render opaque object
                        this->activeShader->setUniform("model_mat", gObj->getModule<TransformModule>()->getModelMat4());
                        mesh.render(this->activeShader);
                    } else {
                        // insert transparent object for sorting
                        transparentRenderMap.insert({zDis, {mesh, modelMat}});
                    }
                }
            }
        }
        for (auto it = transparentRenderMap.rbegin(); it != transparentRenderMap.rend(); ++it) {
            this->activeShader->setUniform("model_mat", it->second.modelMat);
            it->second.mesh.render(this->activeShader);
        }
//        for (auto gObj: this->gameObjectPtrs) {
//            auto *mesh_m = gObj->getModule<MeshModule>();
//            if (mesh_m and mesh_m->getActivation()) {
//                mesh_m->render(this->activeShader);
//            }
//        }
    }

    void DawnEngine::initShaderPrograms() {
        shaderProgramMap.insert({"default", new OpenGLShaderProgram("default", "../shaders/default_rendering.vert",
                                                                    "../shaders/default_rendering.frag")});
        shaderProgramMap.insert({"depth", new OpenGLShaderProgram("depth", "../shaders/default_rendering.vert",
                                                                  "../shaders/depth_buffer_rendering.frag")});
//        shaderProgramMap.insert({"model", new OpenGLShaderProgram("model", "../shaders/model.vert", "../shaders/model.frag")});

        this->activeShader = shaderProgramMap["default"];
//        this->gameObjectShader = new OpenGLShaderProgram("../shaders/default_rendering.vert",
//                                                         "../shaders/default_rendering.frag");
//        this->lightShader = new OpenGLShaderProgram("../shaders/light.vs", "../shaders/light.fs");
//        this->testShader = new OpenGLShaderProgram("../shaders/test.vs", "../shaders/test.fs");
//        this->modelShader = new OpenGLShaderProgram("../shaders/model.vert", "../shaders/model.frag");
    }


    void DawnEngine::mountUISystem(DawnUISystem *uiSystem) {
        this->uiSystem = uiSystem;
        this->uiSystem->initialize(this->renderWindow->getWindowPtr());

    }

    std::vector<GameObject *> DawnEngine::getGameObjectPtrs() const {
        return this->gameObjectPtrs;
    }

    void DawnEngine::loadModel(const std::string &modelPath) {

    }

    bool DawnEngine::enabledDepthRendering() const {
        return this->enableDepthRendering;
    }

    bool &DawnEngine::getDepthRenderingSwitchMeta() {
        return this->enableDepthRendering;
    }

    std::unordered_map<std::string, OpenGLShaderProgram *> &DawnEngine::getShaderProgramMapMeta() {
        return this->shaderProgramMap;
    }

    void DawnEngine::setActiveShaderProgram(const char *name) {
        this->activeShader = this->shaderProgramMap.at(name);

    }

    [[maybe_unused]] OpenGLShaderProgram *DawnEngine::getActiveShaderProgram() {
        return this->activeShader;


    }

    Camera &DawnEngine::getMainCameraMeta() {
        return this->mainCamera;
    }

    void DawnEngine::setUniformInShaderPrograms(std::vector<std::string> shaderProgramNames, const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &uniforms) {
        for (const auto &name: shaderProgramNames) {
            auto shaderProgramPtr = this->shaderProgramMap.at(name);
            if (shaderProgramPtr != nullptr) {
                shaderProgramPtr->activate();
                shaderProgramPtr->setUniforms(uniforms);
            }
        }

    }
} // namespace dawn_engine