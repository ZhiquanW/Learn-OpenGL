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

struct PointLight{
    bool activation;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
};
struct DirectionalLight{
    bool activation;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
};

struct SpotLight{
    bool activation;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    float inner_range;
    float outer_range;

};
in vec3 frag_normal;
in vec3 frag_pos;
in vec2 frag_tex_coord;
uniform Material material;
uniform sampler2D shadow_map;

layout(std140,binding = 0) uniform CameraBlock{
    Camera main_camera;
};


layout(std140,binding = 1) uniform DirectionalLightBlock{
    int dir_lights_num;
    DirectionalLight directional_lights[MAX_DIR_LIGHT_NUM];
};

layout(std140,binding = 2) uniform PointLightBlock{
    int point_lights_num;
    PointLight point_lights[MAX_POINT_LIGHT_NUM];
};

layout(std140,binding=3) uniform SpotLightBlock{
    int spot_lights_num;
    SpotLight spot_lights[MAX_SPOT_LIGHT_NUM];
};
out vec4 out_color;

vec3 compute_directional_light(DirectionalLight dir_light,vec3 normal,vec3 view_dir,vec3 m_diffuse,vec3 m_specular){
    if (dir_light.activation){
        vec3 light_dir = normalize(dir_light.direction);
        vec3 ambient = dir_light.ambient * material.ambient;
        vec3 diffuse = dir_light.diffuse * m_diffuse *  max(dot(normal,-light_dir),0.0);
        vec3 halfway_dir = normalize(light_dir + view_dir);
        vec3 specular = dir_light.specular * m_specular * pow(max(dot(normal,halfway_dir),0.0),material.shininess);
        return ambient + diffuse + specular;
    }
    return vec3(0.0);
}

vec3 compute_point_light(PointLight point_light,vec3 normal,vec3 view_dir,vec3 m_diffuse,vec3 m_specular){
    if(point_light.activation){
        float distance = length(point_light.position-frag_pos);
        float attenuation = 1.0/(point_light.constant + point_light.linear * distance + point_light.quadratic*distance*distance);
        vec3 to_light_dir = normalize(point_light.position - frag_pos);
        vec3 ambient = point_light.ambient * material.ambient;
        vec3 diffuse = point_light.diffuse * m_diffuse *max(dot(normal,to_light_dir),0.0);
        vec3 halfway_dir = normalize(-to_light_dir + view_dir);
        vec3 specular = point_light.specular * vec3(1.0)  * pow(max(dot(normal,halfway_dir),0.0),material.shininess);
        return ambient + diffuse + specular;
    }
    return vec3(0.0);
}

vec3 compute_spot_light(SpotLight spot_light,vec3 normal,vec3 view_dir,vec3 m_diffuse,vec3 m_specular){
    if(spot_light.activation){
        float distance = length(spot_light.position-frag_pos);
        float attenuation = 1.0/(spot_light.constant + spot_light.linear * distance + spot_light.quadratic*distance*distance);
        vec3 to_light_dir = normalize(spot_light.position - frag_pos);
        float theta = dot(to_light_dir, normalize(-spot_light.direction));
        float alpha = cos(radians(spot_light.inner_range));
        float beta = cos(radians(spot_light.outer_range));
        float epsilon = alpha - beta;
        float intensity = clamp((theta-beta)/epsilon,0.0f,1.0f);
        vec3 ambient = spot_light.ambient * material.ambient;
        vec3 diffuse = spot_light.diffuse * m_diffuse *  max(dot(normal,to_light_dir),0.0);
        vec3 halfway_dir = normalize(-to_light_dir + view_dir);
        vec3 specular =   spot_light.specular * m_specular * pow(max(dot(normal,halfway_dir),0.0),material.shininess);
        return attenuation*(ambient + intensity*(diffuse + specular));
    }
    return vec3(0.0);
}
float near = 0.001f;
float far = 100.0f;
float linearize_depth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near))/far;
}
void main() {
    vec3 view_dir = normalize(main_camera.pos - frag_pos);
    vec3 material_diffuse = vec3(0.0,0.0,0.0);
    vec3 material_specular = vec3(0.0,0.0,0.0);
    vec3 material_normal = vec3(0.0,0.0,0.0);
    vec3 normal = normalize(frag_normal);
    if(material.enable_lighting_maps){
        material_diffuse = texture(material.diffuse_texture_0,frag_tex_coord).rgb;
        material_specular = texture(material.specular_texture_0,frag_tex_coord).rgb;
    }else{
        material_diffuse = material.diffuse;
        material_specular = material.specular;
    }
    vec3 shader_color = vec3(0.0);
    for (int i = 0;i < dir_lights_num;++ i){
        shader_color += compute_directional_light(directional_lights[i],normal,view_dir,material_diffuse,material_specular);
    }
    for(int i = 0;i < point_lights_num;++ i){
        shader_color+= compute_point_light(point_lights[i],normal,view_dir,material_diffuse,material_specular);
    }
    for(int i = 0;i < spot_lights_num;++ i){
        shader_color+= compute_spot_light(spot_lights[i],normal,view_dir,material_diffuse,material_specular);
    }
    float alpha = 1.0f-material.transparency;
    out_color =vec4(shader_color,alpha);
}