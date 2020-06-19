#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_g.h>
#include <camera.h>
#include <model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
unsigned int loadCubemap(vector<std::string> faces);

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 1024;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//atmosphere switch
bool atmosphere = true;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Planet", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glew: load all OpenGL function pointers
    // ---------------------------------------
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader PlanetShader("Earth.vs", "Earth.fs", "Earth.gs");
    Shader SpaceBoxShader("Space.vs", "Space.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

  //Earth Vertices Calculation Step 1: Calculation of Icosahedron Vertices
    const float HDelta = M_PI / 180 * 72;      // Change in horizontal angle: 72 degree 
    const float VDelta = atanf(1.0f / 2);      // Change in vertical angle: 26.565 degree

    float EarthVertices[] = {
    0.0f,0.0f,1.0f,
    cos(VDelta),0.0f,sin(VDelta),
    cos(VDelta) * cos(HDelta),cos(VDelta) * sin(HDelta),sin(VDelta),
    0.0f,0.0f,1.0f,
    cos(VDelta) * cos(HDelta),cos(VDelta) * sin(HDelta),sin(VDelta),
    cos(VDelta) * cos(2 * HDelta),cos(VDelta) * sin(2 * HDelta),sin(VDelta),
    0.0f,0.0f,1.0f,
    cos(VDelta) * cos(2 * HDelta),cos(VDelta) * sin(2 * HDelta),sin(VDelta),
    cos(VDelta) * cos(3 * HDelta),cos(VDelta) * sin(3 * HDelta),sin(VDelta),
    0.0f,0.0f,1.0f,
    cos(VDelta) * cos(3 * HDelta),cos(VDelta) * sin(3 * HDelta),sin(VDelta),
    cos(VDelta) * cos(4 * HDelta),cos(VDelta) * sin(4 * HDelta),sin(VDelta),
    0.0f,0.0f,1.0f,
    cos(VDelta) * cos(4 * HDelta),cos(VDelta) * sin(4 * HDelta),sin(VDelta),
    cos(VDelta),0.0f,sin(VDelta),
    cos(VDelta) * cos(4 * HDelta),cos(VDelta) * sin(4 * HDelta),sin(VDelta),
    cos(VDelta) * cos(4.5 * HDelta),cos(VDelta) * sin(4.5 * HDelta),-sin(VDelta),
    cos(VDelta),0.0f,sin(VDelta),
    cos(VDelta) * cos(4 * HDelta),cos(VDelta) * sin(4 * HDelta),sin(VDelta),
    cos(VDelta) * cos(3.5 * HDelta),cos(VDelta) * sin(3.5 * HDelta),-sin(VDelta),
    cos(VDelta) * cos(4.5 * HDelta),cos(VDelta) * sin(4.5 * HDelta),-sin(VDelta),
    cos(VDelta) * cos(3 * HDelta),cos(VDelta) * sin(3 * HDelta),sin(VDelta),
    cos(VDelta) * cos(3.5 * HDelta),cos(VDelta) * sin(3.5 * HDelta),-sin(VDelta),
    cos(VDelta) * cos(4 * HDelta),cos(VDelta) * sin(4 * HDelta),sin(VDelta),
    cos(VDelta) * cos(3 * HDelta),cos(VDelta) * sin(3 * HDelta),sin(VDelta),
    -cos(VDelta),0.0f,-sin(VDelta),
    cos(VDelta) * cos(3.5 * HDelta),cos(VDelta) * sin(3.5 * HDelta),-sin(VDelta),
    cos(VDelta) * cos(2 * HDelta),cos(VDelta) * sin(2 * HDelta),sin(VDelta),
    -cos(VDelta),0.0f,-sin(VDelta),
    cos(VDelta) * cos(3 * HDelta),cos(VDelta) * sin(3 * HDelta),sin(VDelta),
    cos(VDelta) * cos(2 * HDelta),cos(VDelta) * sin(2 * HDelta),sin(VDelta),
    cos(VDelta) * cos(1.5 * HDelta),cos(VDelta) * sin(1.5 * HDelta),-sin(VDelta),
    -cos(VDelta),0.0f,-sin(VDelta),
    cos(VDelta) * cos(HDelta),cos(VDelta) * sin(HDelta),sin(VDelta),
    cos(VDelta) * cos(1.5 * HDelta),cos(VDelta) * sin(1.5 * HDelta),-sin(VDelta),
    cos(VDelta) * cos(2 * HDelta),cos(VDelta) * sin(2 * HDelta),sin(VDelta),
    cos(VDelta) * cos(HDelta),cos(VDelta) * sin(HDelta),sin(VDelta),
    cos(VDelta) * cos(0.5 * HDelta),cos(VDelta) * sin(0.5 * HDelta),-sin(VDelta),
    cos(VDelta) * cos(1.5 * HDelta),cos(VDelta) * sin(1.5 * HDelta),-sin(VDelta),
    cos(VDelta),0.0f,sin(VDelta),
    cos(VDelta) * cos(0.5 * HDelta),cos(VDelta) * sin(0.5 * HDelta),-sin(VDelta),
    cos(VDelta) * cos(HDelta),cos(VDelta) * sin(HDelta),sin(VDelta),
    cos(VDelta),0.0f,sin(VDelta),
    cos(VDelta) * cos(4.5 * HDelta),cos(VDelta) * sin(4.5 * HDelta),-sin(VDelta),
    cos(VDelta) * cos(0.5 * HDelta),cos(VDelta) * sin(0.5 * HDelta),-sin(VDelta),
    cos(VDelta) * cos(4.5 * HDelta),cos(VDelta) * sin(4.5 * HDelta),-sin(VDelta),
    0.0f,0.0f,-1.0f,
    cos(VDelta) * cos(0.5 * HDelta),cos(VDelta) * sin(0.5 * HDelta),-sin(VDelta),
    cos(VDelta) * cos(0.5 * HDelta),cos(VDelta) * sin(0.5 * HDelta),-sin(VDelta),
    0.0f,0.0f,-1.0f,
    cos(VDelta) * cos(1.5 * HDelta),cos(VDelta) * sin(1.5 * HDelta),-sin(VDelta),
    cos(VDelta) * cos(1.5 * HDelta),cos(VDelta) * sin(1.5 * HDelta),-sin(VDelta),
    0.0f,0.0f,-1.0f,
    -cos(VDelta),0.0f,-sin(VDelta),
    -cos(VDelta),0.0f,-sin(VDelta),
    0.0f,0.0f,-1.0f,
    cos(VDelta) * cos(3.5 * HDelta),cos(VDelta) * sin(3.5 * HDelta),-sin(VDelta),
    cos(VDelta) * cos(3.5 * HDelta),cos(VDelta) * sin(3.5 * HDelta),-sin(VDelta),
    0.0f,0.0f,-1.0f,
    cos(VDelta) * cos(4.5 * HDelta),cos(VDelta) * sin(4.5 * HDelta),-sin(VDelta)
    };

    //Earth Vertices Calculation Step 2: SubDivision of each equilaternal triangle into 4 mini equilateral triangles (Done in Earth Geometric Shader)

    float SpaceBoxVertices[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // earth VAO
    unsigned int EarthVBO, EarthVAO;
    glGenVertexArrays(1, &EarthVAO);
    glGenBuffers(1, &EarthVBO);
    glBindVertexArray(EarthVAO);

    glBindBuffer(GL_ARRAY_BUFFER, EarthVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(EarthVertices), EarthVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // skybox VAO
    unsigned int SpaceBoxVAO, SpaceBoxVBO;
    glGenVertexArrays(1, &SpaceBoxVAO);
    glGenBuffers(1, &SpaceBoxVBO);
    glBindVertexArray(SpaceBoxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, SpaceBoxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SpaceBoxVertices), &SpaceBoxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // load textures
    // -------------

    vector<std::string> SpaceFaces{
        "SpaceRight.png","SpaceLeft.png","SpaceTop.png", "SpaceBottom.png", "SpaceFront.png", "SpaceBack.png" };
       
    vector<std::string> EarthFaces{
       "EarthRight.png","EarthLeft.png","EarthTop.png", "EarthBottom.png", "EarthFront.png", "EarthBack.png"};

    vector<std::string> EarthSpecFaces{
        "EarthSpecRight.png", "EarthSpecLeft.png","EarthSpecTop.png","EarthSpecBottom.png", "EarthSpecFront.png","EarthSpecBack.png" };     
    
    vector<std::string> EarthBumpFaces{
        "EarthBumpRight.png", "EarthBumpLeft.png","EarthBumpTop.png","EarthBumpBottom.png","EarthBumpFront.png","EarthBumpBack.png"};

    vector<std::string> EarthNightFaces{
        "EarthNightRight.png", "EarthNightLeft.png","EarthNightTop.png","EarthNightBottom.png","EarthNightFront.png","EarthNightBack.png" };

    vector<std::string> SunFaces{
        "Sun.png","Sun.png","Sun.png","Sun.png","Sun.png","Sun.png" };

    vector<std::string> SunSpecFaces{
        "SunSpec.png","SunSpec.png","SunSpec.png","SunSpec.png","SunSpec.png","SunSpec.png" };

    vector<std::string> CloudsFaces{
        "CloudsRight.png","CloudsLeft.png","CloudsTop.png","CloudsBottom.png","CloudsFront.png","CloudsBack.png"};

    unsigned int SpaceMapTexture = loadCubemap(SpaceFaces);
    unsigned int EarthMapTexture  = loadCubemap(EarthFaces);
    unsigned int EarthSpecMapTexture = loadCubemap(EarthSpecFaces);
    unsigned int EarthBumpMapTexture = loadCubemap(EarthBumpFaces);
    unsigned int EarthNightMapTexture = loadCubemap(EarthNightFaces);
    unsigned int SunMapTexture = loadCubemap(SunFaces);
    unsigned int SunSpecMapTexture = loadCubemap(SunSpecFaces);
    unsigned int CloudsMapTexture = loadCubemap(CloudsFaces);

    // shader configuration
    // --------------------

    SpaceBoxShader.use();
    SpaceBoxShader.setInt("Spacebox", 0);

    PlanetShader.use();
    PlanetShader.setInt("material.diffuse", 0);
    PlanetShader.setInt("material.specular", 1);
    PlanetShader.setInt("material.bump", 2);
    PlanetShader.setInt("material.night", 3);

   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    double previousTime = glfwGetTime();
    int FPS = 0;

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //FPS Calculation
        FPS++;
        // If a second has passed.
        if (currentFrame - previousTime >= 1.0)
            {
                std::stringstream ss;
                ss << "Planet   " << "FPS: " << FPS;
                glfwSetWindowTitle(window,ss.str().c_str());

                FPS = 0;
                previousTime = currentFrame;
            }

        // input
        // -----
        processInput(window);

        // render
        // ------

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_CULL_FACE);
        //glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ONE, GL_ZERO);
        //glEnable(GL_BLEND);

        // be sure to activate shader when setting uniforms/drawing objects
        PlanetShader.use();
        PlanetShader.setVec4("light.direction", cos(glm::radians(2*glfwGetTime())), 0.0f, -sin(glm::radians(2*glfwGetTime())), 0.0f);
        PlanetShader.setVec3("viewPos", camera.Position);

        // light properties
        PlanetShader.setVec4("light.ambient", 0.2f, 0.2f, 0.2f, 1.0f);
        PlanetShader.setVec4("light.diffuse", 0.8f, 0.8f, 0.8f, 1.0f);
        PlanetShader.setVec4("light.specular", 1.0f, 1.0f, 1.0f, 1.0f);

        // material properties
        PlanetShader.setFloat("material.shininess", 32.0f);

        // view/projection transformations+-
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(glm::mat4(1.0f), glm::radians((float)(glfwGetTime())), glm::vec3(0.0f, 1.0f, 0.0f));
        PlanetShader.setMat4("projection", projection);
        PlanetShader.setMat4("view", view);
        PlanetShader.setMat4("model", model);

        // render Earth
        PlanetShader.setBool("Sunlight", false);
        PlanetShader.setFloat("radius", 1.0f);
        glBindVertexArray(EarthVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, EarthMapTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, EarthSpecMapTexture);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, EarthBumpMapTexture);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_CUBE_MAP, EarthNightMapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 60);

        //render Clouds
        if (atmosphere)
        {
            PlanetShader.setBool("Clouds", true);
            PlanetShader.setFloat("radius", 1.04f);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, CloudsMapTexture);
            glDrawArrays(GL_TRIANGLES, 0, 60);
        }

        //render Sun  
        PlanetShader.setBool("Clouds", false);
        PlanetShader.setBool("Sunlight", true);
        PlanetShader.setFloat("radius", 10.0f);
        model = glm::rotate(glm::mat4(1.0f), glm::radians((float)(3*glfwGetTime())), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(-40.0f, 0.0f, 0.0f));
        PlanetShader.setMat4("model", model);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, SunMapTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, SunSpecMapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 60);

        // draw SpaceBox
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        SpaceBoxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        SpaceBoxShader.setMat4("view", view);
        SpaceBoxShader.setMat4("projection", projection);
        
        // SpaceBox cube
        glBindVertexArray(SpaceBoxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, SpaceMapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &EarthVAO);
    glDeleteVertexArrays(1, &SpaceBoxVAO);
    glDeleteBuffers(1, &SpaceBoxVBO);
    glDeleteBuffers(1, &EarthVBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        atmosphere = !atmosphere;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 4);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
