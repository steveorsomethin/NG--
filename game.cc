#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL.h>
// #include <SDL_opengl.h>
// #include <OpenGL/gl3ext.h>
#include <math.h>
#include <time.h>

#include <math/Math.h>
#include <core/Time.h>
#include <core/Entity.h>
#include <components/RenderComponent.h>
#include <components/PositionComponent.h>
#include <components/TweenComponent.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

using namespace NGPP::Core;
using namespace NGPP::Components;

GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile)
{
    GLuint shader = glCreateShader(eShaderType);
    const char *strFileData = strShaderFile.c_str();
    glShaderSource(shader, 1, &strFileData, NULL);

    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

        const char *strShaderType = NULL;
        switch(eShaderType)
        {
        case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
        case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
        case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
        }

        fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
        delete[] strInfoLog;
    }

    return shader;
}

GLuint CreateProgram(const std::vector<GLuint> &shaderList)
{
    GLuint program = glCreateProgram();

    for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
        glAttachShader(program, shaderList[iLoop]);

    glLinkProgram(program);

    GLint status;
    glGetProgramiv (program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        delete[] strInfoLog;
    }

    for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
        glDetachShader(program, shaderList[iLoop]);

    return program;
}

GLuint theProgram;

const std::string strVertexShader(
    "uniform vec4 position;"
    "void main()\n"
    "{\n"
    "   gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex + position);\n"
    "}\n"
);

const std::string strFragmentShader(
    "void main()\n"
    "{\n"
    "   gl_FragColor = mix(vec4(0.4, 0.0, 0.0, 1.0), vec4(gl_FragCoord.x / 800.0), 0.5);\n"
    "}\n"
);

void InitializeProgram()
{
    std::vector<GLuint> shaderList;

    shaderList.push_back(CreateShader(GL_VERTEX_SHADER, strVertexShader));
    shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, strFragmentShader));

    theProgram = CreateProgram(shaderList);

    std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
}

const float vertexPositions[] = {
     0.25f,  0.25f, 0.75f, 1.0f,
     0.25f, -0.25f, 0.75f, 1.0f,
    -0.25f,  0.25f, 0.75f, 1.0f,

     0.25f, -0.25f, 0.75f, 1.0f,
    -0.25f, -0.25f, 0.75f, 1.0f,
    -0.25f,  0.25f, 0.75f, 1.0f,

     0.25f,  0.25f, -0.75f, 1.0f,
    -0.25f,  0.25f, -0.75f, 1.0f,
     0.25f, -0.25f, -0.75f, 1.0f,

     0.25f, -0.25f, -0.75f, 1.0f,
    -0.25f,  0.25f, -0.75f, 1.0f,
    -0.25f, -0.25f, -0.75f, 1.0f,

    -0.25f,  0.25f,  0.75f, 1.0f,
    -0.25f, -0.25f,  0.75f, 1.0f,
    -0.25f, -0.25f, -0.75f, 1.0f,

    -0.25f,  0.25f,  0.75f, 1.0f,
    -0.25f, -0.25f, -0.75f, 1.0f,
    -0.25f,  0.25f, -0.75f, 1.0f,

     0.25f,  0.25f,  0.75f, 1.0f,
     0.25f, -0.25f, -0.75f, 1.0f,
     0.25f, -0.25f,  0.75f, 1.0f,

     0.25f,  0.25f,  0.75f, 1.0f,
     0.25f,  0.25f, -0.75f, 1.0f,
     0.25f, -0.25f, -0.75f, 1.0f,

     0.25f,  0.25f, -0.75f, 1.0f,
     0.25f,  0.25f,  0.75f, 1.0f,
    -0.25f,  0.25f,  0.75f, 1.0f,

     0.25f,  0.25f, -0.75f, 1.0f,
    -0.25f,  0.25f,  0.75f, 1.0f,
    -0.25f,  0.25f, -0.75f, 1.0f,

     0.25f, -0.25f, -0.75f, 1.0f,
    -0.25f, -0.25f,  0.75f, 1.0f,
     0.25f, -0.25f,  0.75f, 1.0f,

     0.25f, -0.25f, -0.75f, 1.0f,
    -0.25f, -0.25f, -0.75f, 1.0f,
    -0.25f, -0.25f,  0.75f, 1.0f,




    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,

    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,

    0.8f, 0.8f, 0.8f, 1.0f,
    0.8f, 0.8f, 0.8f, 1.0f,
    0.8f, 0.8f, 0.8f, 1.0f,

    0.8f, 0.8f, 0.8f, 1.0f,
    0.8f, 0.8f, 0.8f, 1.0f,
    0.8f, 0.8f, 0.8f, 1.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,

    0.5f, 0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 1.0f,

    0.5f, 0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 1.0f,

    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,

    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f, 1.0f,

    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,

    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,

};

