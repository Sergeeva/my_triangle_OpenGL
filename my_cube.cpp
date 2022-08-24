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

#include "check_Error.h"


  
////////////////////////////////////////////

float rot_angle = 45.0f;

glm::vec3 rot_ax(1.0f, 0.5f, 0.0f);

//CallBacks Func

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
      rot_ax.x = 1.0f;
      rot_ax.y = 0.5f;
      rot_ax.z = 0.0f;
      rot_angle = (-1)*45.0f;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
      rot_ax.x = 1.0f;
      rot_ax.y = 0.5f;
      rot_ax.z = 0.0f;
      rot_angle = 45.0f;
    }

    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS){
      rot_ax.x = 0.0f;
      rot_ax.y = 0.5f;
      rot_ax.z = 1.0f;
      rot_angle = 45.0f;
    }

      if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
      rot_ax.x = 0.0f;
      rot_ax.y = 0.5f;
      rot_ax.z = 1.0f;
      rot_angle = (-1)*45.0f;
    }
}

//////////////////////////////////////////////////

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
  glfwMakeContextCurrent(window); //основной контекст окна в этом потоке


  //SET CALLBACKS
  //вызывать на каждое изменение размеров окна
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  //glfwGetFramebufferSize(window, &w, &h);
  //glViewport(0,0,w,h) //<-сообщаем OGL размер окна 
  //(или внутри framebuffer_size_callback) 

  glfwSetKeyCallback(window, key_callback);

  ////////////////////////////////////////////////////////////////////

  
  //GLAD init
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    printf("Fail GLAD init\n");
    return -1;
  }

  printf("OpenGL:: %s; GLSL:: %s \n ", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

  glEnable(GL_DEPTH_TEST); //включаем проверку глубины


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
    //
    0, 1, 4,
    0, 7, 4,
    //
    4, 5, 6,
    4, 6, 7,
    //
    4, 5, 1,
    5, 1, 6,
    //
    1, 6, 2,
    7, 6, 2,
    //
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
   glEnableVertexAttribArray(0); //in vertex_shader: location = 0 

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

    glfwPollEvents(); //проверка "управляющих" событий

////////////////////////////////////////////////////////////////
    //Отрисовка всего
////////////////////////////////////////////////////////////////

    //set state
     glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
    //use state (заполняет экран тем что установлено)
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //|GL_STENSIL_BIT
     

     //glUseProgram(shaderProgram); -->
     myShader.use();

     //Определяем  матрицы:
     //model; view; projection 
     glm::mat4 model = glm::mat4(1.0f);
     model = glm::rotate(model, (float)glfwGetTime()*glm::radians(rot_angle), rot_ax);
     
     glm::mat4 view = glm::mat4(1.0f);
     view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
          
     float sc = 800.0/600.0; //width/height
     glm::mat4 proj = glm::perspective(glm::radians(45.0f),
				       sc,
				       0.1f, 100.0f);

    //glm::ortho(0.0f, 800f, 0.0f, 600.0f, 0.1f, 100.0f)


     //Освещение
     
     
     
     //->>vertex shader
     unsigned int modelLoc = glGetUniformLocation(myShader.shaderProgram, "model");//запрашивает индекс
     glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));//устанавливаем значение по индексу

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
     //glDrawArrays(GL_TRIANGLES, 0, 36); //из буф с вершинами GL_ARRAY_BUFFER
     glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); //отрисовка из буф с индексами GL_ELEMENT_ARRAY_BUFFER
     //////////////////////////////
     //     glBindVertexArray(0);

    //////////////////
     // Переключение буфера
     glfwSwapBuffers(window); // буффер содержит все пиксели окна, 2х буфферная схема
	  //////////////////
   } 
   
 //освобождение ресурсов
 glDeleteBuffers(1, &VBO);
 glDeleteBuffers(1, &VAO);
 glDeleteBuffers(1, &EBO);

 glfwTerminate();

 return 0;
} 
