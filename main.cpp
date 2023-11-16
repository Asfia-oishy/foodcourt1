#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "cube.h"
#include "stb_image.h" //img load korar jnno
#include "pointLight.h"
#include "sphere.h"

#include "SpotLight.h"
#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);
void processInput(GLFWwindow* window);

void scsToWcs(float sx, float sy, float wcsv[3]);

//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scsToWcs(float sx, float sy, float wcsv[3]);
//unsigned int drawControlPoints();
//void saveControlPoints();
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

long long nCr(int n, int r);
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L); //curve calculation er jnno
unsigned int hollowBezier(GLfloat ctrlpoints[], int L);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void table(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
void fan(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
void fan_holder(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Sphere& sp);
void chair(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
void kitchen(unsigned int& cubeVAO, Shader& lightingShader, Shader& lightingShaderWithTexture, Cube& cube, Sphere& sp);
void chair(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
void tap(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Sphere& sp);
void sink(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
void stove(unsigned int& cubeVAO, Shader& lightingShader, Sphere& sp);
void basin(unsigned int& cubeVAO, Shader& lightingShader, Sphere& sp, Sphere& sp2);
void fridge(unsigned int& cubeVAO, Shader& lightingShader, Sphere& sp, glm::mat4 alTogether);
void oven(unsigned int& cubeVAO, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether);
void lamp(Shader& lightingShader, glm::mat4 all);
void objects(Shader& lightingShader, glm::mat4 all, float r, float g, float b, vector<float> v);
// settings
const unsigned int SCR_WIDTH = 1300;
const unsigned int SCR_HEIGHT = 700;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
float z_move1 = 0.0f;
float z_move2 = 0.0f;
float door = 0.0f;
float tap_w = 0.0f;
float frotate = 0.0f;
float frotate2 = 0.0f;
float fr = 0.0f;
bool pLight = false;
bool dLight = false;
bool sLight = false;

bool fanSwitch = false;
float r = 0.0f;

glm::mat4 change = glm::mat4(1.0f);

// model, view & projection matrix
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);

// viewport
GLint viewport[4];

/////////////////////////////////////////////////////////////
float wcsClkDn[3], wcsClkUp[3]; //point click and release
vector <float> plate =
{
    -1.1500, 1.5350, 5.1000,
        -0.9900, 1.5600, 5.1000,
        -0.7250, 1.4500, 5.1000,
        -0.7150, 1.3050, 5.1000,
        -0.3650, 1.2350, 5.1000,
        -0.0500, 1.2300, 5.1000,
};
vector <float> pipe =
{
    -0.3350, 1.6850, 5.1000,
        -0.3150, 1.1700, 5.1000,
        -0.2800, 0.7600, 5.1000,
        -0.2750, 0.4750, 5.1000,
};
vector <float> vase = { -0.4000, 1.8600, .1000,
-0.5050, 1.9500, .1000,
-0.4350, 2.0300, .1000,
-0.2900, 2.0250, .1000,
-0.2500, 1.9250, .1000,
-0.2650, 1.7850, .1000,
-0.3000, 1.6700, .1000,
-0.4100, 1.4650, .1000,
-0.5000, 1.3450, .1000,
-0.6050, 1.1300, .1000,
-0.6050, 0.9900, .1000,
-0.5700, 0.7550, .1000,
-0.4700, 0.4200, .1000,
-0.2800, 0.2250, .1000,
-0.0750, 0.0650, .1000, };
vector <float> pot = {
-0.3750, 2.1350, .1000,
-0.3750, 2.0250, .1000,
-0.3750, 1.9350, .1000,
-0.3900, 1.8050, .1000,
-0.4350, 1.7250, .1000,
-0.6250, 1.6800, .1000,
-0.7650, 1.6400, .1000,
-0.7850, 1.5050, .1000,
-0.7950, 1.3350, .1000,
-0.7950, 1.1050, .1000,
-0.7950, 0.8400, .1000,
-0.7950, 0.5950, .1000,
-0.7850, 0.3800, .1000,
-0.7950, 0.1500, .1000,
-0.7700, 0.0350, .1000,
-0.7400, -0.0200, .1000,
-0.6250, -0.0300, .1000,
-0.4500, -0.0300, .1000,
-0.2350, -0.0300, .1000,
-0.1050, -0.0200, .1000,
0.0000, 0.0100, .1000 };
vector <float> bowl = {
-0.7750, 1.4250, 5.1000,
-0.8500, 1.4950, 5.1000,
-0.8100, 1.5600, 5.1000,
-0.7350, 1.5650, 5.1000,
-0.6550, 1.5150, 5.1000,
-0.6550, 1.4400, 5.1000,
-0.6800, 1.3900, 5.1000,
-0.7250, 1.3300, 5.1000,
-0.7600, 1.2300, 5.1000,
-0.8150, 1.0750, 5.1000,
-0.8850, 0.8850, 5.1000,
-0.8850, 0.7150, 5.1000,
-0.8000, 0.4350, 5.1000,
-0.6800, 0.2600, 5.1000,
-0.5050, 0.1150, 5.1000,
-0.2950, 0.0450, 5.1000,
-0.0050, 0.0150, 5.1000 };
vector <float> bowl2 =
{
    -0.8550, 1.7550, 5.1000,
        -0.8350, 1.5300, 5.1000,
        -0.8550, 1.3250, 5.1000,
        -0.7900, 1.0950, 5.1000,
        -0.6350, 1.0200, 5.1000,
        -0.3700, 1.0200, 5.1000,
        -0.1150, 1.0200, 5.1000,
};
vector <float> gol =
{ -0.0750, 1.9950, 5.1000,
-0.8050, 1.9700, 5.1000,
-1.1550, 1.4650, 5.1000,
-0.9400, 0.8500, 5.1000,
-0.4800, 0.6400, 5.1000,
-0.1700, 0.5300, 5.1000,
-0.0600, 0.5350, 5.1000 };
vector <float> lamp_h = {
-1.0450, 1.3100, 5.1000,
-0.9950, 1.2300, 5.1000,
-1.0100, 1.1400, 5.1000,
-1.1300, 1.1200, 5.1000,
-1.2600, 1.1950, 5.1000,
-1.2900, 1.4150, 5.1000,
-1.2300, 1.6000, 5.1000,
-0.9250, 1.7150, 5.1000,
-0.5650, 1.8500, 5.1000,
-0.3600, 2.0350, 5.1000,
-0.0650, 2.2900, 5.1000 };
vector <float> lamp_b = {
-0.3550, 1.9350, 5.1000,
-0.3750, 1.8000, 5.1000,
-0.4750, 1.5650, 5.1000,
-0.6050, 1.3200, 5.1000,
-0.6750, 0.9300, 5.1000,
-0.5400, 0.5500, 5.1000,
-0.3900, 0.1950, 5.1000,
-0.1750, 0.0100, 5.1000,
-0.0050, 0.0150, 5.1000 };
vector <float> comode =
{
    -0.7900, 1.3150, 5.1000,
        -0.6750, 1.3750, 5.1000,
        -0.6850, 1.5750, 5.1000,
        -0.7800, 1.6450, 5.1000,
        -0.8650, 1.6350, 5.1000,
        -1.0100, 1.5900, 5.1000,
        -1.1150, 1.5300, 5.1000,
        -1.2000, 1.3700, 5.1000,
        -1.2200, 1.1550, 5.1000,
        -1.2250, 0.9750, 5.1000,
        -1.1350, 0.7500, 5.1000,
        -0.9850, 0.5050, 5.1000,
        -0.7550, 0.4450, 5.1000,
        -0.4350, 0.4250, 5.1000,
        -0.1300, 0.4200, 5.1000,
        -0.0100, 0.4200, 5.1000,
};
vector <float> bin =
{
-0.5850, 1.8900, 5.1000,
-0.5900, 1.5850, 5.1000,
-0.5850, 1.3650, 5.1000,
-0.5950, 1.1650, 5.1000,
-0.5850, 0.9650, 5.1000,
-0.3650, 0.8900, 5.1000,
-0.2000, 0.9000, 5.1000,
-0.0300, 0.9150, 5.1000
};

vector <float> coordinates;  //je point nilam tar normal nilam etc
vector <float> normals;
vector <int> indices; //ebo er jnno
vector <float> vertices; // coordi and normal merge kore rakha
class point   //mouse click krle point er x,y val store krbe
{
public:
    point()
    {
        x = 0;
        y = 0;
    }
    int x;
    int y;
} clkpt[2];
int mouseButtonFlag = 0;
FILE* fp;        //point fula save korar jnno
const double pi = 3.14159265389;
const int nt = 40; //1/nt kore barbe
const int ntheta = 20; //360/ntheta kore angle
bool showControlPoints = true;
bool loadBezierCurvePoints = true; //false chilo
bool showHollowBezier = false;
bool lineMode = false;
unsigned int bezierVAO;

bool flag = false;
bool dflag = false;
bool tflag = false;
bool fflag = false;
bool fflag2 = false;
bool of = false;




// camera
Camera camera(glm::vec3(1.0f, 1.0f, 1.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(.0f, 1.0f, .0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);




// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(4.50f,  2.5f,  0.0f),
    glm::vec3(-1.5f,  2.5f,  0.0f),
    glm::vec3(7.5f,  2.5f,  3.5f),
    glm::vec3(-1.5f,  2.5f,  5.5f),
    glm::vec3(.0f,  2.5f,  -3.5f),
    glm::vec3(5.5f,  2.5f,  -1.5f),
    glm::vec3(7.5f,  2.5f,  5.3f)
    /*
        glm::vec3(1.50f,  2.0f,  3.0f),
        glm::vec3(-1.5f,  2.0f,  3.0f),
        glm::vec3(1.5f,  2.0f,  -1.5f),
        glm::vec3(-1.5f,  2.0f,  -1.5f)*/
};
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.7f, 0.7f, 0.7f,     // diffuse
    //1.0f, 1.0f, 1.0f,
    .04f, .04f, .04f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.7f, 0.7f, 0.7f,       // diffuse
    .4f, .4f, .4f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.7f, 0.7f, 0.7f,       // diffuse
    .4f, .4f, .4f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.7f, 0.7f, 0.7f,       // diffuse
    .1f, .1f, .1f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);
PointLight pointlight5(

    pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.7f, 0.7f, 0.7f,       // diffuse
    .1f, .1f, .1f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    5       // light number
);
PointLight pointlight6(

    pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.5f, 0.5f, 0.5f,       // diffuse
    .1f, .1f, .1f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    6       // light number
);
PointLight pointlight7(

    pointLightPositions[6].x, pointLightPositions[6].y, pointLightPositions[6].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    1.0f, 1.0f, 1.0f,       // diffuse
    .05f, .05f, .05f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    7      // light number
);

glm::vec3 spotLightPositions[] = {
    glm::vec3(-1,  1.50f,  0.0f),

};


SpotLight spotlight1(
    spotLightPositions[0].x, spotLightPositions[0].y, spotLightPositions[0].z,  // position
    .2f, .2f, .2f,     // ambient
    1.0f, 1.0f, 1.0f,      // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1,       // light number
    glm::cos(glm::radians(30.5f)),
    glm::cos(glm::radians(60.0f)),
    .5f, -1, 0
);


// light settings
bool onOffToggle = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    //   GLFWwindow* window = glfwCreateWindow(1000, 700, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    // glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");

    string diffuseMapPath = "floor.png";
    string specularMapPath = "floor_spec.png";


    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f); //3ta type cube banano jabe

    string diffuseMapPath2 = "w.jfif";
    string specularMapPath2 = "floor_spec.png";


    unsigned int diffMap2 = loadTexture(diffuseMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap2 = loadTexture(specularMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube2 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    string diffuseMapPath3 = "w2.jpg";



    diffMap2 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap2 = loadTexture(specularMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube3 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    diffuseMapPath3 = "black.jpg";
    diffMap2 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube4 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath3 = "food.jpg";
    diffMap2 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube5 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath3 = "g3.jpg";
    diffMap2 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube6 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    diffuseMapPath3 = "recep.jpg";
    diffMap2 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube7 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath3 = "table_surf.jfif";
    diffMap2 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube8 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    diffuseMapPath3 = "drawer.jpg";
    diffMap2 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube9 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath3 = "washroom.jpg";
    diffMap2 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube10 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath3 = "door.jpg";
    diffMap2 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube11 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath3 = "drawer2.jpg";
    diffMap2 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube12 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath3 = "name.jpg";
    diffMap2 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube13 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath3 = "wall1.jfif";
    diffMap2 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube14 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath4 = "fridge.jpg";
    diffMap2 = loadTexture(diffuseMapPath4.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube15 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath4 = "oven.jpg";
    diffMap2 = loadTexture(diffuseMapPath4.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube16 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);




    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        if (lineMode)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }//curve

        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);

        pointlight5.setUpPointLight(lightingShader);
        pointlight6.setUpPointLight(lightingShader);

        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        lightingShaderWithTexture.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShaderWithTexture.setMat4("view", view);

        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);

        pointlight5.setUpPointLight(lightingShaderWithTexture);
        pointlight6.setUpPointLight(lightingShaderWithTexture);

        pointlight7.setUpPointLight(lightingShaderWithTexture);




        // Modelling Transformation*/
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;

        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0 - .25, -.02, 5.5));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.5, 0.03, 10.0));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;


        glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
        modelMatrixForContainer = glm::translate(model, glm::vec3(-0.5f, 1.2f, -1.0f));
        cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //barrier
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0, 0));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.5, .7, .1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;



        modelMatrixForContainer = glm::translate(model, glm::vec3(-.23f, 3.4f, -10.0f - 1.2));
        cube2.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.5, 1, .1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        modelMatrixForContainer = glm::translate(model, glm::vec3(3.5f, -.0f, -10.0f - 1.2));
        cube3.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        modelMatrixForContainer = glm::translate(model, glm::vec3(3.5f, 1.0f, -10.0f - 1.2));
        cube3.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        modelMatrixForContainer = glm::translate(model, glm::vec3(3.5f, 1.40f, -10.0f - 1.2));
        cube3.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);


        //photoframes
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.05, .7, .7));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        modelMatrixForContainer = glm::translate(model, glm::vec3(-48.5f, 1.9f, 2.5f));
        cube4.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.05, .6, .6));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        modelMatrixForContainer = glm::translate(model, glm::vec3(-48.3f, 2.3f, 3.0f));
        // cube4.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);
        cube5.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);


        //pillar

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.2, 0, -1.11));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(4, 3, .01));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        modelMatrixForContainer = glm::translate(model, glm::vec3(0.0f, 0.0f, 0));
        cube6.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //reception
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.3, 0, 2.7));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.1, .8, 1.9));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        modelMatrixForContainer = glm::translate(model, glm::vec3(.0f, 0.0f, 0));
        cube7.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.3, 0, 4.55));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.1, .8, .1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        modelMatrixForContainer = glm::translate(model, glm::vec3(.0f, 0.0f, 0));
        cube7.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.7, .8, 2.7));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.6, .05, 2.1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        modelMatrixForContainer = glm::translate(model, glm::vec3(.0f, 0.0f, 0));
        cube8.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.04, 3.1, 3.05));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(5.85, 0, 1.5));
        model = translateMatrix * scaleMatrix;
        modelMatrixForContainer = glm::translate(model, glm::vec3(.0f, 0.0f, 0));
        cube10.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //door
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.1, 1.8, .05));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(door + 6.2, 0, 1.55)); //x-axis 7
        model = translateMatrix * scaleMatrix;
        modelMatrixForContainer = glm::translate(model, glm::vec3(.0f, 0.0f, 0));
        cube11.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);  //press E to move door
        if (dflag)
        {
            if (door < .76)
                door += .02;
        }
        if (!dflag)
        {
            if (door > 0.0f)
                door -= .02;
        }

        //fridge
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.1, 1.58, .02));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.88, .195, -3.52));
        model = translateMatrix * scaleMatrix;
        modelMatrixForContainer = glm::translate(model, glm::vec3(.0f, 0.0f, 0));
        cube15.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //oven
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.6, .399, .03));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(fr), glm::vec3(0.f, 1.0f, 0.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.12, 1 - .3 + .05, -4.23 + .4));
        model = translateMatrix * rotateYMatrix * scaleMatrix;
        modelMatrixForContainer = glm::translate(model, glm::vec3(.0f, 0.0f, 0));
        cube16.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);
        if (of)
        {
            fr = -86.0f;
        }
        else
        {
            if (fr != 0.0f)
                fr = 0.0f;
        }



        if (dLight)
        {
            lightingShader.setBool("dlighton", true);
            lightingShader.setVec3("directionalLight.direction", .5, 3.0, -3.0);
            lightingShader.setVec3("directionalLight.ambient", .2, .2, .2);
            lightingShader.setVec3("directionalLight.diffuse", .8, .8, 0.8);
            lightingShader.setVec3("directionalLight.specular", 1.0, 1.0, 1.0);
        }


        spotlight1.setUpspotLight(lightingShader);
        // directionallight1.setUpDirectionalLight(lightingShader);
         // activate shader
        lightingShader.use();
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
    // glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);




        //sphere
        Sphere sphere = Sphere();

        glm::vec3 amb = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 diff = glm::vec3(.1f, .1f, 0.1f);
        glm::vec3 spec = glm::vec3(0.5f, 0.5f, 0.5f);
        Sphere sphere1 = Sphere(.2, 36, 18, amb, diff, spec);


        amb = glm::vec3(1.0f, 1.0f, 1.0f);
        diff = glm::vec3(1.0f, 1.0f, 1.0f);
        spec = glm::vec3(0.5f, 0.5f, 0.5f);
        Sphere sphere2 = Sphere(.2, 36, 18, amb, diff, spec);
        //.154 * 4.5, .231 * 4.5, .246 * 4.5
        amb = glm::vec3(.154 * 1.5, .231 * 1.5, .246 * 1.5);
        diff = glm::vec3(.154 * 3.5, .231 * 3.5, .246 * 3.5);
        spec = glm::vec3(0.5f, 0.5f, 0.5f);
        Sphere sph = Sphere(.2, 36, 18, amb, diff, spec);
        amb = glm::vec3(.1, .1, .1);
        diff = glm::vec3(0, 0, 0);
        spec = glm::vec3(0.5f, 0.5f, 0.5f);
        Sphere flash = Sphere(.2, 36, 18, amb, diff, spec);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0, 0));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1, 1, 1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        glm::mat4 modelForSphere = glm::mat4(1.0f);
        // modelForSphere = glm::translate(model, glm::vec3(1.5f, 1.2f, 0.5f));
        modelForSphere = glm::translate(model, glm::vec3(2.0f, 1.0f, 1.0f));
        // sphere1.drawSphere(lightingShader, modelForSphere);


        //pillers
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z - 1.1));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.05, 2.4, .05));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(.2, translate_Y, translate_Z - 1.1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(.4, translate_Y, translate_Z - 1.1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(.6, translate_Y, translate_Z - 1.1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(.8, translate_Y, translate_Z - 1.1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(1, translate_Y, translate_Z - 1.1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.2, translate_Y, translate_Z - 1.1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.4, translate_Y, translate_Z - 1.1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-.2, translate_Y, translate_Z - 1.1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-.4, translate_Y, translate_Z - 1.1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-.6, translate_Y, translate_Z - 1.1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-.8, translate_Y, translate_Z - 1.1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1, translate_Y, translate_Z - 1.1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.2, translate_Y, translate_Z - 1.1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.4, translate_Y, translate_Z - 1.1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.6, translate_Y, translate_Z - 1.1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.8, translate_Y, translate_Z - 1.1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2, translate_Y, translate_Z - 1.1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.2, translate_Y, translate_Z - 1.1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);







        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);

        table(cubeVAO, lightingShader, model);


        translateMatrix = glm::translate(identityMatrix, glm::vec3(2.0, translate_Y, 2.0));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = translateMatrix * rotateYMatrix * scaleMatrix;
        table(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0., translate_Y, 2.0));
        model = translateMatrix * rotateYMatrix * scaleMatrix;
        table(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0., translate_Y, 4.0));
        model = translateMatrix * rotateYMatrix * scaleMatrix;
        table(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.0, translate_Y, 0.0));
        model = translateMatrix * rotateYMatrix * scaleMatrix;
        table(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(2.0, translate_Y, 4.0));
        model = translateMatrix * rotateYMatrix * scaleMatrix;
        table(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.0, translate_Y, 2.0));
        model = translateMatrix * rotateYMatrix * scaleMatrix;
        table(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.0, translate_Y, 4.0));
        model = translateMatrix * rotateYMatrix * scaleMatrix;
        table(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.1, translate_Y, -3.8));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.7, 1.4, 1.2));
        model = translateMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);

        table(cubeVAO, lightingShader, model);



        //chair


        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, .15));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);


        translateMatrix = glm::translate(identityMatrix, glm::vec3(.5, translate_Y, .15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);


        translateMatrix = glm::translate(identityMatrix, glm::vec3(0, translate_Y, 2.15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(.5, translate_Y, 2.15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(.0, translate_Y, 4.15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(.5, translate_Y, 4.15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(2.0, translate_Y, 2.15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5, translate_Y, 2.15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(2.0, translate_Y, 4.15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5, translate_Y, 4.15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.0, translate_Y, .15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.5, translate_Y, .15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.0, translate_Y, 2.15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.5, translate_Y, 2.15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.0, translate_Y, 4.15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.5, translate_Y, 4.15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + .3, translate_Y, translate_Z - .6));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + .3 + .5, translate_Y, translate_Z - .6));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);


        translateMatrix = glm::translate(identityMatrix, glm::vec3(0 + .3, translate_Y, translate_Z - .7 + 2.15));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);


        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + .3 + .5, translate_Y, -.7 + 2.15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(.3, translate_Y, translate_Z - .7 + 4.15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(+.3 + .5, translate_Y, -.7 + 4.15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(2 + .3, translate_Y, translate_Z - .7 + 2.15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(2 + .3 + .5, translate_Y, -.7 + 2.15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(4 + .3, translate_Y, translate_Z - .7 + .15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(4 + .3 + .5, translate_Y, -.7 + .15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(4 + .3, translate_Y, translate_Z - .7 + 2.15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(4 + .3 + .5, translate_Y, -.7 + 2.15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(4 + .3, translate_Y, translate_Z - .7 + 4.15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(4 + .3 + .5, translate_Y, -.7 + 4.15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(2 + .3, translate_Y, translate_Z - .7 + 2.15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(2 + .3 + .5, translate_Y, -.7 + 2.15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(2 + .3, translate_Y, translate_Z - .7 + 4.15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(2 + .3 + .5, translate_Y, -.7 + 4.15));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        chair(cubeVAO, lightingShader, model);



        //floor
        glm::mat4 translate = glm::mat4(1.0f);
        //translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.5, 0.03, 10.0));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.5, translate_Y, -4.5));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.99, 0.99, 1.0);


        //wall
        //left
        translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.1, 3.1, 10.0));

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.5, translate_Y, -4.5));
        model = translateMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, .1, .1, .6);
        //right
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.1, 3.1, 10.0));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(8.0, translate_Y, -4.5));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, .1, .1, .6);
        //back
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.5, 3.1, .1));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.5, translate_Y, -4.5));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, .6);
        //roof
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.5, 0.05, 10.0));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.5, 3.05, -4.5));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.1, 0.5, .9);


        //washroom
       // glm::mat4 translate = glm::mat4(1.0f);
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.1, 3.1, .05));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(5.5 + .4, translate_Y, 4.5));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.99, 0.99, 1.0);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(5.5 + .4, translate_Y, 4.55));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        modelMatrixForContainer = glm::translate(model, glm::vec3(.0f, 0.0f, 0));
        cube14.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.1, 3.1, .05));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(5.5 + .4, translate_Y, 1.5));
        model = translateMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.99, 0.99, 1.0);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.1, 1.3, .05));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(6.5 + .4, 1.8, 1.5));
        model = translateMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.99, 0.99, 1.0);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.05, 3.1, 3));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(5.9, 0, 1.5));
        model = translateMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.99, 0.99, 1.0);

        //doorFrame
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.06, 1.8, .06));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(7.9, 0, 1.5));
        model = translateMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, .1);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(7, 0, 1.5));
        model = translateMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, .1);



        //holder





        amb = glm::vec3(.1, .1, .1);
        diff = glm::vec3(0, .5, .1);
        spec = glm::vec3(0.5f, 0.5f, 0.5f);
        Sphere f2 = Sphere(.2, 36, 18, amb, diff, spec);




        translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 2.65, 0));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix;
        fan_holder(cubeVAO, lightingShader, model, f2);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 2.65, 3.1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix;
        fan_holder(cubeVAO, lightingShader, model, f2);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(4, 2.65, 0));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix;
        fan_holder(cubeVAO, lightingShader, model, f2);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(4, 2.65, 3.1));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix;
        fan_holder(cubeVAO, lightingShader, model, f2);

        // translate = glm::translate(identityMatrix, glm::vec3(1, 2.66, 0));
        // translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0, 0));

        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0, 0));
        model = rotateYMatrix * translateMatrix;
        fan(cubeVAO, lightingShader, model);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0, 3.1));
        rotateYMatrix = translateMatrix * glm::rotate(identityMatrix, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(identityMatrix, glm::vec3(0, 0, -3.10));
        model = rotateYMatrix * translateMatrix;
        fan(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(4, 0, 0));
        rotateYMatrix = translateMatrix * glm::rotate(identityMatrix, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(identityMatrix, glm::vec3(-4.0, 0, 0));
        model = rotateYMatrix * translateMatrix;
        fan(cubeVAO, lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(4, 0, 3.1));
        rotateYMatrix = translateMatrix * glm::rotate(identityMatrix, glm::radians(r), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(identityMatrix, glm::vec3(-4, 0, -3.10));
        model = rotateYMatrix * translateMatrix;
        fan(cubeVAO, lightingShader, model); //press p
        if (fanSwitch)
        {
            r += 30.0;
        }
        else
        {
            r = 0.0f;
        }



















        kitchen(cubeVAO, lightingShader, lightingShaderWithTexture, cube9, sphere1);
        basin(cubeVAO, lightingShader, sphere1, sphere2);

        amb = glm::vec3(1.0f, 1.0f, 1.0f);
        diff = glm::vec3(.2f, .2f, .2f);
        spec = glm::vec3(0.5f, 0.5f, 0.5f);
        Sphere sphere3 = Sphere(1, 36, 18, amb, diff, spec);
        stove(cubeVAO, lightingShader, sphere3);

        //drawer  
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.04, .3, .7));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.01, .7 - .33, z_move1 - 1.82));
        model = translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, model, .1, .1, .1);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5 - .05, .7 - .33, z_move1 - 1.82));
        model = translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, model, .1, .1, .1);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.45, .01, .7));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0, .7 - .33, z_move1 - 1.82));
        model = translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, model, .1, .1, .1);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5, .3, .04));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0, .7 - .33, z_move1 - 1.85));
        model = translateMatrix * scaleMatrix;
        modelMatrixForContainer = glm::translate(model, glm::vec3(.0f, 0.0f, 0));
        cube12.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

        //drawer2
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.04, .3, .7));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.01, .7 - .33 - .33, z_move2 - 1.82));
        model = translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, model, .1, .1, .1);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.5 - .05, .7 - .33 - .32, z_move2 - 1.82));
        model = translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, model, .1, .1, .1);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.45, .01, .7));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0, .7 - .33 - .32, z_move2 - 1.82));
        model = translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, model, .1, .1, .1);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5, .3, .04));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0, .7 - .33 - .33, z_move2 - 1.85));
        model = translateMatrix * scaleMatrix;
        modelMatrixForContainer = glm::translate(model, glm::vec3(.0f, 0.0f, 0));
        cube12.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer); //press q,w to open drawer


        //tap
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.015, tap_w + .06f, .015));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(7.8, 1, -1.076));
        model = translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, model, .154 * 4.5, .231 * 4.5, .246 * 4.5);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.02, tap_w + .06f, .02));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.87, 1.05, -1.7));
        model = translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, model, .154 * 4.5, .231 * 4.5, .246 * 4.5);//press T

        if (tflag)
        {
            if (tap_w > -.3)
            {
                tap_w -= .01;
            }

        }
        if (!tflag)
        {
            if (tap_w < 0.0f)
            {
                tap_w = 0;
            }

        }

        //fridge
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.87, 0, -4.3));

        model = translateMatrix * scaleMatrix;
        fridge(cubeVAO, lightingShader, sphere, translateMatrix);


        //handles

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.545, 1.77, .02));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(frotate), glm::vec3(0.0f, 1.0f, 0.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.875, 0, -4.3 + .8));
        model = translateMatrix * rotateYMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, model, .01, .01, .01);
        //change=glm::rotate(identityMatrix, glm::radians(frotate2), glm::vec3(0.f, 1.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(frotate2), glm::vec3(0.f, 1.0f, 0.0f));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(4.87 + 1.13, 0, -4.3 + .81));
        model = change * translateMatrix * glm::rotate(identityMatrix, glm::radians(180.f), glm::vec3(0.f, 1.0f, 0.0f)) * rotateYMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, model, .01, .01, .01);
        if (fflag)
        {
            frotate = -86.1f;
        }
        if (!fflag)
        {
            if (frotate != 0.0f)
                frotate = 0.0f;
        }
        if (fflag2)
        {

            frotate2 = 86.1f;
        }
        if (!fflag2)
        {
            if (frotate2 != 0.0f)
                frotate2 = 0.0f;
        }
        //logo
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.03, .8, 1.1));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.4, 1.8, 3.3));
        model = translateMatrix * scaleMatrix;
        modelMatrixForContainer = glm::translate(model, glm::vec3(.0f, 0.0f, 0));
        cube13.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);


        //curve
        lightingShader.use();

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.9, -.1, -.75));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.4, scale_Y * .5, .4));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        bezierVAO = hollowBezier(vase.data(), ((unsigned int)vase.size() / 3) - 1);
        lightingShader.use();
        lightingShader.setVec3("material.ambient", glm::vec3(.2f, .2f, .2f));
        lightingShader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 1.0f));
        lightingShader.setVec3("material.specular", glm::vec3(.7f, .7f, .7f));
        lightingShader.setFloat("material.shininess", 32.0f);
        glBindVertexArray(bezierVAO);
        glDrawElements(GL_TRIANGLES,                    // primitive type
            (unsigned int)indices.size(),          // # of indices
            GL_UNSIGNED_INT,                 // data type
            (void*)0);                       // offset to indices


        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.9, 2, -2.75));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.2, scale_Y * .2, .2));
        model = translateMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        bezierVAO = hollowBezier(bowl.data(), ((unsigned int)bowl.size() / 3) - 1);
        lightingShader.use();
        lightingShader.setVec3("material.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, .0f));
        lightingShader.setVec3("material.specular", glm::vec3(1, 1, 1));
        lightingShader.setFloat("material.shininess", 32.0f);
        glBindVertexArray(bezierVAO);
        glDrawElements(GL_TRIANGLES,                    // primitive type
            (unsigned int)indices.size(),          // # of indices
            GL_UNSIGNED_INT,                 // data type
            (void*)0);                       // offset to indices


        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.9, 2, -3.25));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.2, scale_Y * .3, .2));
        model = translateMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        bezierVAO = hollowBezier(bowl.data(), ((unsigned int)bowl.size() / 3) - 1);
        lightingShader.use();
        lightingShader.setVec3("material.diffuse", glm::vec3(.5f, 1.5f, .0f));
        glBindVertexArray(bezierVAO);
        glDrawElements(GL_TRIANGLES,                    // primitive type
            (unsigned int)indices.size(),          // # of indices
            GL_UNSIGNED_INT,                 // data type
            (void*)0);                       // offset to indices
        // unbind VAO


        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.9, .9, -4.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.2, scale_Y * .1, .4));
        model = translateMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        bezierVAO = hollowBezier(bowl.data(), ((unsigned int)bowl.size() / 3) - 1);
        lightingShader.use();
        lightingShader.setVec3("material.diffuse", glm::vec3(.8f, .5f, .4f));
        glBindVertexArray(bezierVAO);
        glDrawElements(GL_TRIANGLES,                    // primitive type
            (unsigned int)indices.size(),          // # of indices
            GL_UNSIGNED_INT,                 // data type
            (void*)0);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(.56, .74, -1.5));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.2, scale_Y * .05, .2));
        model = translateMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        bezierVAO = hollowBezier(bowl.data(), ((unsigned int)bowl.size() / 3) - 1);
        lightingShader.use();
        lightingShader.setVec3("material.diffuse", glm::vec3(.8f, .01f, .01f));
        glBindVertexArray(bezierVAO);
        glDrawElements(GL_TRIANGLES,                    // primitive type
            (unsigned int)indices.size(),          // # of indices
            GL_UNSIGNED_INT,                 // data type
            (void*)0);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(7.56, -.3, -1.7));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.3, scale_Y * .4, .3));
        model = translateMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        bezierVAO = hollowBezier(bin.data(), ((unsigned int)bin.size() / 3) - 1);
        lightingShader.use();
        lightingShader.setVec3("material.diffuse", glm::vec3(0.545, 0.271, 0.075));
        glBindVertexArray(bezierVAO);
        glDrawElements(GL_TRIANGLES,                    // primitive type
            (unsigned int)indices.size(),          // # of indices
            GL_UNSIGNED_INT,                 // data type
            (void*)0);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.3, .081, -0.2));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.5, .5, .5));
        model = translateMatrix * scaleMatrix;
        lamp(lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.3 + 4, .081, -0.2));
        model = translateMatrix * scaleMatrix;
        lamp(lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.3, .081, -0.2 + 2));
        model = translateMatrix * scaleMatrix;
        lamp(lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.3, .081, -0.2 + 4));
        model = translateMatrix * scaleMatrix;
        lamp(lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.3 + 2, .081, -0.2 + 4));
        model = translateMatrix * scaleMatrix;
        lamp(lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.3 + 4, .081, -0.2 + 4));
        model = translateMatrix * scaleMatrix;
        lamp(lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.3 + 2, .081, -0.2 + 2));
        model = translateMatrix * scaleMatrix;
        lamp(lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.3 + 4, .081, -0.2 + 2));
        model = translateMatrix * scaleMatrix;
        lamp(lightingShader, model);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.7, .52, -3));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5, 1, 1.5));
        model = translateMatrix * scaleMatrix;
        objects(lightingShader, model, .5, 0, 0, plate);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.7, .55, -3));
        objects(lightingShader, translateMatrix, .5, 0.5, 0, plate);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.7, .6, -3));
        objects(lightingShader, translateMatrix, .5, 0.5, 0.8, plate);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.7, .6 + .4, -2.8));
        objects(lightingShader, translateMatrix, .5, 0.5, 0.8, plate);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.7, 1.05, -2.8));
        objects(lightingShader, translateMatrix, 0, 0.5, 0, plate);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.7, 1.1, -2.8));
        objects(lightingShader, translateMatrix, 0, 0.5, 0.5, plate);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.7, 1.15, -2.8));
        objects(lightingShader, translateMatrix, 0.5, 0.5, 0, plate);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.7, 1.2, -2.8));
        objects(lightingShader, translateMatrix, 0.5, 0.5, 0.5, plate);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.8, 1, .6));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.7, .6 + .4, -4));
        model = translateMatrix * scaleMatrix;
        objects(lightingShader, model, .5, 0.5, 0.8, plate);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.7, 1.05, -4));
        model = translateMatrix * scaleMatrix;
        objects(lightingShader, model, 0, 0.5, 0, plate);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.7, 1.1, -4));
        model = translateMatrix * scaleMatrix;
        objects(lightingShader, model, 0, 0.5, 0.5, plate);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.7, 1.15, -4));
        model = translateMatrix * scaleMatrix;
        objects(lightingShader, model, 0.5, 0.5, 0, plate);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.7, 1.2, -4));
        model = translateMatrix * scaleMatrix;
        objects(lightingShader, model, 0.5, 0.5, 0.5, plate);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.8, .9, 1.4));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.7, .6 + .4 + .5, -3.5 - .5));
        model = translateMatrix * scaleMatrix;
        //plates(lightingShader, model, .5, 0.5, 0.8);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.7, 1.05, -3.5));
        model = translateMatrix * scaleMatrix;
        objects(lightingShader, model, 0, 0.5, 0, plate);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.7, 1.1, -3.5));
        model = translateMatrix * scaleMatrix;
        objects(lightingShader, model, 0, 0.5, 0.5, plate);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.7, 1.15, -3.5));
        model = translateMatrix * scaleMatrix;
        objects(lightingShader, model, 0.5, 0.5, 0, plate);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.7, 1.2, -3.5));
        model = translateMatrix * scaleMatrix;
        objects(lightingShader, model, 0.5, 0.5, 0.5, plate);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5, .8, 1.5));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.8, 0, .1));
        model = translateMatrix * scaleMatrix;
        objects(lightingShader, model, 0.5, 0.5, 0., comode);

        //toilet
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.25, .56, 1.1));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(7.5, +.3, 3.6));
        model = translateMatrix * scaleMatrix;
        objects(lightingShader, model, 1, 1, 1, comode);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5, .7, 1.5));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(7.6, 0, 3.6));
        model = translateMatrix * scaleMatrix;
        objects(lightingShader, model, 1, 1, 1, pipe);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.6, .5, .6));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(7.6, .4, 3.6));
        model = translateMatrix * scaleMatrix;
        sph.drawSphere(lightingShader, model);

        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.2, .05, .4));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(7.8, .46, 3.4));
        model = translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, model, 1, 1, 1);
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.3, .55, .5));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(7.8, .48, 3.4));
        model = translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, model, 1, 1, 1);
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.32, .2, .52));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(7.77, .48 + .4, 3.39));
        model = translateMatrix * scaleMatrix;
        drawCube(cubeVAO, lightingShader, model, .5, .5, .5);
        //flash
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(.2, .2, .2));
        translateMatrix = glm::translate(identityMatrix, glm::vec3(7.87, .48 + .59, 4 - .34));
        model = translateMatrix * scaleMatrix;
        flash.drawSphere(lightingShader, model);

        //oven
        model = glm::mat4(1.0f);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.12, 0 - .3 + .05, -4.25));
        oven(cubeVAO, lightingShader, lightingShaderWithTexture, translateMatrix);







        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 7; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.1f)); // Make it a smaller cube
            ourShader.setMat4("model", model);
            //ourShader.setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
            // ourShader.setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
            ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
