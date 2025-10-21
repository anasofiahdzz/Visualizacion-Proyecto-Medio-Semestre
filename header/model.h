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

    //std::string nombreModelo; // Necesario para la lógica de escalado en updateModel()

    //bounding box para el shader de color
    float minY;
    float maxY;

    //miembros ya existentes
    std::string nombreModelo; // <-- NUEVO: Para aplicar la escala correcta
    float angle;

    glm::mat4 modelmat;
    glm::mat4 baseModelmat;  // matriz base con traslación + escala inicial
    //std::string nombreModelo;
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