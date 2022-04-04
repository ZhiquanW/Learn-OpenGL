#version 330 core 
out vec4 out_color; 
uniform vec3 objectColor;
uniform vec3 lightColor;
void main() { 
    out_color = vec4(1.0f,1.0f,1.0f,1.0f);
    // out_color = vec4(vec3(gl_FragCoord.z), 1.0);
};