void lamp(Shader& lightingShader, glm::mat4 all)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 scaleMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, translateMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 1, 0));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, scale_Y * .15, .25));
    model = all * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    bezierVAO = hollowBezier(lamp_h.data(), ((unsigned int)lamp_h.size() / 3) - 1);
    lightingShader.use();
    lightingShader.setVec3("material.diffuse", glm::vec3(.5f, .5f, 1.0f));
    glBindVertexArray(bezierVAO);
    glDrawElements(GL_TRIANGLES,                    // primitive type
        (unsigned int)indices.size(),          // # of indices
        GL_UNSIGNED_INT,                 // data type
        (void*)0);                       // offset to indices
    // unbind VAO
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, .8, 0));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.15, scale_Y * .25, .15));
    model = all * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    bezierVAO = hollowBezier(lamp_b.data(), ((unsigned int)lamp_b.size() / 3) - 1);
    lightingShader.use();
    lightingShader.setVec3("material.diffuse", glm::vec3(1.0f, 01.0f, .0f));
    glBindVertexArray(bezierVAO);
    glDrawElements(GL_TRIANGLES,                    // primitive type
        (unsigned int)indices.size(),          // # of indices
        GL_UNSIGNED_INT,                 // data type
        (void*)0);                       // offset to indices
    // unbind VAO


}
void objects(Shader& lightingShader, glm::mat4 all, float r, float g, float b, vector<float> v)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 scaleMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, translateMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0, 0));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.25, .35, .25));
    model = all * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    bezierVAO = hollowBezier(v.data(), ((unsigned int)v.size() / 3) - 1);
    lightingShader.use();
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    glBindVertexArray(bezierVAO);
    glDrawElements(GL_TRIANGLES,                    // primitive type
        (unsigned int)indices.size(),          // # of indices
        GL_UNSIGNED_INT,                 // data type
        (void*)0);
}
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 32.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}
void fridge(unsigned int& cubeVAO, Shader& lightingShader, Sphere& sp, glm::mat4 alTogether)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 scaleMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, translateMatrix, model;
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.1, 1.8, .02));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0, 0));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.83, 0.83, 0.83);

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.02, 1.8, .8));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.83, 0.83, 0.83);


    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.1, 0, 0));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.83, 0.83, 0.83);

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.05, 1.8, .82));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.55, .0, 0));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.83, 0.83, 0.83);

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.1, .02, .8));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 1.8 - .02, 0));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.83, 0.83, 0.83);
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.2, .15, .85));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-.025, .02, 0));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.23, 0.23, 0.23);


    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.1, .02, .7));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 1.8 - .02 - .4, 0));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.83, 0.83, 0.83);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 1.8 - .02 - .8, 0));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.83, 0.83, 0.83);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 1.8 - .02 - 1.2, 0));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.83, 0.83, 0.83);



}
void oven(unsigned int& cubeVAO, Shader& lightingShader, Shader& lightingShaderWithTexture, glm::mat4 alTogether)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 scaleMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, translateMatrix, model;
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.6, .4, .02));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 1, 0));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.83, 0.83, 0.83);

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.02, .4, .4));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 1, 0));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.83, 0.83, 0.83);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(.6 - .02, 1, 0));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.83, 0.83, 0.83);

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.6, .02, .4));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 1, 0));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.83, 0.83, 0.83);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 1.4 - .02, 0));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.83, 0.83, 0.83);
}

