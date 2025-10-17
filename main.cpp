#include "ventana.h"
#include "shader.h"
//#include "./header/gl.h"
#include <iostream>
#include <vector>
#include <string> // Necesario para std::string

//Ventana *ventana;
//Model *model;
Ventana *ventana=nullptr;
Model *model = nullptr;

void finish(){
    if (model) delete model;
    if (ventana) delete ventana;
    glfwTerminate();
    //delete(model);
    //delete(ventana);
    
}

//void init(){
    //ventana = new Ventana(600,600);
    // Inicializar GLFW
    //ventana->initGLFW();
    
    //model = new Model();
    //ventana->initModels(model);
    //ventana->initViewProyection();
//}
void init(const std::vector<std::string>& obj_files){
    ventana = new Ventana(600,600);
    // Inicializar GLFW
    ventana->initGLFW();
    
    // 3. Pasar la lista de archivos a la Ventana para que los gestione
    //    NOTA: Se asume que la clase Ventana tiene un nuevo método para esto.
    ventana->loadOBJFiles(obj_files); // Necesitas crear este método en Ventana.h/.cpp
    //ventana->initModels(model);
    ventana->initViewProyection();

}

//int main() { 
//    init();
//    ventana->render();
//    finish();
//    return 0;
//}

int main(int argc, char* argv[]) {
    //alamcena los nombres de los archivos obj
    std::vector<std::string> obj_files;
    for (int i = 1; i < argc; ++i) {
        obj_files.push_back(argv[i]);
    }
    
    // Llama a la nueva función init con la lista de archivos
    init(obj_files);

    ventana->render();
    finish();

    return 0;
}
