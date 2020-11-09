#include "ECS/Admin.hpp"
#include "Components/Transform.hpp"
#include "Components/Physics.hpp"
#include "Systems/Physics.hpp"
#include "Extension/Language/Keywords.hpp"

#include <chrono>
#include <thread>

ECS::Admin  admin{};
ECS::Entity entity{};

class Colored {
public:
  Vector3f color{1.0f, 1.0f, 0.0f};
};

/*
 * OGL01Shape3D.cpp: 3D Shapes
 */

#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include "Physics/Shapes/AABB.hpp"
#include "Physics/Shapes/OBB.hpp"
#include "Physics/Shapes/Sphere.hpp"
#include <math.h> /* sin */
#include <iostream>
/* Global variables */
char  title[]   = "3D Shapes";
float angleCube = 0.0f;  // Rotational angle for cube [NEW]
float alpha     = 0.0f;  // Rotational angle for cube [NEW]
float gamma2    = 0.0f;  // Rotational angle for cube [NEW]
float beta      = 0.0f;  // Rotational angle for cube [NEW]
float x1        = 3.0f;
float yy1       = 0.0f;
float z1        = 0.0f;
float x2        = 5.0f;
float y2        = 2.0f;
float z2        = 0.0f;
float x3        = 5.0f;
float y3        = -2.0f;
float z3        = 0.0f;
// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f;
float z = 12.0f;

float deltaAngle = 0.0f;
float deltaMove  = 0;
int   xOrigin    = -1;

/* Initialize OpenGL Graphics */
void initGL() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);               // Set background color to black and opaque
  glClearDepth(1.0f);                                 // Set background depth to farthest
  glEnable(GL_DEPTH_TEST);                            // Enable depth testing for z-culling
  glDepthFunc(GL_LEQUAL);                             // Set the type of depth-test
  glShadeModel(GL_SMOOTH);                            // Enable smooth shading
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

void processSpecialKeys(int key, int xx, int yy) {
  std::cout << angle << std::endl;
  float fraction = 0.1f;
  switch (key) {
    case GLUT_KEY_LEFT:
      angle -= 0.01f;
      lx = sin(angle);
      lz = -cos(angle);
      break;
    case GLUT_KEY_RIGHT:
      angle += 0.01f;
      lx = sin(angle);
      lz = -cos(angle);
      break;
    case GLUT_KEY_UP:
      x += lx * fraction;
      z += lz * fraction;
      break;
    case GLUT_KEY_DOWN:
      x -= lx * fraction;
      z -= lz * fraction;
      break;
  }
}

/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */

void drawAABB(std::vector<Vector3f> points, float red, float green, float blue) {
  glBegin(GL_QUADS);  // Begin drawing the color cube with 6 quads
  // Top face (y = 1.0f)
  // Define vertices in counter-clockwise (CCW) order with normal pointing out
  glColor3f(red, green, blue);  // Green
  glVertex3f(points[2].x, points[2].y, points[2].z);
  glVertex3f(points[3].x, points[3].y, points[3].z);
  glVertex3f(points[4].x, points[4].y, points[4].z);
  glVertex3f(points[7].x, points[7].y, points[7].z);

  // Bottom face  (y = -1.0f)
  // glColor3f(1.0f, 0.5f, 0.0f);  // Orange
  glColor3f(red, green, blue);  // Green
  glVertex3f(points[6].x, points[6].y, points[6].z);
  glVertex3f(points[5].x, points[5].y, points[5].z);
  glVertex3f(points[0].x, points[0].y, points[0].z);
  glVertex3f(points[1].x, points[1].y, points[1].z);

  // Front face  (z = 1.0f)
  // glColor3f(1.0f, 0.0f, 0.0f);  // Red
  glColor3f(red, green, blue);  // Green
  glVertex3f(points[7].x, points[7].y, points[7].z);
  glVertex3f(points[4].x, points[4].y, points[4].z);
  glVertex3f(points[5].x, points[5].y, points[5].z);
  glVertex3f(points[6].x, points[6].y, points[6].z);

  // Back face (z = -1.0f)
  // glColor3f(1.0f, 1.0f, 0.0f);  // Yellow
  glColor3f(red, green, blue);  // Green
  glVertex3f(points[1].x, points[1].y, points[1].z);
  glVertex3f(points[0].x, points[0].y, points[0].z);
  glVertex3f(points[3].x, points[3].y, points[3].z);
  glVertex3f(points[2].x, points[2].y, points[2].z);

  // Left face (x = -1.0f)
  // glColor3f(0.0f, 0.0f, 1.0f);  // Blue
  glColor3f(red, green, blue);  // Green
  glVertex3f(points[4].x, points[4].y, points[4].z);
  glVertex3f(points[3].x, points[3].y, points[3].z);
  glVertex3f(points[0].x, points[0].y, points[0].z);
  glVertex3f(points[5].x, points[5].y, points[5].z);

  // Right face (x = 1.0f)
  // glColor3f(1.0f, 0.0f, 1.0f);  // Magenta
  glColor3f(red, green, blue);  // Green
  glVertex3f(points[2].x, points[2].y, points[2].z);
  glVertex3f(points[7].x, points[7].y, points[7].z);
  glVertex3f(points[6].x, points[6].y, points[6].z);
  glVertex3f(points[1].x, points[1].y, points[1].z);
  glEnd();  // End of drawing color-cube
  // dram my cube
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
  // Compute aspect ratio of the new window
  if (height == 0)
    height = 1;  // To prevent divide by 0
  float aspect = (float)width / (float)height;

  // Set the viewport to cover the new window
  glViewport(0, 0, width, height);

  // Set the aspect ratio of the clipping volume to match the viewport
  glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
  glLoadIdentity();             // Reset
  // Enable perspective projection with fovy, aspect, zNear and zFar
  gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void instructions() {
  std::cout << std::endl << std::endl << std::endl << "  press 'i' at anytime for instructions" << std::endl;
  std::cout << "  press 'esc' at anytime to EXIT" << std::endl << std::endl;
  std::cout << "  press '5' = ROTATE X" << std::endl;
  std::cout << "  press '6' = ROTATE Y" << std::endl;
  std::cout << "  press '7' = ROTATE Z" << std::endl;
  std::cout << "  press '8' = ROTATE RESET" << std::endl;
  std::cout << "  press 'a' = MOVE FRONT" << std::endl;
  std::cout << "  press 'z' = MOVE BACK" << std::endl;
}

void processNormalKeys(unsigned char key, int x, int y) {
  if (key == 27)
    exit(0);
  else if (key == '1') {
    x1 -= 0.1;
    x2 -= 0.1;
  } else if (key == '2') {
    x1 += 0.1;
    x2 += 0.1;
  } else if (key == '3') {
    yy1 -= 0.1;
    y2 -= 0.1;
  } else if (key == '4') {
    yy1 += 0.1;
    y2 += 0.1;
  } else if (key == '5') {
    alpha += 0.25;
  } else if (key == '6') {
    beta += 0.25;
  } else if (key == '7') {
    gamma2 += 0.25;
  } else if (key == '8') {
    alpha  = 0.0f;
    beta   = 0.0f;
    gamma2 = 0.0f;
  } else if (key == 'a') {
    z1 -= 0.1;
    z2 -= 0.1;
  } else if (key == 'z') {
    z1 += 0.1;
    z2 += 0.1;
  }
}

void mouseMove(int x, int y) {
  // this will only be true when the left button is down
  if (xOrigin >= 0) {
    // update deltaAngle
    deltaAngle = (x - xOrigin) * 0.001f;

    // update camera's direction
    lx = sin(angle + deltaAngle);
    lz = -cos(angle + deltaAngle);
  }
}

void mouseButton(int button, int state, int x, int y) {
  // only start motion if the left button is pressed
  if (button == GLUT_LEFT_BUTTON) {
    // when the button is released
    if (state == GLUT_UP) {
      angle += deltaAngle;
      xOrigin = -1;
    } else {  // state = GLUT_DOWN
      xOrigin = x;
    }
  }
}

void display() {
  static auto previous{std::chrono::high_resolution_clock::now()};
  auto        delta{std::chrono::high_resolution_clock::now() - previous};
  previous = std::chrono::high_resolution_clock::now();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear color and depth buffers
  glMatrixMode(GL_MODELVIEW);                          // To operate on model-view matrix

  // Render a color-cube consisting of 6 quads with different colors
  glLoadIdentity();  // Reset the model-view matrix
  gluLookAt(x, 1.0f, z, x + lx, 1.0f, z + lz, 0.0f, 1.0f, 0.0f);
  glRotatef(alpha, 1, 0, 0);   // rotate alpha around the x axis
  glRotatef(beta, 0, 1, 0);    // rotate beta around the y axis
  glRotatef(gamma2, 0, 0, 1);  // rotate gamma around the z axis

  float dt{std::chrono::duration_cast<std::chrono::nanoseconds>(delta).count() / 1000000000.0f};
  std::cout << dt << " ms\t|\t" << 1.0f / dt << " FPS" << std::string(16, ' ') << '\r';

  admin.cacheSystems();
  admin.updateSystems(dt, 0);

  for (auto &&[entity, transform, physics] : admin.getEntitiesWithComponents<Components::Transform, Components::Physics>()) {
    Vector3f color{1.0f, 0.0f, 0.0f};
    if (admin.hasComponent<Colored>(entity)) {
      auto &colored{admin.getComponent<Colored>(entity)};
      color = colored.color;
    }

    auto &shape{physics.m_shape};
    switch (shape->m_shapeType) {
      case ShapeType::AABB:
        using (AABB &aabb{reinterpret_cast<AABB &>(*shape.get())}) {
          drawAABB(aabb.getPoints(transform.matrix), color.x, color.y, color.z);
        }
        break;
      case ShapeType::SPHERE:
        using (Sphere &sphere{reinterpret_cast<Sphere &>(*shape.get())}) {
          glPushMatrix();
          glTranslatef(sphere.getPoints(transform.matrix).x, sphere.getPoints(transform.matrix).y, sphere.getPoints(transform.matrix).z);  // Move right and into the screen
          glColor3f(color.x, color.y, color.z);
          GLUquadric *quadric = gluNewQuadric();
          gluQuadricTexture(quadric, true);
          gluSphere(quadric, sphere.getRadius(), 20, 20);
          glPopMatrix();
        }
        break;
    }
  }

  glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
  glutPostRedisplay();
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char **argv) {
  using (auto entity{admin.createEntity()}) {
    auto &translate{admin.createComponent<Components::Transform>(entity)};
    translate.matrix.setTranslation(Vector3f{3.0f, 0.0f, 0.0f});

    auto &physics{admin.createComponent<Components::Physics>(entity, std::make_unique<AABB>(Vector3f(-1.0f, -1.0f, -6.0f), Vector3f(1.0f, 1.0f, -4.0f)))};
    physics.applyLinearImpulse(Vector3f{0.0f, -5.0f, 0.0f});

    auto &colored{admin.createComponent<Colored>(entity)};
    colored.color = Vector3f{0.25f, 0.5f, 1.0f};
  }

  using (auto entity{admin.createEntity()}) {
    auto &translate{admin.createComponent<Components::Transform>(entity)};
    translate.matrix.setTranslation(Vector3f{-3.0f, 0.0f, 0.0f});

    auto &physics{admin.createComponent<Components::Physics>(entity, std::make_unique<AABB>(Vector3f(-1.0f, -1.0f, -6.0f), Vector3f(1.0f, 1.0f, -4.0f)))};
    physics.applyLinearImpulse(Vector3f{30.0f, 0.0f, 0.0f});
  }

  using (auto entity{admin.createEntity()}) {
    auto &translate{admin.createComponent<Components::Transform>(entity)};
    translate.matrix.setTranslation(Vector3f{3.0f, 5.0f, 0.0f});

    auto &physics{admin.createComponent<Components::Physics>(entity, std::make_unique<Sphere>(Vector3f(-1.0f, -1.0f, -6.0f), 1.0f))};
    physics.applyLinearImpulse(Vector3f{-15.0f, -15.0f, 0.0f});

    auto &colored{admin.createComponent<Colored>(entity)};
    colored.color = Vector3f{0.5f, 1.0f, 0.25f};
  }

  admin.createSystem<Systems::Physics>();

  glutInit(&argc, argv);             // Initialize GLUT
  glutInitDisplayMode(GLUT_DOUBLE);  // Enable double buffered mode
  glutInitWindowSize(1600, 900);     // Set the window's initial width & height
  glutInitWindowPosition(50, 50);    // Position the window's initial top-left corner
  glutCreateWindow(title);           // Create window with the given title
  glutDisplayFunc(display);          // Register callback handler for window re-paint event
  glutReshapeFunc(reshape);          // Register callback handler for window re-size event
  glutIdleFunc(display);
  glutKeyboardFunc(processNormalKeys);
  glutSpecialFunc(processSpecialKeys);
  // here are the two new functions
  glutMouseFunc(mouseButton);
  glutMotionFunc(mouseMove);

  instructions();
  // glutTimerFunc(0, timer, 0);     // First timer call immediately [NEW]
  initGL();        // Our own OpenGL initialization
  glutMainLoop();  // Enter the infinite event-processing loop
  return 0;
}
