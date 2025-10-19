#include "../header/model.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <limits> // Para std::numeric_limits

Model::Model()
{
    // Inicializa punteros a nulo si son miembros de la clase
    VAO = 0;
    VBO = 0;
    EBO = 0;
    shader = nullptr;
    // Inicializar el bounding box
    minY = std::numeric_limits<float>::max();
    maxY = std::numeric_limits<float>::lowest();
}

void Model::initModel()
{
    modelmat = glm::mat4(1.0f);
    modelmat = glm::translate(modelmat, glm::vec3(0.0f, -1.0f, 0.0f));
    modelmat = glm::scale(modelmat, glm::vec3(0.5f)); // reduce tamaño

    shader = new Shader("./shader/model.vert", "./shader/model.frag");
    shader->use();

    if (verticesVector.empty()) {
        std::cerr << "Error: verticesVector está vacío antes de crear buffers.\n";
        return;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    // Enviar los datos reales de los vértices al GPU
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesVector.size() * sizeof(float), verticesVector.data(), GL_STATIC_DRAW);

    // layout (0,1,2)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // Desvincular el VAO
    std::cout << "Modelo cargado y buffer inicializado." << std::endl;
}

void Model::updateModel(float timeValue){
    angle = timeValue * glm::radians(0.1f); // 0.5 grados por segundo
    modelmat = glm::rotate(modelmat, angle, glm::vec3(0.5f, 1.0f, 0.0f)); // Rotar alrededor de (0.5, 1.0, 0.0)
}

void Model::renderModel(glm::mat4 view, glm::mat4 projection){       
    shader->use();

    // Enviar las matrices al shader
    shader->setMat4x4("model", modelmat);
    shader->setMat4x4("view", view);
    shader->setMat4x4("projection", projection);

    // Pasar los mínimos y máximos de Y al shader
    shader->setFloat("y_min", minY); 
    shader->setFloat("y_max", maxY);
        
    // Dibujar el modelo
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, verticesVector.size() / 8); // Dibuja como nube de puntos para probar la carga
}

Model::~Model() {
    // Libera solo si no se liberó antes
    if (shader) {
        delete shader;
        shader = nullptr;
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}


void Model::finish(){       
    std::cout << "Finalizando modelo..." << std::endl;
}

bool Model::Load(const std::string& filename) {
    // cargar el modelo desde el archivo .obj
    std::cout << "Intentando cargar modelo: " << filename << std::endl;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "No se puede abrir el archivo: " << filename << std::endl;
        return false;
    }

    // Listas temporales para almacenar los datos crudos del OBJ
    std::vector<glm::vec3> temp_positions;
    // std::vector<glm::vec2> temp_uvs;     // No se usan por ahora
    // std::vector<glm::vec3> temp_normals;  // No se usan por ahora

    std::string line;
    int numVertices = 0;
    
    // Inicializar Bounding Box (para la cámara y el shader)
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float minZ = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();
    float maxZ = std::numeric_limits<float>::lowest();

    // Limpiar el vector final para el nuevo modelo
    verticesVector.clear(); 

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string line_header;
        ss >> line_header;

        if (line_header == "v") { // Vértices (Posiciones)
            glm::vec3 vertex;
            ss >> vertex.x >> vertex.y >> vertex.z;
            temp_positions.push_back(vertex);
            
            // CÁLCULO DE BOUNDING BOX
            minX = glm::min(minX, vertex.x); maxX = glm::max(maxX, vertex.x);
            minY = glm::min(minY, vertex.y); maxY = glm::max(maxY, vertex.y);
            minZ = glm::min(minZ, vertex.z); maxZ = glm::max(maxZ, vertex.z);

        } else if (line_header == "f") { // Caras (Faces)
            // LÓGICA CLAVE: PARSEAR ÍNDICES Y CONSTRUIR EL VERTEX BUFFER FINAL
            
            // Asumiendo formato simple: f v1 v2 v3 ...
            // Y asumiendo que queremos dibujar triángulos (GL_TRIANGLES)
            // Aquí debes parsear los índices (v/vt/vn) y expandir el modelo a una lista de vértices.
            
            // --- SIMPLIFICACIÓN PARA PRUEBAS (SOLO NUBE DE PUNTOS) ---
            // Para la primera prueba, solo cargaremos la data 'v' y usaremos glDrawArrays(GL_POINTS)
            // Para 'GL_TRIANGLES', debes implementar un 'triangulador' o parser completo.

        }
    }
    
    // Asignar los valores del Bounding Box
    this->minY = minY;
    this->maxY = maxY;
    
    // Rellenar verticesVector para que initModel() pueda crear los buffers.
    // Color/TexCoord (3+3+2 = 8 floats)
    for (const auto& pos : temp_positions) {
        // Posición (x, y, z)
        verticesVector.push_back(pos.x);
        verticesVector.push_back(pos.y);
        verticesVector.push_back(pos.z);
        // Color (R, G, B) - Relleno con 0 para que el shader lo maneje
        verticesVector.push_back(0.0f);
        verticesVector.push_back(0.0f);
        verticesVector.push_back(0.0f);
        // TexCoord (u, v) - Relleno con 0
        verticesVector.push_back(0.0f);
        verticesVector.push_back(0.0f);
        numVertices++;
    }

    //Desplegar información en la terminal
    std::cout << "--- OBJ Cargado ---" << std::endl;
    std::cout << "Nombre: " << filename << std::endl;
    std::cout << "Num. Vértices: " << numVertices << std::endl;
    std::cout << "Mínimos (X,Y,Z): (" << minX << ", " << minY << ", " << minZ << ")" << std::endl;
    std::cout << "Máximos (X,Y,Z): (" << maxX << ", " << maxY << ", " << maxZ << ")" << std::endl;
    
    file.close();

    // Llama a la inicialización de OpenGL
    initModel();
    
    return true; //temporal
}

void Model::draw() {
    if (!shader) {
        std::cerr << "Error: shader no inicializado antes de dibujar.\n";
        return;
    }

    shader->use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, verticesVector.size() / 8);
    glBindVertexArray(0);
}

