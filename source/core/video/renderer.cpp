#define GLFW_INCLUDE_NONE

#include "renderer.h"
#include "core/logging.h"
#include "glad/gl.h"
#include "glfw/glfw3.h"
#include <fstream>
#include <string>

Renderer* pRenderer;

// Load shader string from a file.

static std::string LoadShaderFile(std::string_view path)
{
    std::ifstream file(path.data());

    // Validate that the file was opened.

    if (!file.is_open())
    {
        ERR("Failed to load shader from \"" << path << "\".");

        return {};
    }

    // Read the file into a string.

    std::string source;
    std::string line;

    while (std::getline(file, line))
    {
        source += line + '\n';
    }

    LOG("Loaded shader from \"" << path << "\".");

    return std::move(source);
}

// Load a bitmap image from a file.

static std::vector<unsigned char> LoadImageFile(std::string_view path, int& outWidth, int& outHeight)
{
    std::ifstream file(path.data(), std::ios::binary);

    // Validate that the file was opened.

    if (!file.is_open())
    {
        ERR("Failed to load image from \"" << path << "\".");

        return {};
    }

    // Read the pixel starting offset.

    int pixelOffset;
    file.ignore(10);
    file.read((char*) &pixelOffset, 4);
    file.ignore(4);

    // Read the image's dimensions.

    file.read((char*) &outWidth, 4);
    file.read((char*) &outHeight, 4);
    file.ignore(pixelOffset - 26);

    // Read the image's pixel data.

    std::vector<unsigned char> pixels(outWidth * outHeight * 4);

    for (int i = 0; i < outWidth * outHeight; i++)
    {
        unsigned char pixel[4];
        file.read((char*) pixel, 4);

        pixels[i * 4] = pixel[2];
        pixels[i * 4 + 1] = pixel[1];
        pixels[i * 4 + 2] = pixel[0];
        pixels[i * 4 + 3] = pixel[3];
    }

    LOG("Loaded image from \"" << path << "\".");

    return std::move(pixels);
}

// Initialise the renderer.

Renderer::Renderer(std::string_view vertexPath, std::string_view fragmentPath)
    : fontSprites()
{
    pRenderer = this;

    // Initialise GLAD and enable depth testing.

    gladLoadGL(glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Create the VAO, VBO, and IBO.

    constexpr float vertices[] =
    {
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f
    };

    constexpr unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 20, vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*) (sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    // Bind the vertex array object (VAO).

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(vertexArray);

    // Create and compile the vertex and fragment shaders.

    std::string vertexSource = LoadShaderFile(vertexPath);
    const char* vertexString = vertexSource.c_str();

    std::string fragmentSource = LoadShaderFile(fragmentPath);
    const char* fragmentString = fragmentSource.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexString, nullptr);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentString, nullptr);
    glCompileShader(fragmentShader);

    // Create and link a shader program.

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Save all shader uniforms (modifiable attributes).

    projectionUniform = glGetUniformLocation(shaderProgram, "projection");
    positionUniform = glGetUniformLocation(shaderProgram, "position");
    scaleUniform = glGetUniformLocation(shaderProgram, "scale");
    coordsUniform = glGetUniformLocation(shaderProgram, "coords");

    LOG("Initialised the Renderer.");
}

// Terminate the renderer.

Renderer::~Renderer()
{
    glDeleteTextures((int) textures.size(), &textures[0]);

    glDeleteVertexArrays(1, &vertexArray);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &indexBuffer);

    glDeleteProgram(shaderProgram);
}

// Set the sheet used for drawing strings.

void Renderer::SetFontSheet(const SpriteSheet& sheet)
{
    for (int i = 0; i < 95; i++)
    {
        fontSprites[i] = sheet.GetSprite((i % 16) * 6, i / 16 * 10, 6, 10);
    }
}

// Set the rendering viewport resolution.

void Renderer::SetResolution(int width, int height) const
{
    glViewport(0, 0, width, height);
}

// Set an orthographic projection matrix.

void Renderer::SetProjection(float l, float r, float b, float t, float depth) const
{
    float projection[] =
    {
        2.0f / (r - l), 0.0f,           0.0f,          (r + l) / -(r - l),
        0.0f,           2.0f / (t - b), 0.0f,          (t + b) / -(t - b),
        0.0f,           0.0f,           1.0f / -depth, 0.0f,
        0.0f,           0.0f,           0.0f,          1.0f
    };

    glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &projection[0]);
}

// Draw a sprite at a position with a size;
// Width and height default to 1.0.

void Renderer::DrawSprite(const Sprite& sprite, float x, float y, float z, float w, float h) const
{
    glBindTexture(GL_TEXTURE_2D, sprite.identifier);
    glUniform3f(positionUniform, x, y, z);
    glUniform3f(scaleUniform, w, h, 1.0f);
    glUniform4f(coordsUniform, sprite.x, sprite.y, sprite.w, sprite.h);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

// Draw a string at a position with an alignment;
// Alignment: 0.0 = left, 0.5 = centre, 1.0 = right.

void Renderer::DrawString(std::string_view string, float x, float y, float z, float alignment) const
{
    glBindTexture(GL_TEXTURE_2D, fontSprites[0].identifier);
    glUniform3f(scaleUniform, 0.375f, 0.625f, 1.0f);

    float xOrigin = x;
    y -= 0.625f;

    // Loop over each line of the string.

    size_t begin = 0;
    size_t end = 0;

    while (end != std::string_view::npos)
    {
        end = string.find('\n', begin);

        // Get the line and align it.

        std::string_view line = string.substr(begin, end - begin);
        x -= (float) line.length() * 0.375f * alignment;

        // Draw each character in the line.

        for (char character : line)
        {
            const Sprite& sprite = fontSprites[character - 32];

            glUniform3f(positionUniform, x, y, z);
            glUniform4f(coordsUniform, sprite.x, sprite.y, sprite.w, sprite.h);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

            // Move to the next character.

            x += 0.375f;
        }

        // Move to the next line.

        y -= 0.75f;
        x = xOrigin;
        begin = end + 1;
    }
}

// Clear the rendering viewport.

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Get a sprite sheet from file path.

SpriteSheet Renderer::GetSheet(std::string_view path)
{
    // If the sprite sheet is already loaded, return it.

    if (sheets.find(path) != sheets.end())
    {
        return sheets.at(path);
    }

    // Setup an OpenGL texture.

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Load an image into the OpenGL texture.

    int width, height;
    std::vector<unsigned char> pixels = LoadImageFile(path, width, height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);

    // Cache and return the sprite sheet.

    SpriteSheet sheet = {texture, width, height};
    sheets[path] = sheet;
    textures.push_back(texture);

    return sheet;
}