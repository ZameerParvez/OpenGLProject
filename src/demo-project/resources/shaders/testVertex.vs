#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;

out vec3 vertColour;

uniform float time;

void main() {
  gl_Position = vec4(cos(time)/2 + aPos.x, sin(time)/2 + aPos.y, aPos.z, 1.0);
  vertColour = aPos;
}