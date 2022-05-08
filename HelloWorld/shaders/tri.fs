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
    vec3 pposition = vec3(1,1,1);
    vec3 pambient = vec3(1,1,1);
    vec3 pdiffuse = vec3(1,1,1);
    vec3 pspecular = vec3(1,1,1);
    vec3 mambient = vec3(1,1,1);
    vec3 mdiffuse = vec3(1,1,1);
    vec3 mspecular = vec3(1,1,1);
    float mshininess = 1.0;
    vec3 light_dir = normalize(pposition-frag_pos);
    // ambient
    vec3 ambient =  mambient * pambient;
    // diffuse
    vec3 norm = normalize(frag_normal);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse =diff*  mdiffuse * pdiffuse;
    // specular
    vec3 view_dir = normalize(cam_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);  
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), mshininess);
    vec3 specular = spec * mspecular * pspecular;

    vec3 shader_color = ambient + diffuse + specular;
    out_color =vec4(ambient+diffuse+specular ,1.0f);
    // out_color =vec4(1.0f,1.0f,1.0f,1.0f);
};