#version 430 core 
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
    float constant;
    float linear;
    float quadratic;
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
uniform PointLight point_lights[MAX_POINT_LIGHT_NUM];
uniform DirectionalLight directional_lights[MAX_DIR_LIGHT_NUM];
uniform int dir_lights_num;
uniform int point_lights_num;
uniform int flash_lights_num;
uniform vec3 cam_pos ;
out vec4 out_color; 

vec3 compute_directional_light(DirectionalLight dir_light,vec3 normal,vec3 view_dir){
    vec3 to_light_dir = normalize(-dir_light.direction);
    vec3 ambient = dir_light.ambient * material.ambient;
    vec3 diffuse = dir_light.diffuse *material.diffuse *  max(dot(normal,to_light_dir),0.0);
    vec3 reflect_dir = reflect(-to_light_dir,normal);
    vec3 specular = dir_light.specular * material.specular * pow(max(dot(view_dir,reflect_dir),0.0),material.shininess);
    return ambient + diffuse + specular;
}

vec3 compute_point_light(PointLight point_light,vec3 normal,vec3 view_dir){
    float distance = length(point_light.position-frag_pos);
    float attenuation = 1.0/(point_light.constant + point_light.linear * distance + point_light.quadratic*distance*distance);
    normal = normalize(normal);
    vec3 to_light_dir = normalize(point_light.position - frag_pos);
    vec3 ambient = point_light.ambient * material.ambient;

    vec3 diffuse = point_light.diffuse * material.diffuse*max(dot(normal,to_light_dir),0.0);
    vec3 reflect_dir = reflect(-to_light_dir,normal);
    vec3 specular = point_light.specular * material.specular * pow(max(dot(view_dir,reflect_dir),0.0),material.shininess);
    return attenuation * (ambient + diffuse + specular);
}
void main() { 
    vec3 view_dir = normalize(cam_pos - frag_pos);
    vec3 shader_color = vec3(0.0);
    for (int i = 0;i < dir_lights_num;++ i){
        shader_color += compute_directional_light(directional_lights[i],frag_normal,view_dir);
    }
    for(int i = 0;i < point_lights_num;++ i){
        shader_color+= compute_point_light(point_lights[i],frag_normal,view_dir);
    }
    // out_color =vec4(directional_lights[0].ambient ,1.0f);
    out_color =vec4(shader_color ,1.0f);
    // out_color =vec4(1.0f,1.0f,1.0f,1.0f);
};