#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <sstream>
#include <type_traits>

#include <type_traits>

namespace shader {

enum class ShaderType {
  VERTEX_SHADER,
  FRAGMENT_SHADER,
};

template <ShaderType S>
class Shader {
  public:
    Shader(const char* shader_source) {
      std::string code;

      std::ifstream shader_file;
      shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      try {
        shader_file.open(shader_source);

        std::stringstream shader_stream;
        shader_stream << shader_file.rdbuf();

        shader_file.close();

        code = shader_stream.str();
      } catch(std::ifstream::failure e) {
        std::cout << "Error::SHADER::FILE_NOT_SUCCESFFULY_READ";
      }

      const char* shader_code_ = code.c_str();

      switch(S) {
        case ShaderType::VERTEX_SHADER:
          shader_data_ = glCreateShader(GL_VERTEX_SHADER);
          break;
        case ShaderType::FRAGMENT_SHADER:
          shader_data_ = glCreateShader(GL_FRAGMENT_SHADER);
          break;
        default:
          std::cout << "Error::SHADER_TYPE_NOT_FOUND";
      }

      glShaderSource(shader_data_, 1, &shader_code_, NULL);
    }

    void delete_shader() {
      glDeleteShader(shader_data_);
    }

    bool compile() {
      glCompileShader(shader_data_);

      GLint success = true;
      glGetShaderiv(shader_data_, GL_COMPILE_STATUS, &success);

      return success;
    }
    void print_logs() {
      glGetShaderInfoLog(shader_data_, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    GLuint get_shader_data_() { return shader_data_; }

  private:
    char infoLog[512];
    GLuint shader_data_;
};

}  // namepsace: shader
