#version 430 core 
out vec4 out_color; 

uniform float zNear; 
uniform float zFar;
float linearize_depth(float depth) {
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * zNear * zFar) / (zFar + zNear - z * (zFar - zNear))/zFar;	
}
void main() { 
    out_color =vec4( vec3(linearize_depth(gl_FragCoord.z)),1.0f);

};