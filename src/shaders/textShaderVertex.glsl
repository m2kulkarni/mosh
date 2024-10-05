#version 460 core
layout (location = 0) in vec4 vertex;
layout (location = 1) in vec4 backgroundPos;

out vec2 TexCoords;
out vec2 FragPos;
flat out int isText;

uniform mat4 projection;
uniform int isTextUniform;

void main()
{
    if (isTextUniform == 1)
    {
        gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
        TexCoords = vertex.zw;
        isText = 1;
    }
    else
    {
        gl_Position = projection * vec4(backgroundPos.xy, 0.0, 1.0);
        isText = 0;
    }
    FragPos = vec2(gl_Position.xy);
}
