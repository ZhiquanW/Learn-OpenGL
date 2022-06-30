#version 430 core 
out vec4 out_color; 
struct Camera {
    mat4 view;
    mat4 projection;
    vec3 pos;
    float z_near;
    float z_far;
}; 
uniform Camera main_camera;

float linearize_depth(float depth) {
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * main_camera.z_near * main_camera.z_far) / (main_camera.z_far + main_camera.z_near - z * (main_camera.z_far - main_camera.z_near))/main_camera.z_far;	
}
void main() { 
    // out_color =vec4( vec3(linearize_depth(gl_FragCoord.z)),1.0f);
    out_color =vec4( 1.0f);

}