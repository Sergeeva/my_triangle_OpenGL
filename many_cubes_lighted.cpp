//
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "Shader.h"
#include "stb_image.h"


////////////////////////////////////////////


void framebuffer_size_callback(GLFWwindow* window, int width, int heigth);

void framebuffer_size_callback(GLFWwindow* window, int width, int heigth){
  glViewport(0,0, width, heigth);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
    	glfwSetWindowShouldClose(window, GL_TRUE);
    }

     if (key == GLFW_KEY_UP && action == GLFW_PRESS){

    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){

    }

    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS){

    }

    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS){

    }
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

  glfwSetKeyCallback(window, key_callback);
  
  //GLAD init
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    printf("Fail GLAD init\n");
    return -1;
  }

  //printf("OpenGL:: %s; GLSL:: %s \n ", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

  glEnable(GL_DEPTH_TEST);

  //Компилируем  шейдеры
  Shader myShader("my_vertex_shader_light.glsl", "my_fragment_shader_light.glsl");

/*
  float VertexData[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
  };
*/


  float VertexData[] = {
    // pos:3xfloat,       colors:3xfloat;     texture coords:2xfloat   normals:3xfloat
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   0.0f,  0.0f, -1.0f, 
    0.5f, 0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   0.0f,  0.0f, -1.0f,  
    0.5f, 0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   0.0f,  0.0f, -1.0f,
    -0.5f, 0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,   0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 0.0f,   0.0f, 0.0f,   0.0f,  0.0f, 1.0f,
    0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 0.0f,   1.0f, 0.0f,    0.0f,  0.0f, 1.0f,
    0.5f, 0.5f,  0.5f,  1.0f, 1.0f, 0.0f,   1.0f, 1.0f,    0.0f,  0.0f, 1.0f,
    0.5f, 0.5f,  0.5f,  1.0f, 1.0f, 0.0f,   1.0f, 1.0f,    0.0f,  0.0f, 1.0f,
    -0.5f, 0.5f,  0.5f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   0.0f,  0.0f, 1.0f, 
    -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 0.0f,   0.0f, 0.0f,   0.0f,  0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,   1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,   0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,

    0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   1.0f,  0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   1.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   1.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   1.0f,  0.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,   0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,   1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,   1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,   1.0f, 1.0f,   0.0f, -1.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,   0.0f, 1.0f,   0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,   0.0f, 0.0f,  0.0f, -1.0f,  0.0f,

    -0.5f, 0.5f, -0.5f,  0.5f, 0.3f, 0.5f,   0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, 0.5f,  0.5f,  0.5f, 0.3f, 0.5f,   1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
    0.5f, 0.5f,  0.5f,  0.5f, 0.3f, 0.5f,   1.0f, 1.0f,   0.0f,  1.0f,  0.0f,
    0.5f, 0.5f,  0.5f,  0.5f, 0.3f, 0.5f,   1.0f, 1.0f,   0.0f,  1.0f,  0.0f,
    0.5f, 0.5f, -0.5f,  0.5f, 0.3f, 0.5f,   0.0f, 1.0f,   0.0f,  1.0f,  0.0f,
    -0.5f, 0.5f, -0.5f,  0.5f, 0.3f, 0.5f,   0.0f, 0.0f,  0.0f,  1.0f,  0.0f
  };



std::vector<glm::vec3> cubePos {
    glm::vec3( 0.0f,  0.0f,  0.0f), //1
    glm::vec3( 2.0f,  5.0f, -15.0f), //2
    glm::vec3(-1.5f, -2.2f, -2.5f),  //3
    glm::vec3(-3.8f, -2.0f, -12.3f),  //4
    glm::vec3( 2.4f, -0.4f, -3.5f),  //5
    glm::vec3(-1.7f,  3.0f, -7.5f),  //6
    glm::vec3( 1.3f, -2.0f, -2.5f),  //7
    glm::vec3( 1.5f,  2.0f, -2.5f)  //8 
  };

// расположение источника света
glm::vec3 lightPos(1.5f, 4.0f, -5.0f);  


//
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

//filter (scaling)
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


 //Загружаем и создаём текстуры
 int tex_w, tex_h, tex_numCh;
 unsigned char *tex1_data = stbi_load("texture/tex1.jpg", &tex_w, &tex_h, &tex_numCh, 0);
 
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

                                                                   //координаты

/*
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

  //нормали
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
*/
   
   //координаты
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (8+3)*sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);

   //цвета
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (8+3)*sizeof(float), (void*)(3*sizeof(float)));
   glEnableVertexAttribArray(1);

   //текстура координаты
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, (8+3)*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

  //нормали
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, (8+3)*sizeof(float), (void*)(8*sizeof(float)));
    glEnableVertexAttribArray(3);
   

   ////////////////////////////////

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);
   
   /////////////////////////////////


   //цикл рендеринга   
   while(!glfwWindowShouldClose(window)) {

     glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     
     myShader.use();

    float radius = 10.0f;

    for(std::size_t i = 0; i<cubePos.size(); ++i){
       //Определяем  матрицы:
      //model; view; projection 
      glm::mat4 model = glm::translate(glm::mat4(1.0f), cubePos[i]);
      model = glm::rotate(model, (float)glfwGetTime()*glm::radians((-1)*15.0f*(i+1)),
			   glm::vec3(1.0f, 0.5f, 0.0f));
     
     //camera

      float camX = sin(glfwGetTime()) * radius;
      float camY = 0.0f;
      float camZ = cos(glfwGetTime()) * radius;
     
      glm::mat4 view = glm::mat4(1.0f);
      //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
      view = glm::lookAt(glm::vec3(camX, camY, camZ), 
                        glm::vec3(0.0, 0.0, 0.0f), 
                        glm::vec3(0.0, 1.0, 0.0));
    
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
     glActiveTexture(GL_TEXTURE0); //text block
     glBindTexture(GL_TEXTURE_2D, tex1_id);


    //-> frag shader
        GLint objectColorLoc = glGetUniformLocation(myShader.shaderProgram, "objectColor");
        GLint lightColorLoc  = glGetUniformLocation(myShader.shaderProgram, "lightColor");
        GLint lightPosLoc    = glGetUniformLocation(myShader.shaderProgram, "lightPos");
        glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
        glUniform3f(lightColorLoc,  1.0f, 1.0f, 1.0f);
        glUniform3f(lightPosLoc,    lightPos.x, lightPos.y, lightPos.z);


      glBindVertexArray(VAO);
      // DrawTriangle :: сборка примитива
      glDrawArrays(GL_TRIANGLES, 0, 36); //

      }
     
     //////////////////////////////
     //     glBindVertexArray(0);
     
     glfwSwapBuffers(window); // буффер содержит все пиксели окна, 2х буфферная схема
     glfwPollEvents(); //проверка "управляющих" событий
	  
   } 
   
 //освобождение ресурсов
 glDeleteBuffers(1, &VBO);
 glDeleteBuffers(1, &VAO);

 glfwTerminate();

 return 0;
} 
