//
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>


//вершинный шейдер -->shader.vs

const char* vertexShaderSrc = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 proj;\n"
"void main(){\n"
"gl_Position = proj*view*model*vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
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

  glEnable(GL_DEPTH_TEST);

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
//Куб имеет 8 уникальных вершин
 float VertexData[] = {
    // pos:3xfloat, colors:3xfloat; texture coords:2xfloat  
   -0.5,  -0.5, -0.5,  1.00, 0.00, 0.00, 0.00, 0.00, //0
   -0.5,  -0.5,  0.5,  0.00, 1.00, 0.00, 0.00, 1.00, //1
   0.5,   -0.5,  0.5,  0.00, 0.00, 1.00, 1.00, 1.00, //2
   0.5,   -0.5, -0.5,  0.00, 1.00, 0.00, 1.00, 0.00, //3
   -0.5,   0.5, -0.5,  0.00, 0.00, 1.00, 0.00, 0.00, //4
   -0.5,   0.5,  0.5,  0.00, 1.00, 0.00, 0.00, 1.00, //5
   0.5,    0.5,  0.5,  1.00, 0.00, 0.00, 1.00, 1.00, //6
   0.5,    0.5, -0.5,  0.00, 1.00, 0.00, 1.00, 0.00  //7

 };

 //12 трекгольников 
  unsigned int ind[] = {
    //передняя сторона
    0, 1, 2, 
    0, 2, 3, 
    0, 1, 4,
    0, 7, 4,
    4, 5, 6,
    4, 6, 7,
    4, 5, 1,
    5, 1, 6,
    1, 6, 2,
    7, 6, 2,
    7, 2, 3,
    0, 7, 3
  };

 // ---> Размещаются в видеопамяти
  unsigned int VBO, VAO; // Vertex Buffer Object
 // 1. генерация ID
   glGenBuffers(1, &VBO);
   glGenVertexArrays(1, &VAO);

 // 2. связывание
   glBindVertexArray(VAO); //связываем вершинный массив

   glBindBuffer(GL_ARRAY_BUFFER, VBO);
 // 3. копирование туда вершин
   glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
                                                                //GL_STREAM_DRAW
                                                                //GL_DYNAMIC_DRAW
   //
   unsigned int EBO;
   glGenBuffers(1, &EBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   // --> индексы
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);
   
   //координаты
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   //цвета
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
   glEnableVertexAttribArray(1);

   //текстура
   //   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
   //   glEnableVertexAttribArray(2);

   
   ////////////////////////////////

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);
   
   /////////////////////////////////


   //цикл рендеринга   
   while(!glfwWindowShouldClose(window)) {

     glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
     glUseProgram(shaderProgram);

        //Определяем  матрицы:
     //model; view; projection 
     glm::mat4 model = glm::mat4(1.0f);
     model = glm::rotate(model, (float)glfwGetTime()*glm::radians(-50.0f),
			 glm::vec3(1.0f, 0.5f, 0.0f));
     
     glm::mat4 view = glm::mat4(1.0f);
     view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

          
     float sc = 800.0/600.0; //width/height
     glm::mat4 proj = glm::perspective(glm::radians(45.0f),
				       sc,
				       0.1f, 100.0f);

     
     //->>vertex shader
     unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
     glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

     unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
     glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
     
     unsigned int projLoc = glGetUniformLocation(shaderProgram, "proj");
     glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));



     glBindVertexArray(VAO);
     // DrawTriangle :: сборка примитива
     //glDrawArrays(GL_TRIANGLES, 0, 36); //
     glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
     //////////////////////////////
     //     glBindVertexArray(0);
     
     glfwSwapBuffers(window); // буффер содержит все пиксели окна, 2х буфферная схема
     glfwPollEvents(); //проверка "управляющих" событий
	  
   } 
   
 //освобождение ресурсов
 glDeleteBuffers(1, &VBO);
 glDeleteBuffers(1, &VAO);
 glDeleteBuffers(1, &EBO);

 glfwTerminate();

 return 0;
} 
