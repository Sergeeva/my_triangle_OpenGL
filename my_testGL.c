//
//

#include <stdio.h>
#include <stdlib.h>

//#include <GL/gl3w.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include <GL/glx.h>
//#include <GL/glu.h>

//#include <X11/X.h>
//#include <X11/Xlib.h>

//#include <iostream>

int main(int argc, char *argv[]) {

  if(!glfwInit()){
    printf("glfwInit ERROR! \n");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL){
    //std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    printf("Fail GLAD init\n");
    return -1;
  }
  //  if(!gl3wInit()){
  //    printf("gl3wInit ERROR! \n");
  //  }

  //  if(!gl3wIsSupported(4,6)){
  //    printf("OPEN GL 4.6 not supported! \n");
  //  }
  
  printf("OpenGL:: %s; GLSL:: %s \n ", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
  
  
} 
