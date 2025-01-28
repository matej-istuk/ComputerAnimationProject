//
// Created by babinsinko on 27/03/24.
//

#include <iostream>
#include <map>
#include <functional>
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "util/FPSManager.h"
#include "model/Mesh.h"
#include "util/loaders.h"
#include "model/Object.h"
#include "model/Scene.h"
#include "model/CoordinateSystem.h"
#include "model/SingleViewportRenderVisitor.h"
#include "model/InitializerRenderVisitor.h"
#include "model/ControlPolygon.h"
#include "model/BCurve.h"
#include "glm/geometric.hpp"

namespace ans {
    int width = 500, height = 500;
    const float MOVE_SPEED = 0.03;
    const float ROTATION_SPEED = 0.01;

    double prevXPos = -1;
    double prevYPos = -1;
    bool upDownActive = false;
    bool leftRightActive = false;

    std::shared_ptr<Scene> scene;
    std::vector<std::shared_ptr<Transformable>> transformables;
    std::shared_ptr<Transformable> activeTransformable;
    std::shared_ptr<Transformable> animatedTransformable;
    std::shared_ptr<ControlPolygon> controlPolygon;
    std::shared_ptr<ControlPolygon> tangent;
    std::shared_ptr<BCurve> curve;
    int cameraState = 0;
    int activeTransformableState = 0;
    int curveTick = 0;
    int curveTicksPerSegment = 500;
    int tangentCurveTick = 0;
    int tangentCurveTicksPerSegment = 500;

    std::vector<std::function<void()>> cameraModes = {
            []() -> void {
                scene->getCamera()->setLookAtPoint(scene->getCamera()->getPosition() + scene->getCamera()->getFront());
            },
            []() -> void {
                scene->getCamera()->setLookAtPoint(glm::vec4(0, 0, 0, 1));
            }
    };
    std::vector<std::function<void()>> activeTransformableModes = {
            []() -> void {
                animatedTransformable = nullptr;
                return;
            },
            []() -> void {
                tangent->clearPoints();
                if (tangentCurveTick == tangentCurveTicksPerSegment * curve->getNumSegments()) {
                    tangentCurveTick = 0;
                }
                float t = ((float) tangentCurveTick) / tangentCurveTicksPerSegment;
                glm::vec4 pointOnCurve = curve->value(t);
                glm::vec4 curveDerivative = curve->derivativeValue(t);
                tangent->addPoint(pointOnCurve);
                tangent->addPoint(pointOnCurve+curveDerivative);
                tangentCurveTick++;
            },
            []() -> void {
                if (animatedTransformable == nullptr) {
                    animatedTransformable = activeTransformable;
                }
                if (controlPolygon->getLinePoints().size() < 2) {
                    return;
                }
                if (curveTick == curveTicksPerSegment * curve->getNumSegments()) {
                    curveTick = 0;
                }
                float t = ((float) curveTick) / curveTicksPerSegment;
                animatedTransformable->setPosition(curve->value(t));
                glm::vec3 front = glm::normalize(glm::vec3(curve->derivativeValue(t)));
                glm::vec3 up = glm::normalize(glm::cross(front, glm::vec3(curve->secondDerivativeValue(t))));
                glm::vec3 right = glm::cross(up, front);
                animatedTransformable->setFront(glm::vec4(front, 0));
                animatedTransformable->setUp(glm::vec4(up, 0));
                animatedTransformable->setRight(glm::vec4(right, 0));
                curveTick++;
            }
    };

    std::map<int, std::function<void()>> holdKeyFunctionalities = {
            {GLFW_KEY_W, []() -> void {activeTransformable->localMove(glm::vec3(0, 0, MOVE_SPEED));}},
            {GLFW_KEY_S, []() -> void {activeTransformable->localMove(glm::vec3(0, 0, -MOVE_SPEED));}},
            {GLFW_KEY_A, []() -> void {activeTransformable->localMove(glm::vec3(MOVE_SPEED, 0, 0));}},
            {GLFW_KEY_D, []() -> void {activeTransformable->localMove(glm::vec3(-MOVE_SPEED, 0, 0));}},
            {GLFW_KEY_E, []() -> void {activeTransformable->localRotate(glm::vec3(0, 0, -MOVE_SPEED));}},
            {GLFW_KEY_Q, []() -> void {activeTransformable->localRotate(glm::vec3(0, 0, MOVE_SPEED));}},
            {GLFW_KEY_X, []() -> void {activeTransformable->localMove(glm::vec3(0, -MOVE_SPEED, 0));}},
            {GLFW_KEY_Z, []() -> void {activeTransformable->localMove(glm::vec3(0, MOVE_SPEED, 0));}}
    };
    std::map<int, std::function<void()>> pressKeyFunctionalities = {
            {GLFW_KEY_P, []() -> void {controlPolygon->addPoint(activeTransformable->getPosition());}},
            {GLFW_KEY_0, []() -> void {activeTransformable = transformables[0];}},
            {GLFW_KEY_1, []() -> void {activeTransformable = transformables[1];}},
            {GLFW_KEY_C, []() -> void {cameraState = (cameraState + 1) % cameraModes.size(); std::cout << "camera mode " << cameraState << std::endl;}},
            {GLFW_KEY_SPACE, []() -> void {activeTransformableState = (activeTransformableState + 1) % activeTransformableModes.size(); std::cout << "active transformable mode " << activeTransformableState << std::endl;}}
    };
}
//funkcija koja se poziva prilikom mijenjanja velicine prozora, moramo ju povezati pomocu glfwSetFramebufferSizeCallback
void framebuffer_size_callback(GLFWwindow * window, int Width, int Height)
{
    ans::width = Width;
    ans::height = Height;

    glViewport(0, 0, ans::width, ans::height);

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (ans::holdKeyFunctionalities.count(key)) {
        ans::holdKeyFunctionalities[key]();
    }
    if (ans::pressKeyFunctionalities.count(key) && action == GLFW_PRESS) {
        ans::pressKeyFunctionalities[key]();
    }
}

