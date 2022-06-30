#version 430 core 

struct Camera {
    mat4 view;
    mat4 projection;
    vec3 pos;
    float z_near;
    float z_far;
}; 
layout (location = 0) in vec3 pos; 
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;

// out vec3 vColor;
uniform mat4 model_mat;
uniform Camera main_camera;
out vec3 frag_normal;
out vec3 frag_pos;  
out vec2 frag_tex_coord;
void main() { 
    frag_normal = transpose(inverse(mat3(model_mat)))* normal;
    frag_pos = vec3(model_mat * vec4(pos, 1.0));
    frag_tex_coord = tex_coord;
    gl_Position = main_camera.projection * main_camera.view * model_mat * vec4(pos, 1.0);
}