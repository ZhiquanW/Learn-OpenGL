#version 430 core
layout (location = 0) in vec3 pos;

uniform mat4 light_space_transform_mat;
uniform mat4 model_mat;

void main(){
    gl_Position = light_space_transform_mat * model_mat * vec4(pos, 1.0);
}  

// glActiveTexture(GL_TEXTURE5);
// glBindTexture(GL_TEXTURE_2D, shadow_map_tex);
// glniform1i("shadow_maps[0]", 5);
//
// glsl:
//   texture(shadow_maps[0], uv);