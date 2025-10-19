#pragma once
#include "gl.h" 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include "../header/shader.h"

class Model{
public:
    Shader* shader;

    std::vector<float> verticesVector; // Almacena los datos finales para el VBO (pos/color/tex)

    //bounding box para el shader de color
    float minY;
    float maxY;

    //miembros ya existentes
    float angle;
    glm::mat4 modelmat;
    GLuint VAO, VBO, EBO;

    Model();

    // Devuelve 'true' si la carga es exitosa, 'false' si falla
    bool Load(const std::string& filename);

    void initModel();
    void renderModel(glm::mat4 view, glm::mat4 projection);
    void updateModel(float timeValue);
    ~Model();
    void finish();
    void draw();
};