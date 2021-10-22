//
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>


//вершинный шейдер -->shader.vs

const char* vertexShaderSrc = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main(){\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"ourColor = aColor;\n"
"}\n\0";



//фрагментный шейдер -->shader.fs
const char* fragmentShaderSrc = "#version 460 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main(){\n"
"FragColor = vec4(ourColor, 1.0f);\n"
"}\n\0";

  


////////////////////////////////////////////


void framebuffer_size_callback(GLFWwindow* window, int width, int heigth);

void framebuffer_size_callback(GLFWwindow* window, int width, int heigth){
  glViewport(0,0, width, heigth);
}

int main(int argc, char *argv[]) {
  
  if(!glfwInit()){
    printf("glfwInit ERROR! \n");
  }

  //GL Context
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "My TRIANGLE", NULL, NULL);
  if (window == NULL){
    //std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  //вызывать на каждое изменение размеров окна
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  
  //GLAD init
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    printf("Fail GLAD init\n");
    return -1;
  }

  printf("OpenGL:: %s; GLSL:: %s \n ", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));


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
 int shaderProgram = glCreateProgram();
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


 
// Входные координаты
  float VertexData[] = {
    0.00,  0.75,  0.00, 1.00, 0.00, 0.00,
    -0.75, -0.75, 0.00, 0.00, 1.00, 0.00,
    0.75,  -0.75, 0.00, 0.00, 0.00, 1.00
  };
 
 // ---> Размещаются в видеопамяти
  unsigned int VBO, VAO; // Vertex Buffer Object
 // 1. генерация ID
   glGenBuffers(1, &VBO);
   glGenVertexArrays(1, &VAO);

 // 2. связывание
   glBindVertexArray(VAO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
 // 3. копирование туда вершин
   glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
                                                                //GL_STREAM_DRAW
                                                                //GL_DYNAMIC_DRAW

   //координаты
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   //цвета
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
   glEnableVertexAttribArray(1);

   ////////////////////////////////

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   //   glBindVertexArrays(0);
   
   
   //цикл рендеринга   
   while(!glfwWindowShouldClose(window)) {

     glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
     glClear(GL_COLOR_BUFFER_BIT);
     
     glUseProgram(shaderProgram);
     glBindVertexArray(VAO);
     // DrawTriangle :: сборка примитива
     glDrawArrays(GL_TRIANGLES, 0, 3);
     //////////////////////////////

     glfwSwapBuffers(window); // буффер содержит все пиксели окна, 2х буфферная схема
     glfwPollEvents(); //проверка "управляющих" событий
	  

   } 
   
 //освобождение ресурсов
 glDeleteBuffers(1, &VBO);
 glDeleteBuffers(1, &VAO);


 glfwTerminate();

 return 0;
} 
