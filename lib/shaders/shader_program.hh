#pragma once

#include "shader.hh"

namespace shader {

  class ShaderProgram {
    public:
      ShaderProgram() { shader_program_ = glCreateProgram(); };

      template <ShaderType S>
      void attach_shader(Shader<S> shader_) { glAttachShader(shader_program_, shader_.get_shader_data_()); };

      void link_program() { glLinkProgram(shader_program_); };
      void use_program() { glUseProgram(shader_program_); };
      void delete_program() { glDeleteProgram(shader_program_); };

      GLuint get_shader_program() { return shader_program_; };
    private:
      GLuint shader_program_;
  };

}  // namespace: shader
