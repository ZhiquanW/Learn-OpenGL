#version 430 core 

struct Camera {
    mat4 projection;
    mat4 view;
    vec3 pos;
    float z_near;
    float z_far;
}; 

layout(std140,binding = 0) uniform CameraBlock{
    Camera main_camera;
};


layout (location = 0) in vec3 pos; 

uniform mat4 model_mat;
void main() { 
    gl_Position = main_camera.projection * main_camera.view * model_mat * vec4(pos, 1.0);
}