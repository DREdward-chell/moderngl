#version 460

in vec3 vertexcolor;
out vec4 fragColor;

void main() {
    fragColor = vec4(vertexcolor, 1.0);
}