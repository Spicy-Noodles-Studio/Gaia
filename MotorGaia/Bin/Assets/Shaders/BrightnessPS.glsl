#version 330 core
in vec2 vUv0;

out vec4 fFragColor;

uniform float bright;
uniform sampler2D RTT0;

const vec3 WsRGB = vec3(0.2125, 0.7154, 0.0721);

void main() {
    vec4 sceneColor = texture(RTT0, vUv0);
    float lum = dot(vec3(sceneColor), WsRGB);
    fFragColor = vec4(sceneColor.r * bright, sceneColor.g * bright, sceneColor.b * bright, sceneColor.a);
}