#version 330 core

layout(location=0) in vec3 aPos;    // position
layout(location=1) in vec3 aColor;  // color

out vec3 vColor;

uniform mat4 uMVP;

void main()
{
    vColor = aColor;
    gl_Position = uMVP * vec4(aPos, 1.0);
}
