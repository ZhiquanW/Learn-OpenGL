#include "graphics_layer/gl_shader_program.h"

namespace dawn_engine {

    unsigned int GLShaderProgram::global_texture_counter = 0;
    unsigned int GLShaderProgram::global_texture_space = 0;

    GLShaderProgram::GLShaderProgram(const char *vertexPath, const char *fragmentPath) : vertexPath(vertexPath),
                                                                                         fragmentPath(fragmentPath) {
        // 1. retrieve the vertex/fragment source code from filePath
        this->id = this->createShaderProgram();
        vertexPath = vertexPath;
        fragmentPath = fragmentPath;
    }

    GLShaderProgram::GLShaderProgram(const char *name, const char *vertexPath, const char *fragmentPath) : name(name),
                                                                                                           vertexPath(
                                                                                                                   vertexPath),
                                                                                                           fragmentPath(
                                                                                                                   fragmentPath) {
        this->id = this->createShaderProgram();
    }

    GLShaderProgram::~GLShaderProgram() {
        glDeleteProgram(this->id);
    }


    int GLShaderProgram::createShaderProgram() {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            // open files
            vShaderFile.open(this->vertexPath);
            fShaderFile.open(this->fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        } catch (std::ifstream::failure e) {
            std::cout << "error::shader::file not read successfully" << std::endl;
        }
        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // shader Program
        int shaderID = glCreateProgram();
        glAttachShader(shaderID, vertex);
        glAttachShader(shaderID, fragment);
        glLinkProgram(shaderID);
        checkCompileErrors(shaderID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer
        // necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return shaderID;
    }


    void GLShaderProgram::Reload() {
        this->id = createShaderProgram();
    }

    void GLShaderProgram::Activate() const { glUseProgram(this->id); }

    void GLShaderProgram::SetBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(this->id, name.c_str()), (int) value);
    }

    void GLShaderProgram::SetInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
    }

    void GLShaderProgram::SetFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(this->id, name.c_str()), value);
    }

    void GLShaderProgram::SetMatrix4fv(const std::string &name, glm::mat4 value) const {
        glUniformMatrix4fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE,
                           glm::value_ptr(value));
    }

    void GLShaderProgram::SetVec3fv(const std::string &name, glm::vec3 value) const {
        glUniform3fv(glGetUniformLocation(this->id, name.c_str()), 1, glm::value_ptr(value));
    }

    void GLShaderProgram::SetUniform(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(this->id, name.c_str()), (int) value);
    }

    void GLShaderProgram::SetUniform(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);

    }

    void GLShaderProgram::SetUniform(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(this->id, name.c_str()), value);
    }

    void GLShaderProgram::SetUniform(const std::string &name, glm::vec3 value) const {
        glUniform3fv(glGetUniformLocation(this->id, name.c_str()), 1, glm::value_ptr(value));
    }


    void GLShaderProgram::SetUniform(const std::string &name, glm::mat4 value) const {
        glUniformMatrix4fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE,
                           glm::value_ptr(value));
    }

    void GLShaderProgram::SetUniforms(
            const std::vector<std::shared_ptr<ShaderUniformVariableBase>> &uniforms) const {
        for (const auto &u_sp: uniforms) {
            ShaderUniformVariableBase *u_ptr = u_sp.get();
            if (u_ptr->GetTypeHash() == typeid(int).hash_code()) {
                this->SetUniform(u_ptr->GetName(), dynamic_cast<ShaderUniformVariable<int> *>(u_ptr)->GetData());
//                this->SetInt(u_ptr->GetName(),
//                             dynamic_cast<ShaderUniformVariable<int> *>(u_ptr)->GetData());
            } else if (u_ptr->GetTypeHash() == typeid(bool).hash_code()) {
                this->SetUniform(u_ptr->GetName(), dynamic_cast<ShaderUniformVariable<bool> *>(u_ptr)->GetData());
//                this->SetBool(u_ptr->GetName(),
//                              dynamic_cast<ShaderUniformVariable<bool> *>(u_ptr)->GetData());
            } else if (u_ptr->GetTypeHash() == typeid(float).hash_code()) {
                this->SetUniform(u_ptr->GetName(), dynamic_cast<ShaderUniformVariable<float> *>(u_ptr)->GetData());
                //                this->SetFloat(u_ptr->GetName(),
//                               dynamic_cast<ShaderUniformVariable<float> *>(u_ptr)->GetData());
            } else if (u_ptr->GetTypeHash() == typeid(glm::vec3).hash_code()) {
                this->SetUniform(u_ptr->GetName(), dynamic_cast<ShaderUniformVariable<glm::vec3> *>(u_ptr)->GetData());
//                this->SetVec3fv(u_ptr->GetName(),
//                                dynamic_cast<ShaderUniformVariable<glm::vec3> *>(u_ptr)->GetData());
            } else if (u_ptr->GetTypeHash() == typeid(glm::mat4).hash_code()) {
                this->SetUniform(u_ptr->GetName(), dynamic_cast<ShaderUniformVariable<glm::mat4> *>(u_ptr)->GetData());
//                this->SetMatrix4fv(u_ptr->GetName(),
//                                   dynamic_cast<ShaderUniformVariable<glm::mat4> *>(u_ptr)->GetData());
            } else {
                throw std::runtime_error("unknown shader uniform variable type");
            }
        }
    }

    void GLShaderProgram::SetUniforms(
            const std::map<std::string, std::shared_ptr<ShaderUniformVariableBase>> &uniforms) const {
        for (const auto &uniform: uniforms) {
            ShaderUniformVariableBase *u_ptr = uniform.second.get();
            if (u_ptr->GetTypeHash() == typeid(int).hash_code()) {
                this->SetUniform(u_ptr->GetName(), dynamic_cast<ShaderUniformVariable<int> *>(u_ptr)->GetData());
//                this->SetInt(u_ptr->GetName(),
//                             dynamic_cast<ShaderUniformVariable<int> *>(u_ptr)->GetData());
            } else if (u_ptr->GetTypeHash() == typeid(bool).hash_code()) {
                this->SetUniform(u_ptr->GetName(), dynamic_cast<ShaderUniformVariable<bool> *>(u_ptr)->GetData());
//                this->SetBool(u_ptr->GetName(),
//                              dynamic_cast<ShaderUniformVariable<bool> *>(u_ptr)->GetData());
            } else if (u_ptr->GetTypeHash() == typeid(float).hash_code()) {
                this->SetUniform(u_ptr->GetName(), dynamic_cast<ShaderUniformVariable<float> *>(u_ptr)->GetData());
                //                this->SetFloat(u_ptr->GetName(),
//                               dynamic_cast<ShaderUniformVariable<float> *>(u_ptr)->GetData());
            } else if (u_ptr->GetTypeHash() == typeid(glm::vec3).hash_code()) {
                this->SetUniform(u_ptr->GetName(), dynamic_cast<ShaderUniformVariable<glm::vec3> *>(u_ptr)->GetData());
//                this->SetVec3fv(u_ptr->GetName(),
//                                dynamic_cast<ShaderUniformVariable<glm::vec3> *>(u_ptr)->GetData());
            } else if (u_ptr->GetTypeHash() == typeid(glm::mat4).hash_code()) {
                this->SetUniform(u_ptr->GetName(), dynamic_cast<ShaderUniformVariable<glm::mat4> *>(u_ptr)->GetData());
//                this->SetMatrix4fv(u_ptr->GetName(),
//                                   dynamic_cast<ShaderUniformVariable<glm::mat4> *>(u_ptr)->GetData());
            } else {
                throw std::runtime_error("unknown shader uniform variable type");
            }
        }
    }

    void GLShaderProgram::checkCompileErrors(unsigned int shader, std::string type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "error::shader_compilation_error of type: " << type << "\n"
                          << infoLog
                          << "\n -- --------------------------------------------------- "
                             "-- "
                          << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "error::program linking error of type: " << type << "\n"
                          << infoLog
                          << "\n -- --------------------------------------------------- "
                             "-- "
                          << std::endl;
            }
        }
    }

    std::string GLShaderProgram::getName() const {
        return this->name;
    }

    unsigned int GLShaderProgram::GetID() const {
        return this->id;
    }


    void GLShaderProgram::AllocateTexture(unsigned int unit_idx, unsigned int texture_id) {
        glActiveTexture(GL_TEXTURE0 + unit_idx);
        glBindTexture(GL_TEXTURE_2D, texture_id);
    }

    unsigned int GLShaderProgram::GetGlobalTextureCount() {
        return GLShaderProgram::global_texture_counter;
    }

    unsigned int GLShaderProgram::AppendGlobalTexture(unsigned int texture_id) {
//        std::cout << "global " << GLShaderProgram::global_texture_counter << " " << texture_id << std::endl;
        if (GLShaderProgram::global_texture_counter < GLShaderProgram::global_texture_space){
            GLShaderProgram::AllocateTexture(GLShaderProgram::global_texture_counter, texture_id);
            return GLShaderProgram::global_texture_counter++;
        }else{
            throw  std::runtime_error("GLShaderProgram: Global Texture Space is full");
        }

    }

    void GLShaderProgram::ClearGlobalTextureUnit() {
        GLShaderProgram::global_texture_counter = 0;

    }

    void GLShaderProgram::ReserveGlobalTextureSpace(unsigned int space) {
        GLShaderProgram::global_texture_space = space;

    }

    unsigned int GLShaderProgram::GetGlobalTextureSpace() {
        return GLShaderProgram::global_texture_space;
    }


} // namespace dawn_engine
