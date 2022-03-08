
#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragPath){

//reading shader sources from files

std::string vertexCode;
std::string fragCode;

const char* vertexShaderSrc;
const char* fragmentShaderSrc;


std::ifstream vShaderFile;
std::ifstream fShaderFile;

try{

vShaderFile.open(vertexPath);
fShaderFile.open(fragPath);

std::stringstream vShaderStream;
vShaderStream << vShaderFile.rdbuf();
std::stringstream fShaderStream;
fShaderStream << fShaderFile.rdbuf();

vShaderFile.close();
fShaderFile.close();

vertexCode = vShaderStream.str();
fragCode = fShaderStream.str();

}catch(std::ifstream::failure e){
    std::cout << "ERROR in OPEN SHADER SOURCES" << std::endl;
}

vertexShaderSrc = vertexCode.c_str();
fragmentShaderSrc = fragCode.c_str();

//std::cout<<vertexShaderSrc<<std::endl;
//std::cout<<fragmentShaderSrc<<std::endl;

//Компилируем  шейдеры

 //Вершинный
 int vertexShader = glCreateShader(GL_VERTEX_SHADER);
 glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
 glCompileShader(vertexShader);

 //Фрагментный
 int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
 glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
 glCompileShader(fragmentShader);

 
 //Проверяем ошибки компиляции
 int success = 0;
 char infoLog[512];
 
 glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
 if(!success){
   glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
   std::cout << "ERROR VERTEX SHADER COMPILATION: \n" << infoLog <<"\n";
 }

 glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
 if(!success){
   glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
   std::cout << "ERROR FRAGMENT SHADER COMPILATION: \n" << infoLog <<"\n";
 }

 //Связывание шейдеров в шейдерную программу
 shaderProgram = glCreateProgram();
 glAttachShader(shaderProgram, vertexShader);
 glAttachShader(shaderProgram, fragmentShader); 
 glLinkProgram(shaderProgram);

 //Проверка ошибок связывания
 glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
 if(!success){
   glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
   std::cout<<"ERROR SHADER PROGRAMM LINKING: \n" <<infoLog << "\n";
 }
 
//теперь шейдеры можно удалить
 glDeleteShader(vertexShader);
 glDeleteShader(fragmentShader);

 std::cout<<"Shader CREATED"<<std::endl;

}


void Shader::use(){
  glUseProgram(shaderProgram);   
}