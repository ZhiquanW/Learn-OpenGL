//
// @Author: Zhiquan Wang 
// @Date: 2/16/20.
// @Email: zhiquan.wzq@gmail.com
// Copyright (c) 2020 Zhiquan Wang. All rights reserved.
//

#include "Camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 up, GLfloat fov, GLfloat aspect, GLfloat mv_speed, GLfloat m_sensitivity)
        : pos(pos), world_up(up),
          yaw(-90.0f), pitch(0.0f), fov(fov),
          movement_speed(mv_speed), mouse_sensitivity(m_sensitivity), aspect(aspect) {
    update_coord();
}

inline void Camera::update_coord() {
    glm::vec3 tmp_front;
    tmp_front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    tmp_front.y = sin(glm::radians(this->pitch));
    tmp_front.z = sin(glm::radians(this->yaw) * cos(glm::radians(this->pitch)));

    this->front = glm::normalize(tmp_front);

    this->right = glm::normalize(glm::cross(this->front, this->world_up));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}

glm::mat4 Camera::get_view_mat() {
    return glm::lookAt(this->pos, this->pos + this->front, this->up);
}


glm::mat4 Camera::get_projection_mat(){
    return glm::perspective(glm::radians(this->fov),this->aspect,0.1f,10000.0f);
}

void Camera::process_keyboard(Camera_Movement dir, float delta_time) {
    float v = this->movement_speed * delta_time;
    switch (dir) {
        case FORWARD:
            this->pos += this->front * v;
            break;
        case BACKWARD:
            this->pos -= this->front * v;
            break;
        case LEFT:
            this->pos -= this->right * v;
            break;
        case RIGHT:
            this->pos += this->right * v;
            break;
    }
}

void Camera::process_mouse_movement(glm::vec2 offset, GLboolean constrain_pitch) {
    this->yaw += offset.x * this->mouse_sensitivity;
    this->pitch += offset.y * this->mouse_sensitivity;
    if (constrain_pitch) {
        this->pitch = glm::min(pitch, 89.0f);
        this->pitch = glm::max(pitch, -89.0f);
    }
    this->update_coord();
}

void Camera::process_mouse_scroll(GLfloat y_offset) {
    this->fov -= y_offset;
    this->fov = glm::max(this->fov, 1.0f);
    this->fov = glm::min(this->fov, 45.0f);
}

GLfloat Camera::get_fov() {
    return this->fov;
}

void Camera::set_pos(glm::vec3 pos) {
    this->pos = pos;
}



