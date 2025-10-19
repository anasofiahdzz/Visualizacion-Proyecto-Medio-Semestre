#include "ventana.h"
#include "shader.h"
//#include "./header/gl.h"
#include <iostream>
#include <vector>
#include <string> // Necesario para std::string

Ventana *ventana=nullptr;
Model *model = nullptr;


void finish(){
    if (model) {
        delete model;   
        model = nullptr;
    }
    if (ventana) {
        delete ventana;
        ventana = nullptr;
    }
    glfwTerminate();
}

void init(const std::vector<std::string> obj_files){
    ventana = new Ventana(600,600);
    // Inicializar GLFW
    ventana->initGLFW();

    model = new Model();

    for (auto& filename : obj_files) {
        if (model->Load(filename)) {
            model->initModel();
        }
    }

    ventana->initModels(model);
    ventana->initViewProyection();
}

int main() {
    //alamcena los nombres de los archivos obj
    std::vector<std::string> obj_files{
        "obj/bunny.obj"
    };

    // Llama a la nueva función init con la lista de archivos
    init(obj_files);

    ventana->render();
    finish();

    return 0;
}



