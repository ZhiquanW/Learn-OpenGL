#version 430 core
out vec4 FragColor;
in vec3 oCol;
in vec2 oTexCoord;
layout(location = 0)uniform sampler2D oTex0;
layout(location = 1)uniform sampler2D oTex1;
void main() {
        FragColor = mix(texture(oTex0, oTexCoord), texture(oTex1, oTexCoord), 1.0f);
//        FragColor =vec4(0.5,0.2,0.8,1.0f);
}