void fan_holder(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Sphere& sp)
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(.2, .05, .1));
    translate = glm::translate(model, glm::vec3(-0.5, 0.0, -0.5));
    translate2 = glm::translate(model, glm::vec3(-0., 0.3, -0.));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.03, 0.03, 0.03);


    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(.02, .3, .02));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.03, 0.03, 0.03);


    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(.7, .3, .7));
    translate = glm::translate(model, glm::vec3(-0.3 + .32, 0.15, 0));
    model = alTogether * scale * translate;
    sp.drawSphere(lightingShader, model);


}
void kitchen(unsigned int& cubeVAO, Shader& lightingShader, Shader& lightingShaderWithTexture, Cube& cube, Sphere& sp)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 scaleMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, translateMatrix, model;
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1, .9, 2.1));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0, 0));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    //table
    //drawCube(cubeVAO, lightingShaderWithTexture, model, 0.23, 0., 0.23);
    glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
    modelMatrixForContainer = glm::translate(model, glm::vec3(-2.4, 0, -2.1));
    cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.1, .9, 1.2));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.5, 0, -2.32));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.143, 0.084, 0.071);

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1, .05, 2.1));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.5, 1.4, -4.4));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.143, 0.084, 0.071);


    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.5, 2, -4.4));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.143, 0.084, 0.071);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.5, 2.5, -4.4));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.143, 0.084, 0.071);

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1, 1.2, .05));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.4, 1.35, -2.3));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.143, 0.084, 0.071);


    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.1, 1.3, 1.2));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.5, 1.3, -2.32));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    //drawCube(cubeVAO, lightingShaderWithTexture, model, 0.33, 0., 0.33);


    //sink
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.1, 1, 1.15));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.1, .7, -2.3));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    sink(cubeVAO, lightingShader, model);

    //tap
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5, 1, 1.5));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.1, .9, -2.75));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    tap(cubeVAO, lightingShader, model, sp);

}
void basin(unsigned int& cubeVAO, Shader& lightingShader, Sphere& sp, Sphere& sp2)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 modelForSphere = glm::mat4(1.0f);
    glm::mat4 scaleMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, translateMatrix, model;
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.2, 1, .8));

    translateMatrix = glm::translate(identityMatrix, glm::vec3(7.4, .7, -1.5));
    model = translateMatrix * scaleMatrix;
    sink(cubeVAO, lightingShader, model);


    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(8, .8, -0.5));
    model = translateMatrix * rotateYMatrix * scaleMatrix;
    tap(cubeVAO, lightingShader, model, sp);

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.8, .1, .8));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0, 0));
    model = translateMatrix * scaleMatrix;
    modelForSphere = glm::translate(model, glm::vec3(9.6f, .01f, -1.4));
    sp2.drawSphere(lightingShader, modelForSphere);

    float y = .01f;
    for (int i = 0; i < 390; i++)
    {
        model = translateMatrix * scaleMatrix;
        modelForSphere = glm::translate(model, glm::vec3(9.6f, y, -1.4));
        sp2.drawSphere(lightingShader, modelForSphere);
        y += .02f;
    }

}

