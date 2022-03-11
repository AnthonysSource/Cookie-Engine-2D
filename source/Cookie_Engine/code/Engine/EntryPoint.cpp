#include <stdio.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

int main(){
    // Test implementation to create a window
    GLFWwindow* window;

    if(!glfwInit())
        return -1;
    
    window = glfwCreateWindow(1280, 720, "Cookie Engine", NULL, NULL);

    if(!window){
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}