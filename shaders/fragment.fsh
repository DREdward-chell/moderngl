#version 460

in vec3 vcol;
out vec4 fragColor;

void main() {
    fragColor = vec4(vcol, 1.0);
}