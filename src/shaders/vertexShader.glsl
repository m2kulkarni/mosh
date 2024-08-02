#version 460 core

struct transformation {
    mat4 rotate;
    mat4 scale;
    mat4 translate;
};

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
uniform transformation transform;
out vec3 vertexColor;

void main()
{
   gl_Position = transform.scale*transform.translate*transform.rotate*vec4(aPos, 1.0f);
   vertexColor = aColor;
};

