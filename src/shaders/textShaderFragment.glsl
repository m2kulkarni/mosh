#version 460 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;
uniform vec3 backgroundColor;

void main()
{
   float distance = texture(text, TexCoords).r;
   float smoothWidth = 0.5;
   float alpha = smoothstep(0.5 - smoothWidth, 0.5 + smoothWidth, distance);

   color = vec4(mix(backgroundColor, textColor, alpha), 1.0);
}
