#version 430 core
out vec4 FragColor;
in vec3 oCol;
in vec2 oTexCoord;
layout(location = 1)uniform sampler2D oTex0;
layout(location = 2)uniform sampler2D oTex1;
void main() {
//    FragColor = vec4(oCol, 1.0f);
    FragColor = mix(texture(oTex0,oTexCoord),texture(oTex1,oTexCoord),0.5);
}