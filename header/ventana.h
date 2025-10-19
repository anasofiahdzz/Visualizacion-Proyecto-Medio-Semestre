#ifndef VENTANA_H
#define VENTANA_H
#pragma once
#include "gl.h" 
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include "model.h"
#include "shader.h"

class Ventana{
    public:

        GLuint axesVAO, axesVBO;
        Shader* axesShader; 

        GLuint width, height;
        GLFWwindow* window;
        glm::mat4 view, projection;

        Model *model;

        std::vector<std::string> objFileNames;
        size_t currentFileIndex;

        void initAxes(); // Declaración del método
        void drawAxes(); // Declaración del método

        Ventana();
        Ventana(GLuint w, GLuint h);

        void loadOBJFiles(const std::vector<std::string>& files);
        void initGLFW();
        void initModels(Model* m);
        void initViewProyection();
        void render();
        void update();
        void idel();
        void display();
        void finish();
};

#endif