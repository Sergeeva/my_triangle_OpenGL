//
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "stb_image.h" //tex loader

#include <vector>

#include "Shader.h"

/*
//вершинный шейдер -->shader.vs

const char* vertexShaderSrc = "#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"layout (location = 2) in vec2 aTexCoord;\n"
"out vec3 ourColor;\n"
"out vec3 TexCoord;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 proj;\n"
"void main(){\n"
"gl_Position = proj*view*model*vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"ourColor = aColor;\n"
"TexCoord = aPos;\n"
"}\n\0";

//фрагментный шейдер -->shader.fs
const char* fragmentShaderSrc = "#version 460 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"in vec3 TexCoord;\n"
"uniform samplerCube Tex1;\n"
"void main(){\n"
"FragColor = texture(Tex1, TexCoord)*vec4(ourColor, 0.0f);\n"
"}\n\0";

*/
  
///////////////////////////\0/////////////////


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

/*
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
*/ 
//-->
Shader myShader("my_vertex_shader.glsl", "my_fragment_shader.glsl");


//
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

//filter (scaling)
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


 //Загружаем и создаём текстуры
 int tex_w, tex_h, tex_numCh;
 unsigned char *tex1_data = stbi_load("texture/container.jpg", &tex_w, &tex_h, &tex_numCh, 0);
 
//gen texture: single
 unsigned int tex1_id;
 glGenTextures(1, &tex1_id);
 glBindTexture(GL_TEXTURE_2D, tex1_id);
 glTexImage2D(GL_TEXTURE_2D, 
              0, //mipmap level 
              GL_RGB, 
              tex_w, tex_h, 
              0, //unused 
              GL_RGB, GL_UNSIGNED_BYTE, //format + datatype of the source 
              tex1_data);
 glGenerateMipmap(GL_TEXTURE_2D);
 
 stbi_image_free(tex1_data);

//////////////////////////////////////////////////////////////////////////
//texture: cubemap

unsigned int texcube_id;
glGenTextures(1, &texcube_id);
glBindTexture(GL_TEXTURE_CUBE_MAP, texcube_id);

std::vector<std::string> texcube_faces = 
{
"texture/container_RIGHT.jpg", //right
"texture/container_LEFT.jpg", //left 
"texture/container_TOP.jpg", //top
"texture/container_BOTTOM.jpg", //bottom
"texture/container_FRONT.jpg", //front
"texture/container_BACK.jpg", //back
};

for (unsigned int i=0; i<6; i++){
  tex1_data = stbi_load(texcube_faces[i].c_str(), &tex_w, &tex_h, &tex_numCh, 0);
  if (tex1_data){
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 
                0, //mipmap level 
                GL_RGB, 
                tex_w, tex_h, 
                0, //unused 
                GL_RGB, GL_UNSIGNED_BYTE, //format + datatype of the source 
                tex1_data);
    glGenerateMipmap(GL_TEXTURE_2D); 

  //std::cout<<"w = "<<tex_w<<" h = "<<tex_h<<" numCh = "<<tex_numCh<<std::endl;

  }else{
    std::cout<<"Loading FAILED"<<texcube_faces[i].c_str()<<std::endl;
  }
    stbi_image_free(tex1_data); 
}

//Wrapping and Filtering
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


/////////////////////////////////////////////////////////////////////////////////
// Входные координаты
//Куб имеет 8 уникальных вершин

 float VertexData[] = {
    // pos:3xfloat,    colors:3xfloat;    
   -0.5,  -0.5, -0.5,  1.00, 0.00, 0.00,  //0 
   -0.5,  -0.5,  0.5,  0.00, 1.00, 0.00,  //1 
   0.5,   -0.5,  0.5,  0.00, 0.00, 1.00,  //2
   0.5,   -0.5, -0.5,  0.00, 1.00, 0.00,  //3 
   -0.5,   0.5, -0.5,  0.00, 0.00, 1.00,  //4 
   -0.5,   0.5,  0.5,  0.00, 1.00, 0.00,  //5
   0.5,    0.5,  0.5,  1.00, 0.00, 0.00,  //6
   0.5,    0.5, -0.5,  0.00, 1.00, 0.00,  //7 

 };

/*
 float VertexData[] = {
  // pos:3xfloat,       colors:3xfloat;     texture coords:2xfloat  
  -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 
   0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   1.0f, 0.0f, 
   0.5f, 0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
   0.5f, 0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
  -0.5f, 0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 
  -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 

  -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 0.0f,   0.0f, 0.0f, 
   0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 0.0f,   1.0f, 0.0f, 
   0.5f, 0.5f,  0.5f,  1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
   0.5f, 0.5f,  0.5f,  1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
  -0.5f, 0.5f,  0.5f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f, 
  -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 0.0f,   0.0f, 0.0f, 

  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 
  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 

  0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 
  0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 
  0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
  0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
  0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f, 
  0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 

  -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,   0.0f, 0.0f, 
  -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,   1.0f, 0.0f, 
   0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,   1.0f, 1.0f,
   0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,   1.0f, 1.0f,
   0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,   0.0f, 1.0f, 
  -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,   0.0f, 0.0f, 

  -0.5f, 0.5f, -0.5f,  0.5f, 0.3f, 0.5f,   0.0f, 0.0f, 
  -0.5f, 0.5f,  0.5f,  0.5f, 0.3f, 0.5f,   1.0f, 0.0f, 
   0.5f, 0.5f,  0.5f,  0.5f, 0.3f, 0.5f,   1.0f, 1.0f,
   0.5f, 0.5f,  0.5f,  0.5f, 0.3f, 0.5f,   1.0f, 1.0f,
   0.5f, 0.5f, -0.5f,  0.5f, 0.3f, 0.5f,   0.0f, 1.0f, 
  -0.5f, 0.5f, -0.5f,  0.5f, 0.3f, 0.5f,   0.0f, 0.0f, 
}; 
*/

 //12 треугольников 
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
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   //цвета
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
   glEnableVertexAttribArray(1);

   //текстура
  //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
  //glEnableVertexAttribArray(2);

   
   ////////////////////////////////

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);
   
   /////////////////////////////////


   //цикл рендеринга   
   while(!glfwWindowShouldClose(window)) {

     glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
     //glUseProgram(shaderProgram); -->
     myShader.use();

        //Определяем  матрицы:
     //model; view; projection 
     glm::mat4 model = glm::mat4(1.0f);
     model = glm::rotate(model, (float)glfwGetTime()*glm::radians(45.0f),
		 glm::vec3(1.0f, 0.5f, 0.0f));
     
     glm::mat4 view = glm::mat4(1.0f);
     view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

          
     float sc = 800.0/600.0; //width/height
     glm::mat4 proj = glm::perspective(glm::radians(45.0f),
				       sc,
				       0.1f, 100.0f);

     
     //->>vertex shader
     unsigned int modelLoc = glGetUniformLocation(myShader.shaderProgram, "model");
     glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

     unsigned int viewLoc = glGetUniformLocation(myShader.shaderProgram, "view");
     glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
     
     unsigned int projLoc = glGetUniformLocation(myShader.shaderProgram, "proj");
     glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

    
     glUniform1i(glGetUniformLocation(myShader.shaderProgram, "Tex1"), 0);

     glActiveTexture(GL_TEXTURE0);
     //glBindTexture(GL_TEXTURE_2D, tex1_id);
     glBindTexture(GL_TEXTURE_CUBE_MAP, texcube_id);
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
