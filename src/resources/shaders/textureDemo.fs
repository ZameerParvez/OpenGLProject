#version 460 core

out vec4 FragColor;

in vec3 ourColour;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
  vec4 texel0, texel1, result;
  texel0 = texture(texture1, TexCoord);
  texel1 = texture(texture2, vec2(-TexCoord.s, TexCoord.t));
  FragColor = mix( texel0, (texel1 + vec4(ourColour,1)) / 2, texel1.a * 0.8);
}