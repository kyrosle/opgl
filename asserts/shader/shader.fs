#version 330 core
out vec4 FragColor;

in vec3 ourColor;
uniform vec4 outColor;

void main() {
  FragColor = vec4((ourColor.x + outColor.x) / 2, (ourColor.y + outColor.y) / 2, (ourColor.z + outColor.z) / 2, 1.0);
}