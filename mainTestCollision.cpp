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
  ml::vec3 color{1.0f, 1.0f, 0.0f};
};

/*
 * OGL01Shape3D.cpp: 3D Shapes
 */

#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include "Physics/Shapes/AABB.hpp"
#include "Physics/Shapes/OBB.hpp"
#include "Physics/Shapes/Sphere.hpp"
#include "Physics/Shapes/Capsule.hpp"
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
float xSouris = 0.0f;
float ySouris = 0.0f;
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

glm::vec3 GetAnyPerpendicularUnitVector(const glm::vec3& vec)
{
  if (vec.y != 0.0f || vec.z != 0.0f)
    return glm::vec3(1, 0, 0);
  else
    return glm::vec3(0, 1, 0);
}

void drawTriangle(const glm::vec3 p0, const glm::vec3 p1, const glm::vec3 p2, glm::vec3 color)
{
  float vertices[] =
    {
        p0.x, p0.y, p0.z, // top corner
        p1.x, p1.y, p1.z, // bottom left corner
        p2.x, p2.y, p2.z // bottom right corner
    };
  glColor3f(color.r, color.g, color.b);  // Green
  glEnableClientState( GL_VERTEX_ARRAY ); // tell OpenGL that you're using a vertex array for fixed-function attribute
  glVertexPointer( 3, GL_FLOAT, 0, vertices ); // point to the vertices to be used
  glDrawArrays( GL_TRIANGLES, 0, 3 ); // draw the vertixes
  glDisableClientState( GL_VERTEX_ARRAY ); // tell OpenGL that you're finished using the vertex arrayattribute
}

void drawCapsule(glm::vec3 start, glm::vec3 end, float radius, glm::vec3 color)
{
  const glm::vec3 axis   = end - start;
  const float     length = glm::length(axis);
  const glm::vec3 localZ = axis / length;
  const glm::vec3 localX = GetAnyPerpendicularUnitVector(localZ);
  const glm::vec3 localY = glm::cross(localZ, localX);

  using glm::cos;
  using glm::sin;
  constexpr float pi = glm::pi<float>();

  const glm::vec3 startP(0.0f);
  const glm::vec3 endP(1.0f);
  const float     resolution = 16.0f;

  const glm::vec3 step = (endP - startP) / resolution;

  auto cylinder = [localX, localY, localZ, start, length, radius](const float u,
                                                      const float v) {
    return start                                  //
           + localX * cos(2.0f * pi * u) * radius //
           + localY * sin(2.0f * pi * u) * radius //
           + localZ * v * length;                   //

  };

  auto sphereStart = [localX, localY, localZ, start, radius](const float u,
                                                 const float v) -> glm::vec3 {
    const float latitude = (pi / 2.0f) * (v - 1);

    return start                                                  //
           + localX * cos(2.0f * pi * u) * cos(latitude) * radius //
           + localY * sin(2.0f * pi * u) * cos(latitude) * radius //
           + localZ * sin(latitude) * radius;
  };

  auto sphereEnd = [localX, localY, localZ, end, radius](const float u, const float v) {
    const float latitude = (pi / 2.0f) * v;
    return end                                                    //
           + localX * cos(2.0f * pi * u) * cos(latitude) * radius //
           + localY * sin(2.0f * pi * u) * cos(latitude) * radius //
           + localZ * sin(latitude) * radius;
  };

  for (float i = 0; i < resolution; ++i) {
    for (float j = 0; j < resolution; ++j) {
      const float u = i * step.x + startP.x;
      const float v = j * step.y + startP.y;

      const float un =
        (i + 1 == resolution) ? endP.x : (i + 1) * step.x + startP.x;
      const float vn =
        (j + 1 == resolution) ? endP.y : (j + 1) * step.y + startP.y;

    // Draw Cylinder
      {
        const glm::vec3 p0 = cylinder(u, v);
        const glm::vec3 p1 = cylinder(u, vn);
        const glm::vec3 p2 = cylinder(un, v);
        const glm::vec3 p3 = cylinder(un, vn);

        drawTriangle(p0, p1, p2, color);
        drawTriangle(p3, p1, p2, color);
      }

    // Draw Sphere start
      {
        const glm::vec3 p0       = sphereStart(u, v);
        const glm::vec3 p1       = sphereStart(u, vn);
        const glm::vec3 p2       = sphereStart(un, v);
        const glm::vec3 p3       = sphereStart(un, vn);
        drawTriangle(p0, p1, p2, color);
        drawTriangle(p3, p1, p2, color);
      }

    // Draw Sphere end
      {
        const glm::vec3 p0       = sphereEnd(u, v);
        const glm::vec3 p1       = sphereEnd(u, vn);
        const glm::vec3 p2       = sphereEnd(un, v);
        const glm::vec3 p3       = sphereEnd(un, vn);
        drawTriangle(p0, p1, p2, color);
        drawTriangle(p3, p1, p2, color);
      }
    }
  }
}

