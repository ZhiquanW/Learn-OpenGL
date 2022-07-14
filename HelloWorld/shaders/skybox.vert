#version 430 core
layout (location = 0) in vec3 pos;

out vec3 TexCoords;

layout(std140,binding =0 ) uniform CameraBlock{
    mat4 projection;
    mat4 view;
    float z_near;
    float z_far;
};
 
void main(){
    TexCoords = pos;
    vec4 pos = projection * mat4(mat3(view))* vec4(pos, 1.0);
    gl_Position = pos.xyww;

}  