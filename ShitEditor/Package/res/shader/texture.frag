#version 310 es
precision mediump float;

out vec4 FragColor;

in vec4 vColor;
in vec2 TexCoord;

uniform sampler2D mainTexture;

void main() {
    FragColor = texture(mainTexture, TexCoord) * vColor;
}
