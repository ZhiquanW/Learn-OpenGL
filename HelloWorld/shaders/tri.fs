#version 330 core 
#define MAX_POINT_LIGHT_NUM 64
#define MAX_DIR_LIGHT_NUM 64
#define MAX_FLASH_LIGHT_NUM 64

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct PointLight{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct DirectionalLight{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
in vec3 frag_normal;
in vec3 frag_pos;  

uniform Material material;
uniform PointLight p_light;
uniform PointLight pointLights[MAX_POINT_LIGHT_NUM];
uniform int dir_lights_num;
uniform int point_lights_num;
uniform int flash_lights_num;
uniform vec3 cam_pos ;
out vec4 out_color; 

void main() { 
    // bool enable_p_light = true;
    vec3 light_dir = normalize(p_light.position-frag_pos);
    // ambient
    vec3 ambient =  material.ambient * p_light.ambient;
    // diffuse
    vec3 norm = normalize(frag_normal);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse =diff*  material.diffuse * p_light.diffuse;
    // specular
    vec3 view_dir = normalize(cam_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);  
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 specular = spec * material.specular * p_light.specular;

    vec3 shader_color = ambient + diffuse + specular;
    out_color =vec4(ambient+diffuse+specular ,1.0f);
    // out_color =vec4(1.0f,1.0f,1.0f,1.0f);
};