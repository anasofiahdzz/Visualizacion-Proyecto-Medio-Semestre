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

class Ventana{
    public:
        GLuint width, height;
        GLFWwindow* window;
        glm::mat4 view, projection;

        Model *model;
        //nuevo
        std::vector<std::string> objFileNames;
        size_t currentFileIndex;

        Ventana();
        Ventana(GLuint w, GLuint h);
        //Ventana(unsigned int w, unsigned int h);
        //anteriorres
        void initGLFW();
        //void initGLEW();
        void initModels(Model* m);
        
        void initViewProyection();
        void render();
        void update();
        void idel();
        void display();
        void finish();

        // Nuevo método
        void loadOBJFiles(const std::vector<std::string>& files);


};

#endif