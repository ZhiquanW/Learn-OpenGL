//
// @Author: Zhiquan Wang 
// @Date: 2/16/20.
// @Email: zhiquan.wzq@gmail.com
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//
#ifndef HELLO_WORLD_CAMERA_H
#define HELLO_WORLD_CAMERA_H

#include <glad/glad.h>

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
public:
    // camera attributes
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;
    GLfloat fov;
    GLfloat aspect;

    // euler angles
    GLfloat yaw;
    GLfloat pitch;

    // camera options
    GLfloat movement_speed;
    GLfloat mouse_sensitivity;

    //functions
    inline void update_coord();

public:
    Camera(glm::vec3= glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3 = glm::vec3(0.0f, 1.0f, 0.0f),
           GLfloat= 45.0f, GLfloat= 10.0f, GLfloat = 0.1f, GLfloat = 0.1f);

    glm::mat4 get_view_mat();

    void process_keyboard(Camera_Movement, float);

    void process_mouse_movement(glm::vec2, GLboolean = true);

    void process_mouse_scroll(GLfloat y_offset);

    GLfloat get_fov();
};


#endif //HELLO_WORLD_CAMERA_H