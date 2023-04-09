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
    // positions          // colors           // texture coords
    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top left
     0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // top left
     0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
};
GLuint indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3, // second triangle
    5, 4, 6,
    4, 7, 6,
    2, 3, 7,
    2, 6, 7,
    0, 1, 4,
    4, 5, 1,
    0, 3, 4,
    7, 3, 4,
    5, 2, 6,
    5, 2, 1,
};
glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

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

  shader::Shader<shader::ShaderType::FRAGMENT_SHADER> fragment_shader_{make_abs_path("resources/shaders/shader.frag")};
  if(!fragment_shader_.compile())
    fragment_shader_.print_logs();

  shader::ShaderProgram shader_program_{};

  shader_program_.attach_shader(vertex_shader_);
  shader_program_.attach_shader(fragment_shader_);
  shader_program_.link_program();

  vertex_shader_.delete_shader();
  fragment_shader_.delete_shader();

  GLuint VAO;
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);

  GLuint VBO;
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  GLuint EBO;
  glGenBuffers(1, &EBO);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(GL_ARRAY_BUFFER);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to intialize GLAD" << std::endl;
    return -1;
  }

  GLuint texture1, texture2;

  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  GLfloat border_color[] = {1.0f, 1.0f, 0.0f, 1.0f};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

  int width, height, nr_channels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(make_abs_path("resources/textures/pop_cat_close.png"), &width, &height, &nr_channels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

  data = stbi_load(make_abs_path("resources/textures/pop_cat_open.png"), &width, &height, &nr_channels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  shader_program_.use_program();
  glUniform1i(glGetUniformLocation(shader_program_.get_shader_program(), "texture1"), 0);
  glUniform1i(glGetUniformLocation(shader_program_.get_shader_program(), "texture2"), 1);

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
    GLuint u_time_location = glGetUniformLocation(shader_program_.get_shader_program(), "u_time");
    glUniform1f(u_time_location, time_value);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    shader_program_.use_program();
    glBindVertexArray(VAO);

    for(unsigned int i = 0; i < 10; ++i) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      model = glm::rotate(model, glm::radians(time_value * 30.0f), glm::vec3(1.0f, 0.3f, 0.5f));

      glm::mat4 view = glm::mat4(1.0f);
      view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);

      glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)screen_width/(float)height, 0.1f, 100.0f);

      int modelLoc = glGetUniformLocation(shader_program_.get_shader_program(), "model");
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

      int viewLoc = glGetUniformLocation(shader_program_.get_shader_program(), "view");
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

      int projLoc = glGetUniformLocation(shader_program_.get_shader_program(), "proj");
      glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

      glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();

    last_time_value = time_value;
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  shader_program_.delete_program();

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
