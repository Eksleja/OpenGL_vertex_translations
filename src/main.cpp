#include "config.h"
#include "triangle_mesh.h"
#include "linAlgebra.h"


// vvvvvvvvvvvvvvvvvvvvvv pretty much boiler plate vvvvvvvvvvvvvvvvvvvvvv
unsigned int make_module(const std::string& filepath, unsigned int module_type);

unsigned int make_shader(const std::string& vertex_filepath,
        const std::string& fragment_filepath) {

	//To  store all the shader modules
	std::vector<unsigned int> modules;

	//Add a vertex shader module
	modules.push_back(make_module(vertex_filepath, 
                GL_VERTEX_SHADER));

	//Add a fragment shader module
	modules.push_back(make_module(fragment_filepath, 
                GL_FRAGMENT_SHADER));

	//Attach all the modules then link the program
	unsigned int shader = glCreateProgram();
	for (unsigned int shaderModule : modules) {
		glAttachShader(shader, shaderModule);
	}
	glLinkProgram(shader);

	//Check the linking worked
	int success;
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
		char errorLog[1024];
		glGetProgramInfoLog(shader, 1024, 
                NULL, errorLog);
		std::cout << "Shader linking error:\n" << errorLog << '\n';
	}

	//Modules are now unneeded and can be freed
	for (unsigned int shaderModule : modules) {
		glDeleteShader(shaderModule);
	}

	return shader;
}

unsigned int make_module(const std::string& filepath, unsigned int module_type) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cout << "Could not open shader: " << filepath << std::endl;
        return 0;
    }

    std::stringstream bufferedLines;
    bufferedLines << file.rdbuf();
    std::string shaderSource = bufferedLines.str();
    const char* shaderSrc = shaderSource.c_str();

    std::cout << "Compiling shader: " << filepath << std::endl; // <--- Debug

    unsigned int shaderModule = glCreateShader(module_type);
    glShaderSource(shaderModule, 1, &shaderSrc, NULL);
    glCompileShader(shaderModule);

    int success;
    glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
    if (!success) {
        char errorLog[1024];
        glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
        std::cout << "Shader Module compilation error in " << filepath << ":\n"
                  << errorLog << std::endl;
    }

    return shaderModule;
}
// ^^^^^^^^^^^^^^^^^^^^^^^^ pretty much boiler plate ^^^^^^^^^^^^^^^^^^^^^^^^^





int main() {
	
	// ########################### pretty much boiler plate ###########################
	GLFWwindow* window;

	if (!glfwInit()) {
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	
	window = glfwCreateWindow(640, 480, "Hello Window!", 
            NULL, NULL);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Couldn't load opengl" << std::endl;
		glfwTerminate();
		return -1;
	}

	// ########################### pretty much boiler plate ###########################

	

	// vvvvvvvvvvvv----- CUSTOMIZE STUFF BELOW HERE -----vvvvvvvvvvvv

	glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
	TriangleMesh* triangle = new TriangleMesh();

	unsigned int shader = make_shader(
		"../src/shaders/vertex.glsl",
		"../src/shaders/fragment.glsl"
	);

	glUseProgram(shader);
	vector3 triangle_position = {-0.0f, 0.0f, 0.0f};
	//vector3 camera_pos ={-0.4f, 0.0f, 0.2f};
	vector3 camera_pos = {-3.0f, 0.0f, 1.5f};
	vector3 camera_target = {0.0f, 0.0f, 0.0f};
	//matrix4 model = create_matrix_transform(triangle_position);
	unsigned int model_location_on_GPU = glGetUniformLocation(shader, "model");
	unsigned int view_location = glGetUniformLocation(shader, "view");
	unsigned int proj_location = glGetUniformLocation(shader, "projection");
	//glUniformMatrix4fv(model_location_on_GPU, 1, GL_FALSE, model.entries);

	matrix4 view = create_look_at(camera_pos, camera_target);
	glUniformMatrix4fv(view_location, 1, GL_FALSE, view.entries);

	matrix4 projection = create_perspective_projection(45.0f, 640.0f/480.0f, 0.1f, 10.0f);
	glUniformMatrix4fv(proj_location,1,GL_FALSE,projection.entries);


float rotationX = 0.0f;
float rotationY = 0.0f;
float rotationZ = 0.0f;
float rotationSpeed = 60.0f; // degrees per second

float lastTime = glfwGetTime();

while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    float currentTime = glfwGetTime();
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    // --- WASD Control ---
if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) rotationX += rotationSpeed * deltaTime;
if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) rotationX -= rotationSpeed * deltaTime;

if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) rotationY += rotationSpeed * deltaTime;
if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) rotationY -= rotationSpeed * deltaTime;

if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) rotationZ += rotationSpeed * deltaTime;
if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) rotationZ -= rotationSpeed * deltaTime;


    // --- Update model matrix with both rotations ---
matrix4 model = rotate_and_translate_allAtOnce(triangle_position, rotationX, rotationY, rotationZ);
glUniformMatrix4fv(model_location_on_GPU, 1, GL_FALSE, model.entries);


    // --- Clear and Draw ---
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shader);
    triangle->draw();
    glfwSwapBuffers(window);
}


	glDeleteProgram(shader);
	delete triangle;
	glfwTerminate();
	return 0;
}