GLuint positionBufferObject;
GLuint vao;
auto position1 = glm::vec4(0.5f, 0.0f, 0.0f, 0.0f);
auto position2 = glm::vec4(-0.5f, 0.0f, 0.0f, 0.0f);
std::vector<RenderComponent*> renderComponents;
std::vector<PositionComponent*> positionComponents;
std::vector<TweenComponent*> tweenComponents;
Time gameTime = {0, 0, 16, 0.0f};

void InitializeVertexBuffer()
{
    glGenBuffers(1, &positionBufferObject);

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void init()
{
    InitializeProgram();
    InitializeVertexBuffer();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

void render() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(theProgram);

    GLint positionUniform = glGetUniformLocation(theProgram, "position");
    
    glUniform4fv(positionUniform, 1, glm::value_ptr(position1));

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    for (auto tweenComponent : tweenComponents)
    {
        tweenComponent->Tick(gameTime);
    }

    for (auto renderComponent : renderComponents)
    {
        renderComponent->Tick(gameTime);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glDisableVertexAttribArray(0);
    glUseProgram(0);
}

void initComponents()
{
    GLuint positionUniform = glGetUniformLocation(theProgram, "position");
    for (int i = 0; i < 2; i++) {
        Entity *entity = new Entity(i);
        RenderComponent *renderComponent = new RenderComponent(entity, positionUniform);
        PositionComponent *positionComponent = new PositionComponent(entity);
        TweenComponent *tweenComponent = new TweenComponent(entity, 100);

        renderComponents.push_back(renderComponent);
        positionComponents.push_back(positionComponent);
        tweenComponents.push_back(tweenComponent);

        positionComponent->output->Pipe(tweenComponent->input);
        tweenComponent->output->Pipe(renderComponent->positionInput);
        positionComponent->SetPosition(Vector3((float)(i - 1), 0.0f, 2.0f));
    }
}

int main(int argc, char *argv[])
{
    SDL_Window *window;         /* main window */
    SDL_GLContext context;
    int screen_w, screen_h;
    Uint32 startFrame;          /* time frame began to process */
    Uint32 endFrame;            /* time frame ended processing */
    int done;                   /* should we clean up and exit? */

    /* initialize SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("%s\n", "Could not initialize SDL");
    }

    /* Request opengl 3.2 context.
     * SDL doesn't have the ability to choose which profile at this time of writing,
     * but it should default to the core profile */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
 
    /* Turn on double buffering with a 24bit Z buffer.
     * You may need to change this to 16 or 32 for your system */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    /* seed the random number generator */
    srand(time(NULL));

    /* create main window and renderer */
    window = SDL_CreateWindow(NULL, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    context = SDL_GL_CreateContext(window);

    SDL_GetWindowSize(window, &screen_w, &screen_h);
    glViewport(0, 0, screen_w, screen_h);

    init();
    initComponents();

    done = 0;
    /* enter main loop */
    while (!done) {
        startFrame = SDL_GetTicks();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                SDL_KeyboardEvent keyEvent = *(SDL_KeyboardEvent*)&event;
                int scanCode = keyEvent.keysym.scancode;
                PositionComponent *positionComponent = positionComponents[0];
                float x = positionComponent->GetX();
                float y = positionComponent->GetY();
                float z = positionComponent->GetZ();

                if (scanCode == 80) positionComponent->SetX(x - 0.75f);
                if (scanCode == 79) positionComponent->SetX(x + 0.75f);
                if (scanCode == 82) positionComponent->SetY(y + 0.75f);
                if (scanCode == 81) positionComponent->SetY(y - 0.75f);
                if (scanCode == 20) positionComponent->SetZ(z - 0.75f);
                if (scanCode == 26) positionComponent->SetZ(z + 0.75f);
            }
            if (event.type == SDL_QUIT) {
                done = 1;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
            }
        }
        
        //UPDATE AND DRAW
        render();
        
        SDL_GL_SwapWindow(window);
        endFrame = SDL_GetTicks();
    }

    /* shutdown SDL */
    SDL_Quit();

    return 0;
}