#include <shader.h>
#include <utils.h>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string &vsPath, const std::string &fsPath) {
  init(vsPath, fsPath);
}

Shader::Shader(const std::string &vsPath, const std::string &fsPath, const std::string &gsPath) {
  init(vsPath, fsPath, gsPath);
}

Shader::~Shader() {
  glDeleteProgram(id);
}
void Shader::init(const std::string &vsPath, const std::string &fsPath, const std::string &gsPath) {
  if(gsPath=="") initWithCode(getCodeFromFile(vsPath), getCodeFromFile(fsPath));
  else initWithCode(getCodeFromFile(vsPath), getCodeFromFile(fsPath), getCodeFromFile(gsPath));
}

void Shader::initWithCode(const std::string &vs, const std::string &fs, const std::string &gs) {
// TODO: filling this function to set shaders to OpenGL
  const char* vscode= vs.c_str();
  unsigned int vertexShader;
  int success;
  char infoLog[512];
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vscode, NULL);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success)
  {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  };

  unsigned int fragmentShader;
  const char* fscode= fs.c_str();
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fscode, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if(!success)
  {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  };

  unsigned int geometricShader;
  if(gs!=""){
    const char* gscode= gs.c_str();
    geometricShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometricShader, 1, &gscode, NULL);
    glCompileShader(geometricShader);
    glGetShaderiv(geometricShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(geometricShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::GEOMETRIC::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
  }

  id = glCreateProgram();
  glAttachShader(id, vertexShader);
  if(gs!="") glAttachShader(id, geometricShader);
  glAttachShader(id, fragmentShader);
  glLinkProgram(id);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  if(gs!="") glDeleteShader(geometricShader);
}

std::string Shader::getCodeFromFile(const std::string &path) {
  std::string code;
  std::ifstream file;
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    file.open(getPath(path));
    std::stringstream stream;
    stream << file.rdbuf();
    file.close();
    code = stream.str();
  } catch (std::ifstream::failure &e) {
    LOG_ERR("File Error: " + std::string(e.what()));
  }
  return code;
}

void Shader::use() const { glUseProgram(id); }

GLint Shader::getUniformLocation(const std::string &name) const {
  return glGetUniformLocation(id, name.c_str());
}

void Shader::setInt(const std::string &name, GLint value) const {
  glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(const std::string &name, GLfloat value) const {
  glUniform1f(getUniformLocation(name), value);
}

void Shader::setMat3(const std::string &name, const mat3 &value) const {
  glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE,
                     glm::value_ptr(value));
}

void Shader::setMat4(const std::string &name, const mat4 &value) const {
  glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE,
                     glm::value_ptr(value));
}

void Shader::setVec3(const std::string &name, const vec3 &value) const {
  glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string &name, const vec4 &value) const {
  glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
}
