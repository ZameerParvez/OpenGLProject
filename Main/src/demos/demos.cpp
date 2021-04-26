#include "demos.h"

GLenum GlCheckError(const char* file, int line) {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (errorCode) {
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_STACK_OVERFLOW:
                error = "STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                error = "STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
        }

        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}

GLuint CompileShader(const char* shaderSource, ShaderType type) {
    GLuint glShaderType;
    const char* glErrorMessage;

    switch (type) {
        case ShaderType::VERTEX:
            glShaderType = GL_VERTEX_SHADER;
            glErrorMessage = "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";
            break;
        case ShaderType::FRAGMENT:
            glShaderType = GL_FRAGMENT_SHADER;
            glErrorMessage = "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n";
            break;
        default:
            glShaderType = GL_VERTEX_SHADER;
            glErrorMessage = "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";
    }

    // create shader object in gpu, and load shader source into it, then compile
    GLuint shader;
    shader = glCreateShader(glShaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    // check if shader compilation was successful
    GLint success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << glErrorMessage << infoLog << std::endl;
    }

    return shader;
}

GLuint LinkShaders(GLuint vertexShader, GLuint fragmentShader) {
    // must have vertex and fragment shaders
    if (!vertexShader || !fragmentShader) return 0;  // TODO: throw error

    // link shaders (so outputs and inputs match)
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check if shader linking was successful
    GLint success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    return shaderProgram;
}


// NOTE: if the same thing is selected then the same shape will be initialised again
void Demos::ImguiDemoSelector(Demos::demoFunc& currDemo, bool& initialSetupDone) {
    const auto demoSelect = [&](const char* label, const Demos::demoFunc& select) {
      if (ImGui::Button(label)) {
        currDemo = select;
        initialSetupDone = false;
      }
    };

    ImGui::Begin("Demo Selector");
    demoSelect("Simple Triangle", Demos::SimpleTriangle);
    demoSelect("Simple Rectangle", Demos::SimpleRectangle);
    ImGui::End();
}

void Demos::RunDemos() {
    static bool initialSetupDone = false;
    static Demos::demoFunc currentDemo = Demos::SimpleTriangle;

    ImguiDemoSelector(currentDemo, initialSetupDone);

    currentDemo(!initialSetupDone);
    initialSetupDone = true;
}