void tap(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, Sphere& sp)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 modelForSphere = glm::mat4(1.0f);
    glm::mat4 scaleMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, translateMatrix, model;
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.02, .3, .02));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0, 0.7));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.17, .02, .02));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, .3, .7));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.02, .15, .02));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(.15, .15, 0.7));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.01, .01, .12));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(.01, .15, 0.65));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);


    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.13, .12, .12));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0, 0));
    model = alTogether * translateMatrix * scaleMatrix;
    modelForSphere = glm::translate(model, glm::vec3(0.125f, 1.3f, 6.5));
    sp.drawSphere(lightingShader, modelForSphere);

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.13, .12, .12));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0, 0));
    model = alTogether * translateMatrix * scaleMatrix;
    modelForSphere = glm::translate(model, glm::vec3(0.125f, 1.3f, 5.3));
    sp.drawSphere(lightingShader, modelForSphere);








}
void sink(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 scaleMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, translateMatrix, model;
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.5, .1, 1));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0, 0));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.9, 0.9);

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.5, .2, .03));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0, 0));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.9, 0.9);


    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.5, .2, .03));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0, 1));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.9, 0.9);


    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.03, .2, 1));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0, 0));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.9, 0.9);


    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.03, .2, 1));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.47, 0, 0));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.9, 0.9);

    //black
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.03, .03, .03));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.25, .08, 0.5));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);

}

