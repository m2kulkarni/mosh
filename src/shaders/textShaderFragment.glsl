#version 460 core
in vec2 TexCoords;
in vec2 FragPos;
flat in int isText;

out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;
uniform vec3 backgroundColor;


void main()
{
   if (isText == 0)
   {
      color = vec4(backgroundColor, 1.0);
   }
   else
   {
      color = vec4(backgroundColor, 1.0);
      float distance = texture(text, TexCoords).r;
      float smoothWidth = 0.5;
      float alpha = smoothstep(0.5 - smoothWidth, 0.5 + smoothWidth, distance);

      color = mix(color, vec4(textColor, 1.0), alpha);
   }
}
