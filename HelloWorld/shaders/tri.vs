#version 330 core 
layout (location = 0) in vec3 pos; 
// layout (location = 1) in vec3 text_coord;
layout (location = 1) in vec3 normal;
// out vec3 vColor; 
uniform mat4 model_mat;
uniform mat4 projection;
uniform mat4 view;

out vec3 frag_normal;
out vec3 frag_pos;  

void main() 
{ 
    gl_Position = projection * view * model_mat * vec4(pos, 1.0);
    frag_normal = transpose(inverse(mat3(model_mat)))* normal;
    frag_pos = vec3(model_mat * vec4(pos, 1.0));


};