void stove(unsigned int& cubeVAO, Shader& lightingShader, Sphere& sp)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 modelForSphere = glm::mat4(1.0f);
    glm::mat4 scaleMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, translateMatrix, model;
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5, .7, .7));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0, -1.82));
    model = translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 1.0, 0.9, 0.9);

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5, .01, .7));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, .7, -1.82));
    model = translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, .1, .1, .1);

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.17, .01, .17));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0, 0));
    model = translateMatrix * scaleMatrix;
    modelForSphere = glm::translate(model, glm::vec3(3.0f, 71.0f, -8.55));
    sp.drawSphere(lightingShader, modelForSphere);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 0, 0));
    model = translateMatrix * scaleMatrix;
    modelForSphere = glm::translate(model, glm::vec3(6.0f, 71.0f, -8.55));
    sp.drawSphere(lightingShader, modelForSphere);

}

void chair(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether)
{
    float legHeightmin = .3;
    float legHeightmax = 1.3;
    float seat = .04;
    float top_len = 1.2;
    float top_h = .02;
    float top_w = 0.6;
    glm::mat4 identityMatrix = glm::mat4(1.0f);


    //long_left
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(.02, .7, .02));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);




    //long_right
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    glm::mat4 model2 = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(.02, .7, .02));
    translate2 = glm::translate(identityMatrix, glm::vec3(.3, 0, 0.0));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);

    //short_left
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);

    scale = glm::scale(model, glm::vec3(.02, .35, .02));
    translate2 = glm::translate(identityMatrix, glm::vec3(.0, 0, -0.3));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);


    //short_right
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);

    scale = glm::scale(model, glm::vec3(.02, .35, .02));
    translate2 = glm::translate(identityMatrix, glm::vec3(.3, 0, -0.3));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.1, 0.1, 0.1);

    //seat
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);

    scale = glm::scale(model, glm::vec3(.4, .02, .4));
    translate2 = glm::translate(identityMatrix, glm::vec3(.15, .35, -0.2));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);

    //baack
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);

    scale = glm::scale(model, glm::vec3(.4, .16, .015));
    translate2 = glm::translate(identityMatrix, glm::vec3(0.15, .5, -0.03));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);

}


