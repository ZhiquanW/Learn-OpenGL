#include "dawn_engine.h"
namespace dawn {
DawnEngine::DawnEngine(uint32_t win_width, uint32_t win_height, const std::string name)
    : deltaTime(1.0f / 60.0f), lastTime(0.0f) {
    this->renderWindow = new RenderWindow(win_width, win_height, name.c_str());
    this->mainCamera = Camera();
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("failed to initialize glad");
        return;
    }
    glViewport(0, 0, this->renderWindow->get_win_width(), this->renderWindow->get_win_height());
    glEnable(GL_DEPTH_TEST);
    this->createShaderPrograms();
    this->addDefaultCube();
    this->addDefaultLight();
    // customized data
    // this->cubeMaterial =
    //     Material(glm::vec3(1.0f, 0.5f, 0.31f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f);
}

DawnEngine::~DawnEngine() {
    glDeleteVertexArrays(1, &this->cubeVAO);
    glDeleteBuffers(1, &this->cubeVBO);
    glDeleteBuffers(1, &this->cubeEBO);
    delete this->renderWindow;
}

void DawnEngine::addDefaultLight() {
    std::shared_ptr<PointLight> pLight(new PointLight(glm::vec3(1.0f), glm::vec3(0.9f), glm::vec3(0.9f), glm::vec3(0.9f)));
    this->addLight(pLight);
}

void DawnEngine::addDefaultCube() { this->gameObjects.emplace_back(GameObject::createPrimitive(CubePrimitiveType)); }

void DawnEngine::addGameObject(bool isEntity) {}

void DawnEngine::launch() {
    this->awake();
    this->start();
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    glClearDepth(1.0);
    while (!this->renderWindow->should_close()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GLfloat currentTime = static_cast<GLfloat>(glfwGetTime());
        this->deltaTime = currentTime - this->lastTime;
        this->lastTime = currentTime;
        this->renderWindow->process_inputs(&this->mainCamera, this->deltaTime);
        this->update();
        this->render();
        this->renderWindow->swap_buffers();
    }
}

void DawnEngine::setupLights() {}

void DawnEngine::addLight(const std::shared_ptr<DirectionalLight> light) {
    if (this->dirLightNum < this->MAX_DIR_LIGHT_NUM) {
        this->lights.emplace_back(light);
        this->dirLightNum += 1;
    }
}
void DawnEngine::addLight(const std::shared_ptr<PointLight> light) {
    if (this->pointLightNum < this->MAX_POINT_LIGHT_NUM) {
        this->lights.emplace_back(light);
        this->pointLightNum += 1;
    }
}
void DawnEngine::render() {
    glm::mat4 modelMat = glm::mat4(1.0f);
    glm::mat4 view = this->mainCamera.getViewMatrix();
    glm::mat4 projection = glm::perspective(
        glm::radians(this->mainCamera.getFov()),
        (float)this->renderWindow->get_win_width() / (float)this->renderWindow->get_win_height(), 0.001f, 100.0f);
    glm::vec3 camPos = this->mainCamera.getPos();
    this->gameObjectShader->activate();
    this->gameObjectShader->setMatrix4fv("view", view);
    this->gameObjectShader->setMatrix4fv("projection", projection);
    this->gameObjectShader->setUniform("cam_pos", camPos);
    this->gameObjectShader->setUniforms(dynamic_cast<PointLight *>(this->lights.begin()->get())->getUniforms("p_light"));
    for (auto gObj : this->gameObjects) {
        VisualShapeModule *visualShape = gObj->getModule<VisualShapeModule>();
        if (visualShape) {
            this->gameObjectShader->setMatrix4fv("model_mat", gObj->getModule<TransformModule>()->getModelMat4());
            this->gameObjectShader->setUniforms(visualShape->getMaterial().getUniforms("material"));
            visualShape->setRenderTarget();
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }
    // this->testShader->activate();
    // glBindVertexArray(this->VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 3);

    // render light
    this->lightShader->activate();
    // for (auto light : this->lights) {
    //     light->
    // }
    // this->lights.glBindVertexArray(this->lightVAO);
    // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void DawnEngine::add_data() {}

void DawnEngine::createShaderPrograms() {
    this->gameObjectShader = new DawnShaderProgram("../shaders/tri.vs", "../shaders/tri.fs");
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}
} // namespace dawn