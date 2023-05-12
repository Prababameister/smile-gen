#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>

#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "lib/shaders/shader.hh"
#include "lib/shaders/shader_program.hh"

#define STB_IMAGE_IMPLEMENTATION
#include "lib/textures/std_image.h"

// -- Relevant constants
std::string path_storage;

const unsigned int screen_width = 800;
const unsigned int screen_height = 600;

const std::string proj_head = PROJECT_HEAD;

GLfloat tex_coords[] = {
0.0f, 0.0f,
1.0f, 0.0f,
0.5f, 1.0f,
};

GLfloat vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

glm::vec3 light_pos(1.2f, 1.0f, 2.0f);

float delta_time_val = 0;

glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
void process_input(GLFWwindow *window);

const char* make_abs_path(std::string rel_path) {
  path_storage = proj_head + "/" + rel_path;
  return path_storage.c_str();
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "First-ish Window", NULL, NULL);

  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, mouse_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
  }

  shader::Shader<shader::ShaderType::VERTEX_SHADER> vertex_shader_{make_abs_path("resources/shaders/shader.vert")};
  if(!vertex_shader_.compile())
    vertex_shader_.print_logs();

  shader::Shader<shader::ShaderType::FRAGMENT_SHADER> obj_fragment_shader_{make_abs_path("resources/shaders/obj.frag")};
  if(!obj_fragment_shader_.compile())
    obj_fragment_shader_.print_logs();

  shader::Shader<shader::ShaderType::FRAGMENT_SHADER> light_fragment_shader_{make_abs_path("resources/shaders/light.frag")};
  if(!light_fragment_shader_.compile())
    light_fragment_shader_.print_logs();

  shader::ShaderProgram obj_sp{}, light_sp{};

  obj_sp.attach_shader(vertex_shader_);
  obj_sp.attach_shader(obj_fragment_shader_);
  obj_sp.link_program();

  light_sp.attach_shader(vertex_shader_);
  light_sp.attach_shader(light_fragment_shader_);
  light_sp.link_program();

  vertex_shader_.delete_shader();
  light_fragment_shader_.delete_shader();
  obj_fragment_shader_.delete_shader();

  GLuint VBO, obj_VAO;
  glGenVertexArrays(1, &obj_VAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(obj_VAO);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
  GLuint light_VAO;
  glGenVertexArrays(1, &light_VAO);
  glBindVertexArray(light_VAO);

  // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to intialize GLAD" << std::endl;
    return -1;
  }

  glEnable(GL_DEPTH);

  GLfloat last_time_value = glfwGetTime();
  while(!glfwWindowShouldClose(window)) {
    process_input(window);

    glEnable(GL_DEPTH_TEST);

    // Set a new color for OpenGL to clear the screen with
    glClearColor(0.207f, 0.866f, 0.545f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLfloat time_value = glfwGetTime();
    delta_time_val = time_value - last_time_value;

    glm::vec3 obj_color = glm::vec3(1.0f, 0.5f, 0.31f);
    glm::vec3 light_color = glm::vec3(1.0f, 1.0f, 1.0f);

    // Start of obj Cube
    obj_sp.use_program();

    int light_pos_Loc = glGetUniformLocation(obj_sp.get_shader_program(), "lightPos");
    glUniform3fv(light_pos_Loc, 1, glm::value_ptr(light_pos));

    int view_pos_Loc = glGetUniformLocation(obj_sp.get_shader_program(), "viewPos");
    glUniform3fv(view_pos_Loc, 1, glm::value_ptr(camera_pos));

    int obj_col_Loc = glGetUniformLocation(obj_sp.get_shader_program(), "objectColor");
    glUniform3fv(obj_col_Loc, 1, glm::value_ptr(obj_color));

    int light_col_Loc = glGetUniformLocation(obj_sp.get_shader_program(), "lightColor");
    glUniform3fv(light_col_Loc, 1, glm::value_ptr(light_color));

    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)screen_width/(float)screen_height, 0.1f, 100.0f);

    int modelLoc = glGetUniformLocation(obj_sp.get_shader_program(), "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    int viewLoc = glGetUniformLocation(obj_sp.get_shader_program(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    int projLoc = glGetUniformLocation(obj_sp.get_shader_program(), "proj");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

    glBindVertexArray(obj_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // End of obj Cube

    // Start of light source Cube
    light_sp.use_program();

    light_col_Loc = glGetUniformLocation(light_sp.get_shader_program(), "lightColor");
    glUniform3fv(light_col_Loc, 1, glm::value_ptr(light_color));

    model = glm::mat4(1.0f);
    model = glm::translate(model, light_pos);
    model = glm::scale(model, glm::vec3(0.2f));

    view = glm::mat4(1.0f);
    view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);

    proj = glm::perspective(glm::radians(45.0f), (float)screen_width/(float)screen_height, 0.1f, 100.0f);

    modelLoc = glGetUniformLocation(light_sp.get_shader_program(), "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    viewLoc = glGetUniformLocation(light_sp.get_shader_program(), "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    projLoc = glGetUniformLocation(light_sp.get_shader_program(), "proj");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

    glBindVertexArray(light_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // End of light source Cube

    glfwSwapBuffers(window);
    glfwPollEvents();

    last_time_value = time_value;
  }

  glDeleteVertexArrays(1, &obj_VAO);
  glDeleteVertexArrays(1, &light_VAO);
  glDeleteBuffers(1, &VBO);

  obj_sp.delete_program();
  light_sp.delete_program();

  glfwTerminate();

  return 0;
}

void process_input(GLFWwindow *window) {
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  const float camera_speed = 2.5f * delta_time_val;
  if(glfwGetKey(window, GLFW_KEY_W))
    camera_pos += camera_speed * camera_front;
  if(glfwGetKey(window, GLFW_KEY_S))
    camera_pos -= camera_speed * camera_front;
  if(glfwGetKey(window, GLFW_KEY_A))
    camera_pos -= camera_speed * glm::normalize(glm::cross(camera_front, camera_up));
  if(glfwGetKey(window, GLFW_KEY_D))
    camera_pos += camera_speed * glm::normalize(glm::cross(camera_front, camera_up));
}

bool first_mouse_input = true;
GLdouble last_x = 400, last_y = 300;
GLfloat yaw = -90.0f, pitch = 0.0f;
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos) {
  if (first_mouse_input) {
    last_x = x_pos;
    last_y = y_pos;
    first_mouse_input = false;
  }

  float x_off = x_pos - last_x;
  float y_off = last_y - y_pos;
  last_x = x_pos;
  last_y = y_pos;

  float sens = 0.1f;
  x_off *= sens;
  y_off *= sens;

  yaw += x_off;
  pitch += y_off;

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

  camera_front = glm::normalize(direction);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glad_glViewport(0, 0, width, height);
}