void fan(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether)
{



    //fin

    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 modelForSphere = glm::mat4(1.0f);
    glm::mat4 scaleMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, translateMatrix, model;
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.65, .02, .12));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 2.71, 0));
    model = alTogether * translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0, .5, .1);

    // scaleMatrix = glm::scale(identityMatrix, glm::vec3(.65, .02, .12));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 1, 0));
    model = alTogether * translateMatrix * rotateYMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0, .5, .1);

    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(240.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //  translateMatrix = glm::translate(identityMatrix, glm::vec3(0, 1, 0));
    model = alTogether * translateMatrix * rotateYMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0, .5, .1);


}




void table(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether)
{
    float legHeight = .5;
    float legwidth = .03;
    float top_len = 1.3;
    float top_h = .02;
    float top_w = 0.6;
    glm::mat4 identityMatrix = glm::mat4(1.0f);


    //leg_left1
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    glm::mat4 translate1 = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(legwidth, legHeight, legwidth));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    translate1 = glm::translate(identityMatrix, glm::vec3(-.2, 0, 0));
    model = alTogether * translate1 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);

    //leg_left2
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    glm::mat4 model2 = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(legwidth, legHeight, legwidth));
    translate2 = glm::translate(identityMatrix, glm::vec3(-0.2, 0, -0.4));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);

    //right1
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(legwidth, legHeight, legwidth));
    translate2 = glm::translate(identityMatrix, glm::vec3(1.0, 0, 0));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);

    //right2
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(legwidth, legHeight, legwidth));
    translate2 = glm::translate(identityMatrix, glm::vec3(1.0, 0, -0.4));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);

    //top
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    scale = glm::scale(identityMatrix, glm::vec3(top_len, top_h, -top_w));
    translate2 = glm::translate(identityMatrix, glm::vec3(.40, legHeight, -0.2));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 1, 0.35, 0);

}







// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        // if (rotateAxis_X) rotateAngle_X -= 0.1;
        // else if (rotateAxis_Y) rotateAngle_Y -= 0.1;
         //else rotateAngle_Z -= 0.1;
    }

    //fan_onOff

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        fanSwitch = !fanSwitch;

    }

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        of = !of;

    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        rotateAngle_X += 0.1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        rotateAngle_Y += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 1.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        rotateAngle_Z += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 1.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        if (z_move1 == 0.0f)
        {
            z_move1 -= .5f;
            flag = true;
        }
        else if (flag)
        {
            z_move1 += .5f;
            flag = false;
        }


    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        if (z_move2 == 0.0f)
        {
            z_move2 -= .5f;
            flag = true;
        }
        else if (flag)
        {
            z_move2 += .5f;
            flag = false;
        }

    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        dflag = !dflag;
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        tflag = !tflag;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        fflag = !fflag;
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        fflag2 = !fflag2;
    }


    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) //point
    {
        pLight = true;
        dLight = false;
        sLight = false;
    }

    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) //directional
    {
        pLight = false;
        if (dLight == false)
        {
            dLight = true;
        }


        sLight = false;
    }

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) //spot
    {
        pLight = false;
        dLight = false;
        sLight = true;
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        if (pLight)
        {
            pointlight1.turnOff();
            pointlight2.turnOff();
            pointlight3.turnOff();
            pointlight4.turnOff();
            pointlight5.turnOff();
            pointlight6.turnOff();
            pointlight7.turnOff();

        }
        if (sLight)
        {
            spotlight1.turnOff();
        }
        if (dLight)
        {
            //directionallight1.turnOff();
        }
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {

        if (pLight)
        {
            pointlight1.turnOn();
            pointlight2.turnOn();
            pointlight3.turnOn();
            pointlight4.turnOn();
            pointlight5.turnOn();
            pointlight6.turnOn();
            pointlight7.turnOn();
        }

        if (sLight)
        {
            spotlight1.turnOn();
        }
        if (dLight)
        {
            //directionallight1.turnOn();
        }

    }


    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        if (pLight)
        {
            pointlight1.turnAmbientOn();
            pointlight2.turnAmbientOn();
            pointlight3.turnAmbientOn();
            pointlight4.turnAmbientOn();
            pointlight5.turnAmbientOn();
            pointlight6.turnAmbientOn();
            pointlight7.turnAmbientOn();
        }

        if (sLight)
            spotlight1.turnAmbientOn();

        if (dLight)
        {
            //directionallight1.turnAmbientOn();
        }
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {

        if (pLight)
        {
            pointlight1.turnAmbientOff();
            pointlight2.turnAmbientOff();
            pointlight3.turnAmbientOff();
            pointlight4.turnAmbientOff();
            pointlight5.turnAmbientOff();
            pointlight6.turnAmbientOff();
            pointlight7.turnAmbientOff();
        }

        if (sLight)
            spotlight1.turnAmbientOff();

        if (dLight)
        {
            //directionallight1.turnAmbientOff();
        }
    }

    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {

        if (pLight)
        {
            pointlight1.turnDiffuseOn();
            pointlight2.turnDiffuseOn();
            pointlight3.turnDiffuseOn();
            pointlight4.turnDiffuseOn();
            pointlight5.turnDiffuseOn();
            pointlight6.turnDiffuseOn();
            pointlight7.turnDiffuseOn();
        }
        if (sLight)
            spotlight1.turnDiffuseOn();

        if (dLight)
        {
            //directionallight1.turnDiffuseOn();
        }
    }

    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        if (pLight)
        {

            pointlight1.turnDiffuseOff();
            pointlight2.turnDiffuseOff();
            pointlight3.turnDiffuseOff();
            pointlight4.turnDiffuseOff();
            pointlight5.turnDiffuseOff();
            pointlight6.turnDiffuseOff();
            pointlight7.turnDiffuseOff();
        }

        if (sLight)
            spotlight1.turnDiffuseOff();

        if (dLight)
        {
            //directionallight1.turnDiffuseOff();
        }
    }

    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
    {
        if (pLight)
        {
            pointlight1.turnSpecularOn();
            pointlight2.turnSpecularOn();
            pointlight3.turnSpecularOn();
            pointlight4.turnSpecularOn();
            pointlight5.turnSpecularOn();
            pointlight6.turnSpecularOn();
            pointlight7.turnSpecularOn();
        }
        if (sLight)
            spotlight1.turnSpecularOn();

        if (dLight)
        {
            //directionallight1.turnSpecularOn();
        }
    }


    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
    {
        if (pLight)
        {
            pointlight1.turnSpecularOff();
            pointlight2.turnSpecularOff();
            pointlight3.turnSpecularOff();
            pointlight4.turnSpecularOff();
            pointlight5.turnSpecularOff();
            pointlight6.turnSpecularOff();
            pointlight7.turnSpecularOff();
        }

        if (sLight)
            spotlight1.turnSpecularOff();

        if (dLight)
        {
            //directionallight1.turnSpecularOff();
        }
    }


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
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

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
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0); //img er path, w and h and cmp mane koyta channel(rgb naki other)
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
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data); //0 mipmap level
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}



