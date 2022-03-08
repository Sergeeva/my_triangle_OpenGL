#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
out vec3 ourColor;
out vec3 TexCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main(){
     gl_Position = proj*view*model*vec4(aPos.x, aPos.y, aPos.z, 1.0f);
     
     ourColor = aColor;
     TexCoord = aPos;
}


