
#include "../header/ventana.h"
#include <iostream>
Ventana::Ventana(){
    width = 800;
    height = 600;
}

Ventana::Ventana(GLuint w, GLuint h){
    width = w;
    height = h;
}

// para declarar extern la función de inicialización de GALOGEN 
int gl_init();

void Ventana::initGLFW()
{
    if (!glfwInit()) {
        std::cerr << "Error al inicializar GLFW" << std::endl;
        return ;
    }

    // Crear una ventana
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Vizualizador", nullptr, nullptr);
    if (!window) {
        std::cerr << "Error al crear la ventana" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    // inicializacion de galogen
    if (!gl_init()) { // desde la fun gl_init()
        std::cerr << "Error al inicializar GALOGEN (OpenGL Function Loader)" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }

    // configuarando opengl
    glEnable(GL_DEPTH_TEST);  // Prueba de profundidad (requerido para 3D)
    glEnable(GL_CULL_FACE);   // Habilitar culling de caras
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void Ventana::loadOBJFiles(const std::vector<std::string>& files) {
    if (files.empty()) {
        std::cerr << "Error: No se proporcionaron archivos OBJ para cargar." << std::endl;
            return;
    }   
    
    this->objFileNames = files; // Almacena todos los nombres de archivo
    this->currentFileIndex = 0; // Empieza en el primer archivo
            
    // se crea el primer modelo.
    // se carga el primer modelo:

    this->model = new Model(); // Crea la instancia del modelo
            
    // Llama a la lógica de Model para cargar el primer archivo
    if (!this->model->Load(this->objFileNames[this->currentFileIndex])) { 
    std::cerr << "Error al cargar el archivo: " << this->objFileNames[this->currentFileIndex] << std::endl;
    // Manejar error (tal vez terminar el programa o pasar al siguiente)
    }
}

void Ventana::initModels(Model* m)
{
    model = m;
    model->initModel();
}

void Ventana::initViewProyection(){
     // Configurar matrices de transformación (model, view, projection)
    view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 5.0f),  // posición de la cámara
        glm::vec3(0.0f, 0.0f, 0.0f),  // hacia dónde mira
        glm::vec3(0.0f, 1.0f, 0.0f)   // vector "arriba"
    );

    projection = glm::perspective(
        glm::radians(45.0f),
        (float)width / (float)height,
        0.1f,
        100.0f
    );
}

void Ventana::update(){
 // Calcular la rotación del cubo
    model->updateModel(glfwGetTime());
}
void Ventana::idel(){}

void Ventana::display() {}

// src/ventana.cpp (Definición global o como const en el .cpp)
float axis_vertices[] = {
    // Posición X, Y, Z, R, G, B
    0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f, // Eje X (Rojo)
    2.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,

    0.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f, // Eje Y (Verde)
    0.0f, 2.0f, 0.0f,   0.0f, 1.0f, 0.0f,

    0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f, // Eje Z (Azul)
    0.0f, 0.0f, 2.0f,   0.0f, 0.0f, 1.0f
};

// src/ventana.cpp
void Ventana::initAxes() {
    // shader simple solo usa Posición y Color para los ejes.
    axesShader = new Shader("./shader/model.vert", "./shader/model.frag");
    
    glGenVertexArrays(1, &axesVAO);
    glGenBuffers(1, &axesVBO);

    glBindVertexArray(axesVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axis_vertices), axis_vertices, GL_STATIC_DRAW);

    // Atributo 0: Posición (3 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Atributo 3: Color (3 floats)
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Ventana::drawAxes() {
    if (this->axesShader && this->axesVAO) {
        this->axesShader->use();
        
        // Enviar matrices de cámara (View y Projection)
        this->axesShader->setMat4x4("view", view);
        this->axesShader->setMat4x4("projection", projection);
        
        // Matriz Model para los ejes es identidad (no rotan)
        glm::mat4 identity = glm::mat4(1.0f);
        this->axesShader->setMat4x4("model", identity);

        // Envía y_min = y_max = 0.0f para indicarle al shader que dibuje los ejes (color fijo)
        this->axesShader->setFloat("y_min", 0.0f); 
        this->axesShader->setFloat("y_max", 0.0f);
        
        glBindVertexArray(this->axesVAO);
        glLineWidth(2.0f); 
        glDrawArrays(GL_LINES, 0, 6); // 6 vértices = 3 líneas
        glBindVertexArray(0);
    }
}

void Ventana::render() {
    // llamar a update() antes de dibujar si se quiere una rotación continua
    
    while (!glfwWindowShouldClose(window)) {
        update(); // Llama a la rotación del modelo
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Fondo oscuro
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // dubujar modelo obj
        if (model) { 
            if (!model->shader) {
                std::cerr << "Error: El modelo no tiene shader inicializado.\n";
                break; //return;
            }

            // Configurar color del objeto antes de dibujarlo
            model->shader->use();
            model->shader->setVec3("objectColor", glm::vec3(1.0f, 0.8f, 0.7f)); // Rosado claro

            model->renderModel(view, projection); // model->draw() llama a renderModel internamente
        } else {
            std::cerr << "Advertencia: modelo no cargado.\n";
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}