// This function converts the screen coordinates to world coordinates
// ---------------------------------------------------------------------------------------------
void scsToWcs(float sx, float sy, float wcsv[3])
{

    glm::vec4 winCoordinateVector; //vector to hold screen x,y,z coordinates
    glm::vec4 worldCoordinateVector; //vector to hold world x,y,z coordinates

    glGetIntegerv(GL_VIEWPORT, viewport); //get the viewport info

    // setup screen coordinates values
    winCoordinateVector.x = sx;
    winCoordinateVector.y = (float)viewport[3] - (float)sy; // reversed since y-coordinates go from bottom to top ,scrn er 00 opre wrld er ta niche
    winCoordinateVector.z = 0.0f;
    winCoordinateVector.w = 1.0f;

    // prepare viewport matrix
    float v_l = viewport[0], v_b = viewport[1], v_r = viewport[2], v_t = viewport[3];
    glm::mat4 viewportMat = glm::mat4(1.0f);
    viewportMat[0].x = (v_r - v_l) / 2.0f;  viewportMat[1].x = 0.0f; viewportMat[2].x = 0.0f;  viewportMat[3].x = (v_r + v_l) / 2.0f;
    viewportMat[0].y = 0.0f; viewportMat[1].y = (v_t - v_b) / 2.0f;  viewportMat[2].y = 0.0f;  viewportMat[3].y = (v_t + v_b) / 2.0f;
    viewportMat[0].z = 0.0f; viewportMat[1].z = 0.0f; viewportMat[2].z = 0.5f;  viewportMat[3].z = 0.5f;
    viewportMat[0].w = 0.0f; viewportMat[1].w = 0.0f; viewportMat[2].w = 0.0f;  viewportMat[3].w = 1.0f;

    //get the world coordinates from the screen coordinates
    worldCoordinateVector = glm::inverse(viewportMat * projection * view * model) * winCoordinateVector;
    wcsv[0] = worldCoordinateVector.x / worldCoordinateVector.w;
    wcsv[1] = worldCoordinateVector.y / worldCoordinateVector.w;
    wcsv[2] = worldCoordinateVector.z / worldCoordinateVector.w;
}

// glfw: whenever mouse button is pressed, this callback is called
// -------------------------------------------------------




long long nCr(int n, int r)
{
    if (r > n / 2)
        r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for (i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}

//polynomial interpretation for N points
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L) //t er jnno x,y er value ber kre dibe
{
    double y = 0;
    double x = 0;
    t = t > 1.0 ? 1.0 : t;
    for (int i = 0; i < L + 1; i++)
    {
        long long ncr = nCr(L, i);
        double oneMinusTpow = pow(1 - t, double(L - i));
        double tPow = pow(t, double(i));
        double coef = oneMinusTpow * tPow * ncr;
        x += coef * ctrlpoints[i * 3]; //x,y,z 3tai ache tai 3 gun kore kora
        y += coef * ctrlpoints[(i * 3) + 1];

    }
    xy[0] = float(x);
    xy[1] = float(y);
}

unsigned int hollowBezier(GLfloat ctrlpoints[], int L)
{
    int i, j;
    float x, y, z, r;                //current coordinates
    float theta;
    float nx, ny, nz, lengthInv;    // vertex normal
    coordinates.clear();
    normals.clear();
    indices.clear();
    vertices.clear();

    const float dtheta = 2 * pi / ntheta;        //angular step size

    float t = 0;
    float dt = 1.0 / nt;
    float xy[2];

    for (i = 0; i <= nt; ++i)              //step through y
    {
        BezierCurve(t, xy, ctrlpoints, L);
        r = xy[0];
        y = xy[1];
        theta = 0;
        t += dt;
        lengthInv = 1.0 / r;

        for (j = 0; j <= ntheta; ++j)
        {
            double cosa = cos(theta);
            double sina = sin(theta);
            z = r * cosa;
            x = r * sina;

            coordinates.push_back(x);
            coordinates.push_back(y);
            coordinates.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            // center point of the circle (0,y,0)
            nx = (x - 0) * lengthInv;
            ny = (y - y) * lengthInv;
            nz = (z - 0) * lengthInv;

            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            theta += dtheta;
        }
    }

    // generate index list of triangles
    // k1--k1+1
    // |  / |
    // | /  |
    // k2--k2+1

    int k1, k2;
    for (int i = 0; i < nt; ++i)
    {
        k1 = i * (ntheta + 1);     // beginning of current stack
        k2 = k1 + ntheta + 1;      // beginning of next stack

        for (int j = 0; j < ntheta; ++j, ++k1, ++k2)
        {
            // k1 => k2 => k1+1
            indices.push_back(k1);
            indices.push_back(k2);
            indices.push_back(k1 + 1);

            // k1+1 => k2 => k2+1
            indices.push_back(k1 + 1);
            indices.push_back(k2);
            indices.push_back(k2 + 1);
        }
    }

    size_t count = coordinates.size();
    for (int i = 0; i < count; i += 3)
    {
        vertices.push_back(coordinates[i]);
        vertices.push_back(coordinates[i + 1]);
        vertices.push_back(coordinates[i + 2]);

        vertices.push_back(normals[i]);
        vertices.push_back(normals[i + 1]);
        vertices.push_back(normals[i + 2]);
    }

    unsigned int bezierVAO;
    glGenVertexArrays(1, &bezierVAO);
    glBindVertexArray(bezierVAO);

    // create VBO to copy vertex data to VBO
    unsigned int bezierVBO;
    glGenBuffers(1, &bezierVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);           // for vertex data
    glBufferData(GL_ARRAY_BUFFER,                   // target
        (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
        vertices.data(),   // ptr to vertex data
        GL_STATIC_DRAW);                   // usage

    // create EBO to copy index data
    unsigned int bezierEBO;
    glGenBuffers(1, &bezierEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bezierEBO);   // for index data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
        (unsigned int)indices.size() * sizeof(unsigned int),             // data size, # of bytes
        indices.data(),               // ptr to index data
        GL_STATIC_DRAW);                   // usage

    // activate attrib arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // set attrib arrays with stride and offset
    int stride = 24;     // should be 24 bytes
    glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));

    // unbind VAO, VBO and EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return bezierVAO;
}

