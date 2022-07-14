#version 430 core 
#define MAX_POINT_LIGHT_NUM 32
#define MAX_DIR_LIGHT_NUM 32
#define MAX_SPOT_LIGHT_NUM 32
struct Camera {
    mat4 projection;
    mat4 view;
    vec3 pos;
    float z_near;
    float z_far;
}; 
struct Material {
    bool enable_lighting_maps;
    bool opaque;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float transparency;
    sampler2D diffuse_texture_0;
    sampler2D specular_texture_0;
    sampler2D normal_texture_0;
}; 

uniform Material material;

layout(std140,binding = 0) uniform CameraBlock{
    Camera main_camera;
};


out vec4 out_color; 

void main() { 
    vec3 material_diffuse = vec3(0.0,0.0,0.0);
    vec3 material_specular = vec3(0.0,0.0,0.0);
    vec3 shader_color = material.ambient;
    float alpha = 1.0f-material.transparency;
    out_color =vec4(shader_color,alpha);
}