/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */

void drawAABB(std::vector<ml::vec3> points, float red, float green, float blue) {
  glBegin(GL_QUADS);  // Begin drawing the color cube with 6 quads
  // Top face (y = 1.0f)
  // Define vertices in counter-clockwise (CCW) order with normal pointing out
  glColor3f(1.0f, 0.0f, 0.0f);  // Red
  //glColor3f(red, green, blue);  // Green
  glVertex3f(points[2].x, points[2].y, points[2].z);
  glVertex3f(points[3].x, points[3].y, points[3].z);
  glVertex3f(points[4].x, points[4].y, points[4].z);
  glVertex3f(points[7].x, points[7].y, points[7].z);

  // Bottom face  (y = -1.0f)
  glColor3f(1.0f, 0.5f, 0.0f);  // Orange
  //glColor3f(red, green, blue);  // Green
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
  glColor3f(1.0f, 1.0f, 0.0f);  // Blue
  //glColor3f(red, green, blue);  // Green
  glVertex3f(points[4].x, points[4].y, points[4].z);
  glVertex3f(points[3].x, points[3].y, points[3].z);
  glVertex3f(points[0].x, points[0].y, points[0].z);
  glVertex3f(points[5].x, points[5].y, points[5].z);

  // Right face (x = 1.0f)
  glColor3f(1.0f, 0.0f, 1.0f);  // Magenta
  //glColor3f(red, green, blue);  // Green
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
  if (xSouris == 0) {
    xSouris = x;
    ySouris = y;
    return;
  }
  float saveX = x;
  float saveY = y;
  x = x - xSouris;
  y = y - ySouris;
  xSouris = saveX;
  ySouris = saveY;
  std::cout << x << " | " << y << std::endl;
    for (auto &&[entity, transform, physics] : admin.getEntitiesWithComponents<Components::Transform, Components::Physics>()) {
       auto &shape{physics.m_shape};
    switch (shape->m_shapeType) {
      case ShapeType::OBB:
        physics.applyAngularImpulse(ml::vec3{y / 20.0f, 0.0f, x / 20.0f});
    }
  }
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
/*   if (button == GLUT_LEFT_BUTTON) {
    // when the button is released
    if (state == GLUT_UP) {
      angle += deltaAngle;
      xOrigin = -1;
    } else {  // state = GLUT_DOWN
      xOrigin = x;
    }
  } */
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
  //std::cout << dt << " ms\t|\t" << 1.0f / dt << " FPS" << std::string(16, ' ') << '\r';

  admin.cacheSystems();
  admin.updateSystems(dt, 0);

  Ray ray(ml::vec3(-20.0f, 0.0f, -3.0f), ml::vec3(1.0f, 0.0f, 0.0f));
  RayCollision rayCollision;
  
  /* if (admin.getSystem<Systems::Physics>().RayIntersection(ray, rayCollision)) {
    std::cout << rayCollision.node << std::endl;
    auto &colored{admin.getComponent<Colored>(rayCollision.node)};
    colored.color = ml::vec3{1.0f, 1.0f, 0.0f};
    std::cout << "collide" << std::endl; 
  }

  glBegin(GL_LINES);
  glColor3f(1.0f, 1.0f, 0.0f);
  glVertex3f(-20.0f, 0.0f, -3.0f);
  glVertex3f(-20.0f + 1.0f * 100.0f, 0.0f, -3.0f);
  glEnd(); */
  for (auto &&[entity, transform, physics] : admin.getEntitiesWithComponents<Components::Transform, Components::Physics>()) {
    ml::vec3 color{1.0f, 0.0f, 0.0f};
    if (admin.hasComponent<Colored>(entity)) {
      auto &colored{admin.getComponent<Colored>(entity)};
      color = colored.color;
    }
    std::cout << entity << std::endl;
    //if (entity == 2) {
    //  physics.applyLinearImpulse(ml::vec3{0.0f, -0.1f, 0.0f});
    //}
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
      case ShapeType::CAPSULE:
        using (Capsule &capsule{reinterpret_cast<Capsule &>(*shape.get())}) {
          auto points = capsule.getPoints(transform.matrix);
          drawCapsule(glm::vec3(points[0].x, points[0].y, points[0].z), glm::vec3(points[1].x, points[1].y, points[1].z), capsule.getRadius(), glm::vec3(color.x, color.y, color.z));
        }
        break;
      case ShapeType::OBB:
        using (OBB &obb{reinterpret_cast<OBB &>(*shape.get())}) {
          drawAABB(obb.getPoints(transform.matrix), color.x, color.y, color.z);
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
    translate.matrix.setTranslation(ml::vec3{-6.0f, 8.0f, 0.0f});

    auto &physics{admin.createComponent<Components::Physics>(entity, std::make_unique<AABB>(ml::vec3(-1.0f, -1.0f, -6.0f), ml::vec3(1.0f, 1.0f, -4.0f)))};
    physics.applyLinearImpulse(ml::vec3{0.0f, 0.0f, 0.0f});

    auto &colored{admin.createComponent<Colored>(entity)};
    colored.color = ml::vec3{0.25f, 0.5f, 1.0f};
  }

  using (auto entity{admin.createEntity()}) {
    auto &translate{admin.createComponent<Components::Transform>(entity)};
    translate.matrix.setTranslation(ml::vec3{-6.0f, 2.0f, 0.0f});

    auto &physics{admin.createComponent<Components::Physics>(entity, std::make_unique<AABB>(ml::vec3(-1.0f, -1.0f, -6.0f), ml::vec3(1.0f, 1.0f, -4.0f)))};
    physics.applyLinearImpulse(ml::vec3{0.0f, 14.0f, 0.0f});
    auto &colored{admin.createComponent<Colored>(entity)};
    colored.color = ml::vec3{1.0f, 0.0f, 0.0f};
  }

 using (auto entity{admin.createEntity()}) {
    auto &translate{admin.createComponent<Components::Transform>(entity)};
    translate.matrix.setTranslation(ml::vec3{-3.0f, 6.0f, 0.0f});

    auto &physics{admin.createComponent<Components::Physics>(entity, std::make_unique<AABB>(ml::vec3(-1.0f, -1.0f, -6.0f), ml::vec3(1.0f, 1.0f, -4.0f)))};
    physics.applyLinearImpulse(ml::vec3{0.0f, -15.0f, 0.0f});

    auto &colored{admin.createComponent<Colored>(entity)};
    colored.color = ml::vec3{0.25f, 0.5f, 1.0f};
  }

  using (auto entity{admin.createEntity()}) {
    auto &translate{admin.createComponent<Components::Transform>(entity)};
    translate.matrix.setTranslation(ml::vec3{-3.0f, 0.0f, 0.0f});

    auto &physics{admin.createComponent<Components::Physics>(entity, std::make_unique<AABB>(ml::vec3(-1.0f, -1.0f, -6.0f), ml::vec3(1.0f, 1.0f, -4.0f)))};
    physics.applyLinearImpulse(ml::vec3{0.0f, 0.0f, 0.0f});
    auto &colored{admin.createComponent<Colored>(entity)};
    colored.color = ml::vec3{1.0f, 0.0f, 0.0f};
    physics.setIsRigid(true);
  }

  using (auto entity{admin.createEntity()}) {
    auto &translate{admin.createComponent<Components::Transform>(entity)};
    translate.matrix.setTranslation(ml::vec3{0.0f, -1.0f, 0.0f});

    auto &physics{admin.createComponent<Components::Physics>(entity, std::make_unique<Sphere>(ml::vec3(-3.0f, -2.0f, -4.0f), 1.50f))};
    physics.applyLinearImpulse(ml::vec3{0.0f, 0.0f, 0.0f});

    auto &colored{admin.createComponent<Colored>(entity)};
    colored.color = ml::vec3{1.0f, 0.0f, 0.0f};
    physics.setIsRigid(true);
  }

  using (auto entity{admin.createEntity()}) {
    auto &translate{admin.createComponent<Components::Transform>(entity)};
    translate.matrix.setTranslation(ml::vec3{-11.0f, -4.0f, 0.0f});

    auto &physics{admin.createComponent<Components::Physics>(entity, std::make_unique<AABB>(ml::vec3(-1.0f, -1.0f, -6.0f), ml::vec3(1.0f, 1.0f, -4.0f)))};
    physics.applyLinearImpulse(ml::vec3{17.0f, 0.0f, 0.0f});
    auto &colored{admin.createComponent<Colored>(entity)};
    colored.color = ml::vec3{1.0f, 0.0f, 0.0f};
  }

  using (auto entity{admin.createEntity()}) {
    auto &translate{admin.createComponent<Components::Transform>(entity)};
    translate.matrix.setTranslation(ml::vec3{0.0f, 0.0f, 0.0f});

    auto &physics{admin.createComponent<Components::Physics>(entity, std::make_unique<Capsule>(ml::vec3(0.0f, 3.0f, -4.0f), ml::vec3(0.0f, -3.0f, -4.0f), 2.0f))};
    physics.applyLinearImpulse(ml::vec3{10.0f, 0.0f, 0.0f});

    auto &colored{admin.createComponent<Colored>(entity)};
    colored.color = ml::vec3{0.5f, 1.0f, 0.25f};
  }

  using (auto entity{admin.createEntity()}) {
    auto &translate{admin.createComponent<Components::Transform>(entity)};
    translate.matrix.setTranslation(ml::vec3{6.50f, 0.0f, 0.0f});

    auto &physics{admin.createComponent<Components::Physics>(entity, std::make_unique<AABB>(ml::vec3(-1.0f, -1.0f, -6.0f), ml::vec3(1.0f, 1.0f, -4.0f)))};
    physics.applyLinearImpulse(ml::vec3{0.0f, 0.0f, 0.0f});
    auto &colored{admin.createComponent<Colored>(entity)};
    colored.color = ml::vec3{1.0f, 0.0f, 0.0f};
  }

  /*using (auto entity{admin.createEntity()}) {
    auto &translate{admin.createComponent<Components::Transform>(entity)};
    translate.matrix.setTranslation(ml::vec3{0.0f, 0.0f, 0.0f});

    auto &physics{admin.createComponent<Components::Physics>(entity, std::make_unique<Sphere>(ml::vec3(0.0f, 5.0f, -4.0f), 4.0f))};
    physics.applyLinearImpulse(ml::vec3{0.0f, 0.0f, 0.0f});

    auto &colored{admin.createComponent<Colored>(entity)};
    colored.color = ml::vec3{0.5f, 1.0f, 0.25f};
    //physics.setIsRigid(true);
  }*/


/*   using (auto entity{admin.createEntity()}) {
    auto &translate{admin.createComponent<Components::Transform>(entity)};
    translate.matrix.setTranslation(ml::vec3{0.0f, 0.0f, 0.0f});

    auto &physics{admin.createComponent<Components::Physics>(entity, std::make_unique<Sphere>(ml::vec3(2.0f, 0.0f, -5.0f), 2.0f))};
    physics.applyLinearImpulse(ml::vec3{0.0f, 0.0f, 0.0f});

    auto &colored{admin.createComponent<Colored>(entity)};
    colored.color = ml::vec3{0.5f, 1.0f, 0.25f};
  }  */





/*   using (auto entity{admin.createEntity()}) {
    auto &translate{admin.createComponent<Components::Transform>(entity)};
    translate.matrix.setTranslation(ml::vec3{0.0f, 0.0f, 0.0f});

    auto &physics{admin.createComponent<Components::Physics>(entity, std::make_unique<AABB>(ml::vec3(1.0f, -1.0f, -4.0f), ml::vec3(3.0f, 1.0f, -2.0f)))};
    physics.applyLinearImpulse(ml::vec3{0.0f, -0.0f, 0.0f});

    auto &colored{admin.createComponent<Colored>(entity)};
    colored.color = ml::vec3{1.0f, 0.0f, 0.0f};
  }  */

 
/*   using (auto entity{admin.createEntity()}) {
    auto &translate{admin.createComponent<Components::Transform>(entity)};
    translate.matrix.setTranslation(ml::vec3{0.0f, 0.0f, 0.0f});

    auto &physics{admin.createComponent<Components::Physics>(entity, std::make_unique<Sphere>(ml::vec3(3.0f, -1.0f, -4.0f), 2.0f))};
    physics.applyLinearImpulse(ml::vec3{-5.0f, 0.0f, 0.0f});

    auto &colored{admin.createComponent<Colored>(entity)};
    colored.color = ml::vec3{0.5f, 1.0f, 0.25f};
  } */

/*   using (auto entity{admin.createEntity()}) {
    auto &translate{admin.createComponent<Components::Transform>(entity)};
    translate.matrix.setTranslation(ml::vec3{-0.0f, 0.0f, 0.0f});

    auto &physics{admin.createComponent<Components::Physics>(entity, std::make_unique<OBB>(ml::vec3(-1.0f, -1.0f, -6.0f), ml::vec3(1.0f, 1.0f, -4.0f)))};
    //physics.applyAngularImpulse(ml::vec3{0.0f, 0.0f, -20.0f});

    auto &colored{admin.createComponent<Colored>(entity)};
    colored.color = ml::vec3{0.25f, 0.5f, 1.0f};
  }
 */

  using (auto entity{admin.createEntity()}) {
    auto &translate{admin.createComponent<Components::Transform>(entity)};
    translate.matrix.setTranslation(ml::vec3{-8.0f, 3.0f, 0.0f});

    auto &physics{admin.createComponent<Components::Physics>(entity, std::make_unique<OBB>(ml::vec3(-1.0f, -1.0f, -6.0f), ml::vec3(1.0f, 1.0f, -4.0f)))};
    physics.applyLinearImpulse(ml::vec3{0.0f, 0.0f, 0.0f});
    physics.applyAngularImpulse(ml::vec3{0.0f, 0.0f, 0.0f});

    auto &colored{admin.createComponent<Colored>(entity)};
    colored.color = ml::vec3{1.0f, 0.0f, 0.0f};
  }

  using (auto entity{admin.createEntity()}) {
    auto &translate{admin.createComponent<Components::Transform>(entity)};
    translate.matrix.setTranslation(ml::vec3{-8.0f, 8.0f, 0.0f});

    auto &physics{admin.createComponent<Components::Physics>(entity, std::make_unique<OBB>(ml::vec3(-1.0f, -1.0f, -6.0f), ml::vec3(1.0f, 1.0f, -4.0f)))};
    physics.applyLinearImpulse(ml::vec3{0.0f, -11.0f, 0.0f});
    physics.applyAngularImpulse(ml::vec3{0.0f, 0.0f, 0.0f});

    auto &colored{admin.createComponent<Colored>(entity)};
    colored.color = ml::vec3{0.0f, 1.0f, 0.0f};
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