void cursor_position_callback(GLFWwindow * window, double xpos, double ypos)
{
    if (ans::upDownActive) {
        ans::activeTransformable->localRotate(glm::vec3((ypos-ans::prevYPos) * ans::ROTATION_SPEED, 0, 0));
    }
    if (ans::leftRightActive) {
        ans::activeTransformable->localRotate(glm::vec3(0, (xpos-ans::prevXPos) * ans::ROTATION_SPEED, 0));
    }
    ans::prevXPos = xpos;
    ans::prevYPos = ypos;

}

void mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        ans::upDownActive = true;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        ans::upDownActive = false;
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        ans::leftRightActive = true;
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        ans::leftRightActive = false;
    }
}

int main(int argc, char * argv[]) {
    std::cout << argv[0] << std::endl;
    /*********************************************************************************************/
    //postavljanje OpenGL konteksta, dohvacanje dostupnih OpenGL naredbi
    GLFWwindow* window;

    glfwInit();
    gladLoadGL();

    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);


    window = glfwCreateWindow(ans::width, ans::height, "Zadatak X", nullptr, nullptr);
    // provjeri je li se uspio napraviti prozor
    if (window == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    // dohvati sve dostupne OpenGL funkcije
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize GLAD");
        exit(-1);
    }
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    glfwSwapInterval(0); //ne cekaj nakon iscrtavanja (vsync)

    FPSManager FPSManagerObject(window, 144, 1.0, "Zadatak Xa");

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //funkcija koja se poziva prilikom mijenjanja velicine prozora
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    SingleViewportRenderVisitor wfVisitor;
    InitializerRenderVisitor iVisitor;

    ans::scene = std::make_shared<Scene>();


    std::shared_ptr<Shader> shaderObject = loadVertFragShader(argv[0], "shader1");

    std::shared_ptr<Shader> shaderCoord = loadVertFragShader(argv[0], "shader");
    std::shared_ptr<Shader> shaderLineStrip = loadVertFragShader(argv[0], "lsshader");
    std::shared_ptr<Mesh> meshHead = loadMesh(argv[0], "/resources/glava/glava.obj", true);
    std::shared_ptr<ObjectMaterial> materialHead = loadMaterial(argv[0], "/resources/glava/glava.obj");
    std::shared_ptr<ObjectTexture> textureHead = loadDiffuseTexture(argv[0], "/resources/glava/glava.obj");
    std::shared_ptr<std::vector<glm::vec4>> controlPoints = loadControlPolygon(argv[0], "resources/curve/spiral", 0.3);
    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    std::shared_ptr<Object> objectHead = std::make_shared<Object>(*shaderObject, materialHead, textureHead);
    std::shared_ptr<Object> objectGlobalAnchor = std::make_shared<Object>(*shaderCoord);
    std::shared_ptr<Object> objectCurve = std::make_shared<Object>(*shaderLineStrip);
    std::shared_ptr<Object> objectTangent = std::make_shared<Object>(*shaderLineStrip);
    std::shared_ptr<CoordinateSystem> coordinateSystem = std::make_shared<CoordinateSystem>();
    ans::controlPolygon = std::make_shared<ControlPolygon>();
    ans::tangent = std::make_shared<ControlPolygon>();
    std::shared_ptr<BCurve> bCurve = std::make_shared<BCurve>();
    std::shared_ptr<Light> light = std::make_shared<Light>(0.7);


    ans::controlPolygon->subscribeListener(bCurve);

    ans::curve = bCurve;

    ans::scene->add(objectHead);
    ans::scene->add(objectGlobalAnchor);
    ans::scene->add(objectCurve);
    ans::scene->add(objectTangent);
    ans::scene->setCamera(camera);
    ans::scene->setLight(light);
    objectHead->add(meshHead);
    objectGlobalAnchor->add(coordinateSystem);
    objectCurve->add(ans::controlPolygon);
    objectCurve->add(bCurve);
    objectTangent->add(ans::tangent);

    ans::activeTransformable = camera;
    ans::scene->accept(iVisitor);

    glClearColor(0.5, 0.5, 0.5, 1); //boja brisanja platna izmedu iscrtavanja dva okvira

    objectHead->doScale(glm::vec3(0.4));

    camera->globalRotate(glm::vec3(3.14, 0, 3.14));
    camera->globalMove(glm::vec3(0, 0, 2));
    light->globalMove(glm::vec3(0, 0, 2));
    ans::controlPolygon->setColor(glm::vec3(1., 0., 0.));
    ans::tangent->setColor(glm::vec3(1., 1., 0.));
    bCurve->setColor(glm::vec3(0., 1., 0.));

    ans::transformables.push_back(camera);
    ans::transformables.push_back(objectHead);

    ans::controlPolygon->addPoints(*controlPoints);
    while (glfwWindowShouldClose(window) == false) {
        FPSManagerObject.enforceFPS(true);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ans::cameraModes[ans::cameraState]();
        ans::activeTransformableModes[ans::activeTransformableState]();
        ans::scene->accept(wfVisitor);

        glfwSwapBuffers(window);
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

    }
    glfwTerminate();
    return EXIT_SUCCESS;
}