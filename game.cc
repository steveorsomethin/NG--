#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <array>
#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL.h>
// #include <SDL_opengl.h>
// #include <OpenGL/gl3ext.h>
#include <math.h>
#include <time.h>
#include <random>

#include <math/Math.h>
#include <core/Time.h>
#include <core/Entity.h>
#include <components/RenderComponent.h>
#include <components/PositionComponent.h>
#include <components/TweenComponent.h>
#include <components/CameraComponent.h>
#include <components/GridComponent.h>
#include <components/StateComponent.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

using namespace NGPP::Core;
using namespace NGPP::Components;

enum BlockState
{
    FALLING = 1u << 0,
    RESTING = 1u << 1,
    SELECTED = 1u << 2,
    DESTROYING = 1u << 3,
    DESTROYED = 1u << 4
};

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
    "uniform mat4 model;\n"
    "varying float depth;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = model * gl_Vertex;\n"
    "   depth = gl_Position.z * -0.05;\n"
    "}\n"
);

const std::string strFragmentShader(
    "uniform vec4 color;\n"
    "varying float depth;\n"
    "void main()\n"
    "{\n"
    "   gl_FragColor = mix(color, vec4(depth, depth, depth, 1.0), 0.1);\n"
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
     0.5f,  0.5f, 0.5f, 1.0f,
     0.5f, -0.5f, 0.5f, 1.0f,
    -0.5f,  0.5f, 0.5f, 1.0f,

     0.5f, -0.5f, 0.5f, 1.0f,
    -0.5f, -0.5f, 0.5f, 1.0f,
    -0.5f,  0.5f, 0.5f, 1.0f,

     0.5f,  0.5f, -0.5f, 1.0f,
    -0.5f,  0.5f, -0.5f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f,

     0.5f, -0.5f, -0.5f, 1.0f,
    -0.5f,  0.5f, -0.5f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f,

    -0.5f,  0.5f,  0.5f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f,

    -0.5f,  0.5f,  0.5f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f,
    -0.5f,  0.5f, -0.5f, 1.0f,

     0.5f,  0.5f,  0.5f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f,

     0.5f,  0.5f,  0.5f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f,

     0.5f,  0.5f, -0.5f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f,
    -0.5f,  0.5f,  0.5f, 1.0f,

     0.5f,  0.5f, -0.5f, 1.0f,
    -0.5f,  0.5f,  0.5f, 1.0f,
    -0.5f,  0.5f, -0.5f, 1.0f,

     0.5f, -0.5f, -0.5f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f,

     0.5f, -0.5f, -0.5f, 1.0f,
    -0.5f, -0.5f, -0.5f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f
};

GLuint positionBufferObject;
GLuint vao;
std::vector<RenderComponent*> renderComponents;
std::vector<PositionComponent*> positionComponents;
std::vector<TweenComponent*> tweenComponents;

int gridWidth = 6;
int gridHeight = 12;

int selectedX1 = 0;
int selectedX2 = 1;
int selectedY = 0;

PositionComponent *cameraPosition;
PositionComponent *cameraTarget;
TweenComponent *cameraTween;
CameraComponent *cameraComponent;

GridComponent *gridComponent;

Time gameTime = {0, 0, 16, 0.0f};
glm::mat4 proj = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

std::array<glm::vec4, 6> colors = {
    glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), // Red
    glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), // Green
    glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), // Blue
    glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), // Purple
    glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), // Yellow
    glm::vec4(1.0f, 1.0f, 1.0f, 0.0f) // Clear
};

