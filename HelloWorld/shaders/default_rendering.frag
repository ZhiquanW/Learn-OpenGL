#version 430 core 
#define MAX_POINT_LIGHT_NUM 8
#define MAX_DIR_LIGHT_NUM 8
#define MAX_SPOT_LIGHT_NUM 8
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
    mat4 light_space_transform_mat;
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
uniform sampler2D shadow_maps[MAX_DIR_LIGHT_NUM+MAX_POINT_LIGHT_NUM+MAX_SPOT_LIGHT_NUM];
// uniform sampler2D shadow_maps[2];

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

vec3 compute_directional_light(DirectionalLight dir_light,
                                vec3 normal,
                                vec3 view_dir,
                                vec3 m_diffuse,
                                vec3 m_specular,
                                float in_shadow){
    if (dir_light.activation){
        vec3 light_dir = normalize(dir_light.direction);
        vec3 ambient = dir_light.ambient * material.ambient;
        vec3 diffuse = dir_light.diffuse * m_diffuse *  max(dot(normal,-light_dir),0.0);
        vec3 reflect_dir = reflect(light_dir,normal);
        vec3 specular = dir_light.specular * m_specular * pow(max(dot(view_dir,reflect_dir),0.0),material.shininess);
        return ambient +(1-in_shadow) * (diffuse + specular);    
    }
    return vec3(0.0);
}

vec3 compute_point_light(PointLight point_light,vec3 normal,vec3 view_dir,vec3 m_diffuse,vec3 m_specular,float in_shadow){
    if(point_light.activation){
        float distance = length(point_light.position-frag_pos);
        float attenuation = 1.0/(point_light.constant + point_light.linear * distance + point_light.quadratic*distance*distance);
        vec3 to_light_dir = normalize(point_light.position - frag_pos);
        vec3 ambient = point_light.ambient * material.ambient;
        vec3 diffuse = point_light.diffuse * m_diffuse *max(dot(normal,to_light_dir),0.0);
        vec3 reflect_dir = reflect(-to_light_dir,normal);
        vec3 specular = point_light.specular * vec3(1.0)  * pow(max(dot(view_dir,reflect_dir),0.0),material.shininess);
        return ambient + (1-in_shadow) *(diffuse + specular);
    }
    return vec3(0.0);
}

vec3 compute_spot_light(SpotLight spot_light,vec3 normal,vec3 view_dir,vec3 m_diffuse,vec3 m_specular,float in_shadow){
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
        vec3 reflect_dir = reflect(-to_light_dir,normal);
        vec3 specular =   spot_light.specular * m_specular * pow(max(dot(view_dir,reflect_dir),0.0),material.shininess);
        return attenuation*(ambient + (1-in_shadow) *intensity*(diffuse + specular));
    }
    return vec3(0.0);
}

float shadow_calculation(vec4 frag_pos_in_light_space,vec3 normal,vec3 light_dir,int shadow_map_idx){
    // perform perspective divide
    vec3 proj_coords = frag_pos_in_light_space.xyz / frag_pos_in_light_space.w;
    // transform to [0,1] range
    proj_coords = proj_coords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closest_depth = texture(shadow_maps[shadow_map_idx], proj_coords.xy).r; 
    // get depth of current fragment from light's perspective
    float current_depth = proj_coords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.001* (1.0 - dot(normal, light_dir)), 0.005);  

    float shadow = 0.0;
    vec2 texel_size = 1.0 / textureSize(shadow_maps[shadow_map_idx], 0);
    for(int x = -1; x <= 1; ++x){
        for(int y = -1; y <= 1; ++y){
            float pcf_depth = texture(shadow_maps[shadow_map_idx], proj_coords.xy + vec2(x, y) * texel_size).r; 
            shadow += (current_depth - bias) > pcf_depth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    if(proj_coords.z >= 1.0){
        shadow = 0.0;
    }

    return shadow;
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
    float in_shadow = 0.0;
    for(int i = 0; i < dir_lights_num; ++ i){
        vec4 frag_pos_in_light_space =  directional_lights[i].light_space_transform_mat * vec4(frag_pos,1.0f);
        if (shadow_calculation(frag_pos_in_light_space,normal,directional_lights[i].direction,i)==1.0){
            in_shadow = 1.0;
            break;
        }
    }

    for (int i = 0;i < dir_lights_num;++ i){
        shader_color += compute_directional_light(directional_lights[i],normal,view_dir,material_diffuse,material_specular,in_shadow);
    }
    for(int i = 0;i < point_lights_num;++ i){
        shader_color+= compute_point_light(point_lights[i],normal,view_dir,material_diffuse,material_specular,in_shadow);
    }
    for(int i = 0;i < spot_lights_num;++ i){
        shader_color+= compute_spot_light(spot_lights[i],normal,view_dir,material_diffuse,material_specular,in_shadow);
    }
    float alpha = 1.0f-material.transparency;

    out_color =vec4(shader_color,alpha);
}