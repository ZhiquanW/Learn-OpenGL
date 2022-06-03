#include "dawn_engine.h"

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
        this->createShaderPrograms();
    }


    DawnEngine::~DawnEngine() {
        glDeleteVertexArrays(1, &this->cubeVAO);
        glDeleteBuffers(1, &this->cubeVBO);
        glDeleteBuffers(1, &this->cubeEBO);
        delete this->renderWindow;
        if (!this->uiSystem) {
            delete this->uiSystem;
        }
    }

// void DawnEngine::addDefaultLight() {
//     std::shared_ptr<PointLight> pLight(new PointLight(glm::vec3(1.0f), glm::vec3(0.9f), glm::vec3(0.9f), glm::vec3(0.9f)));
//     this->addLight(pLight);
// }

// void DawnEngine::addDefaultCube() { this->gameObjects.emplace_back(GameObject::createPrimitive(CubePrimitiveType)); }

// void DawnEngine::addGameObject(bool isEntity) {}

// void DawnEngine::addGameObject(const std::shared_ptr<GameObject> gameObjPtr) { this->gameObjects.emplace_back(gameObjPtr); }

    void DawnEngine::launch() {
        this->awake();
        this->start();
        if (this->uiSystem != nullptr) {
            this->uiSystem->start(this);
        }
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClearDepth(1.0);
        while (!this->renderWindow->should_close()) {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            auto currentTime = static_cast<GLfloat>(glfwGetTime());
            this->deltaTime = currentTime - this->lastTime;
            this->lastTime = currentTime;
            // Start the Dear ImGui frame
            this->renderWindow->process_inputs(&this->mainCamera, this->deltaTime);
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

    void DawnEngine::render() {
        glm::mat4 modelMat = glm::mat4(1.0f);
        glm::mat4 view = this->mainCamera.getViewMatrix();
        glm::mat4 projection = glm::perspective(
                glm::radians(this->mainCamera.getFov()),
                (float) this->renderWindow->getWinWidth() /
                (float) this->renderWindow->getWinHeight(), 0.001f, 100.0f);
        glm::vec3 camPos = this->mainCamera.getPos();
        this->gameObjectShader->activate();
        this->gameObjectShader->setUniform("cam_view", view);
        this->gameObjectShader->setUniform("cam_proj", projection);
        this->gameObjectShader->setUniform("cam_pos", camPos);
        // set light information in shader program
        uint32_t dirLightNum = 0;
        uint32_t pointLightNum = 0;
        uint32_t spotLightNum = 0;
        for (auto gObj: this->gameObjectPtrs) {
            auto *dirLightM = gObj->getModule<DirectionalLightModule>();
            if (dirLightM != nullptr && dirLightM->getActivation()) {
                this->gameObjectShader->setUniforms(dirLightM->getUniforms(dirLightNum));
                dirLightNum++;
            }
            auto *pointLightM = gObj->getModule<PointLightModule>();
            if (pointLightM != nullptr && pointLightM->getActivation()) {
                this->gameObjectShader->setUniforms(pointLightM->getUniforms(pointLightNum));
                pointLightNum++;
            }
            auto *spotLightM = gObj->getModule<SpotLightModule>();
            if (spotLightM != nullptr && spotLightM->getActivation()) {
                this->gameObjectShader->setUniforms(spotLightM->getUniforms((spotLightNum)));
                spotLightNum++;
            }
        }
        this->gameObjectShader->setUniform("dir_lights_num", int(dirLightNum));
        this->gameObjectShader->setUniform("point_lights_num", int(pointLightNum));
        this->gameObjectShader->setUniform("spot_lights_num", int(spotLightNum));
        for (auto gObj: this->gameObjectPtrs) {
            auto *mesh_m = gObj->getModule<MeshModule>();
            if (mesh_m && mesh_m->getActivation()) {
                this->gameObjectShader->setUniform("model_mat",
                                                   gObj->getModule<TransformModule>()->getModelMat4());
                this->gameObjectShader->setUniforms(
                        mesh_m->getMaterial().getUniforms("material"));
                mesh_m->setAsRenderTarget();
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
        // render light
        this->lightShader->activate();
    }

    void DawnEngine::add_data() {}

    void DawnEngine::createShaderPrograms() {
        this->gameObjectShader = new DawnShaderProgram("../shaders/tri.vert",
                                                       "../shaders/tri.frag");
        this->lightShader = new DawnShaderProgram("../shaders/light.vs", "../shaders/light.fs");
        this->testShader = new DawnShaderProgram("../shaders/test.vs", "../shaders/test.fs");
    }

    void DawnEngine::loadTextures(const char *text_path_0, const char *text_path_1) {

        glGenTextures(1, &this->texture0);
        // activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, this->texture0);
        // set the texture wrapping/filtering options (on the currently bound
        // texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);

        unsigned char *data = stbi_load(text_path_0, &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                         data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        glGenTextures(1, &this->texture1);
        // activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, this->texture1);
        // set the texture wrapping/filtering options (on the currently bound
        // texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        // stbi_set_flip_vertically_on_load(true);

        data = stbi_load(text_path_1, &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                         data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }

    void DawnEngine::mountUISystem(DawnUISystem *uiSystem) {
        this->uiSystem = uiSystem;
        this->uiSystem->initialize(this->renderWindow->getWindowPtr());

    }

    std::vector<GameObject *> DawnEngine::getGameObjectPtrs() const {
        return this->gameObjectPtrs;
    }
} // namespace dawn_engine