void InitializeVertexBuffer()
{
    glGenBuffers(1, &positionBufferObject);

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void initComponents()
{
    cameraTarget = new PositionComponent();
    cameraPosition = new PositionComponent();
    cameraTween = new TweenComponent(100.0f);
    cameraComponent = new CameraComponent(proj);

    Entity *camEntity = new Entity(0, {
        {"target", cameraTarget},
        {"position", cameraPosition},
        {"positionTween", cameraTween},
        {"camera", cameraComponent}
    });

    cameraPosition->SetPosition(Vector3(3.0f, 6.0f, 16.0f));
    cameraTarget->SetPosition(Vector3(3.0f, 6.0f, 0.0f));

    cameraTarget->output->Pipe(cameraComponent->targetInput);
    cameraPosition->output->Pipe(cameraTween->input);
    cameraTween->output->Pipe(cameraComponent->positionInput);

    gridComponent = new GridComponent(gridWidth, gridHeight);
    Entity *gridEntity = new Entity(0, {
        {"grid", gridComponent}
    });

    positionComponents = gridComponent->positions;

    std::default_random_engine e1;
    std::uniform_int_distribution<int> uniform_dist(0, 5);

    for (auto positionComponent : gridComponent->positions) {
        int colorIdx = uniform_dist(e1);

        RenderComponent *renderComponent = new RenderComponent();
        StateComponent *stateComponent = new StateComponent();
        TweenComponent *tweenComponent = new TweenComponent(100.0f);

        Entity *entity = new Entity(0, {
            {"position", positionComponent},
            {"positionTween", tweenComponent},
            {"state", stateComponent},
            {"renderer", renderComponent}
        });

        renderComponents.push_back(renderComponent);
        tweenComponents.push_back(tweenComponent);

        stateComponent->SetState(RESTING);

        positionComponent->output->Pipe(tweenComponent->input);
        tweenComponent->output->Pipe(renderComponent->positionInput);

        renderComponent->object3d.color = colors[colorIdx];
    }
}

//Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void init()
{
    InitializeProgram();
    InitializeVertexBuffer();
    initComponents();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

int lastTime;

void destroyConsecutiveBlocks()
{

}

void update()
{
    lastTime += 16;
    if (lastTime < 30) return;

    lastTime = 0;

    int i = 0;

    for (auto position : gridComponent->positions) {
        Entity *entity = position->entity;

        StateComponent * stateComp = entity->GetComponent<StateComponent*>("state");
        int state = stateComp->state;

        if (entity->GetComponent<RenderComponent*>("renderer")->object3d.color.w == 0.0f) {
            stateComp->state |= DESTROYED;
        }

        if (state & RESTING) {
            float x = position->GetX();
            float y = position->GetY();

            bool isAtBottom = y == 0.0f;

            auto underPos = gridComponent->GetComponentAt(x, y - 1);
            auto overPos = gridComponent->GetComponentAt(x, y + 1);

            if (underPos) {
                int nextState =
                    underPos->entity->GetComponent<StateComponent*>("state")->state;

                if (nextState & DESTROYED) {
                    float nextX = underPos->GetX();
                    float nextY = underPos->GetY();

                    gridComponent->Swap(x, y, nextX, nextY);
                }
            }
        }
    }
}

void render() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(theProgram);

    GLuint modelUniform = glGetUniformLocation(theProgram, "model");
    GLuint colorUniform = glGetUniformLocation(theProgram, "color");

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    cameraTween->Tick(gameTime);

    for (auto tweenComponent : tweenComponents)
    {
        tweenComponent->Tick(gameTime);
    }

    for (auto renderComponent : renderComponents)
    {
        if (renderComponent->object3d.color.w > 0.0f) {
            glm::mat4 mvp = cameraComponent->camera.matrix * renderComponent->object3d.matrix;
            glUniform4fv(colorUniform, 1, glm::value_ptr(renderComponent->object3d.color));
            glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(mvp));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    glDisableVertexAttribArray(0);
    glUseProgram(0);
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

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    SDL_GetWindowSize(window, &screen_w, &screen_h);
    glViewport(0, 0, screen_w, screen_h);

    init();

    done = 0;
    /* enter main loop */
    while (!done) {
        startFrame = SDL_GetTicks();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                SDL_KeyboardEvent keyEvent = *(SDL_KeyboardEvent*)&event;
                int scanCode = keyEvent.keysym.scancode;
                PositionComponent *positionComponent1 = positionComponents[0];
                PositionComponent *positionComponent2 = positionComponents[1];

                float x = positionComponent1->GetX();
                float y = positionComponent1->GetY();
                float z = positionComponent1->GetZ();

                if (scanCode == 4) positionComponent1->SetX(x - 1.0f);
                if (scanCode == 7) positionComponent1->SetX(x + 1.0f);
                if (scanCode == 26) positionComponent1->SetY(y + 1.0f);
                if (scanCode == 22) positionComponent1->SetY(y - 1.0f);

                float camX = cameraPosition->GetX();

                if (scanCode == 80 && selectedX1 > 0) {
                    selectedX1--;
                    selectedX2--;
                }
                if (scanCode == 79 && selectedX2 < gridWidth - 1) {
                    selectedX1++;
                    selectedX2++;
                }

                if (scanCode == 81 && selectedY > 0) selectedY--;
                if (scanCode == 82 && selectedY < gridHeight - 1) selectedY++;
                if (scanCode == 20) cameraPosition->SetX(camX - 1.0f);
                if (scanCode == 8) cameraPosition->SetX(camX + 1.0f);

                if (scanCode == 40) {
                    gridComponent->Swap(selectedX1, selectedY, selectedX2, selectedY);
                }
            }
            if (event.type == SDL_QUIT) {
                done = 1;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
            }
        }
        
        update();
        render();
        
        SDL_GL_SwapWindow(window);
        endFrame = SDL_GetTicks();
    }

    /* shutdown SDL */
    SDL_Quit();

    return 0;
}