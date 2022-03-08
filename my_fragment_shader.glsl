#version 460 core

out vec4 FragColor;
in vec3 ourColor;
in vec3 TexCoord;
uniform samplerCube Tex1;

void main(){
     FragColor = texture(Tex1, TexCoord);//*vec4(ourColor, 0.0f);
     //FragColor = vec4(ourColor, 0.0f);

}
