#version 460 core

in vec3 vertColour;

out vec4 FragColor;

void main() {
  FragColor = vec4(vertColour, 1.0);
}