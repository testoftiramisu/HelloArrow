//
//  RenderingEngine1.cpp
//  HelloArrow
//
//  Created by Denys Khlivnyy on 7/14/14.
//  Copyright (c) 2014 Denys Khlivnyy. All rights reserved.
//

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include "IRenderingEngine.hpp"

static const float ResolutionPerSecond = 1;

class RenderingEngine1: public IRenderingEngine {
public:
    RenderingEngine1();
    void Initialize(int width, int height);
    void Render() const;
    void UpdateAnimation(float timeStep);
    void OnRotate(DeviceOrientation newOrietation);
private:
    float RotationDirection() const;
    float m_desiredAngle;
    float m_currentAngle;
    GLuint m_framebuffer;
    GLuint m_renderbuffer;
};

IRenderingEngine* CreateRenderer1()
{
    return new RenderingEngine1();
}

struct Vertex {
    float Position[2];
    float Color[4];
};

// Define the positions and colors of two triangles
// Vericles[XYRGBA]
const Vertex Vertices[] = {
    {{-0.5, -0.866}, {1, 1, 0.5f, 1}},
    {{0.5, -0.866},  {1, 1, 0.5f, 1}},
    {{0, 1},         {1, 1, 0.5f, 1}},
    {{-0.5, -0.866}, {0.5f, 0.5f, 0.5f}},
    {{0.5, -0.866},  {0.5f, 0.5f, 0.5f}},
    {{0, -0.4f},     {0.5f, 0.5f, 0.5f}},
 
};

RenderingEngine1::RenderingEngine1()
{
    glGenRenderbuffersOES(1, &m_renderbuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_renderbuffer);
}

/*
 renderbuffer is a 2D surface filled with some type of data (in this case, color).
 framebuffer is a bundle of renderbuffers
 */
void RenderingEngine1::Initialize(int width, int height)
{
    // Create the framebuffer object and attach the color buffer.
    glGenFramebuffersOES(1, &m_framebuffer);
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_framebuffer);
    
    // Attaching the renderbuffer object to the framebuffer object.
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES,
                                 GL_COLOR_ATTACHMENT0_OES,
                                 GL_RENDERBUFFER_OES,
                                 m_renderbuffer);
    
    // Setting up a coordinate system
    glViewport(0, 0, width, height);
    
    // Initialize the projection matrix
    glMatrixMode(GL_PROJECTION);
    const float maxX = 2;
    const float maxY = 3;
    glOrthof(-maxX, +maxX, -maxY, +maxY, -1, 1);
    
    // Set up the vertex transformation state
    glMatrixMode(GL_MODELVIEW);
    
    // Initialize the rotation animation state
    OnRotate(DeviceOrientationPortrait);
    m_currentAngle = m_desiredAngle;
    
}

void RenderingEngine1::Render() const
{
    // Using OpenGL’s “clear” mechanism to fill the buffer with a solid color
    // Color is set to gray using four values (red, green, blue, alpha)
    glClearColor(0.5f, 0.5f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glPushMatrix();
    glRotatef(m_currentAngle, 0, 0, 1);
    
    // Enable two vertex attributes (position and color)
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    
    // Tell OpenGL how to fetch the data for the position and color attributes.
    glVertexPointer(2, GL_FLOAT, sizeof(Vertex), &Vertices[0].Position[0]);
    glColorPointer(4, GL_FLOAT, sizeof(Vertex), &Vertices[0].Color[0]);
    
    GLsizei vertexCount = sizeof(Vertices) / sizeof(Vertex);
    
    // Execute the draw command with glDrawArrays:
    // GL_TRIANGLES for the topology
    // 0 for the starting vertex
    // vertexCount for the number of vertices
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    
    // Disable the two vertex attributes;
    // they need to be enabled only during the preceding draw command.
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    
    glPopMatrix();
}

void RenderingEngine1::OnRotate(DeviceOrientation orientation)
{
    float angle = 0;
    
    switch (orientation) {
        case DeviceOrientationLandscapeLeft:
            angle = 270;
            break;
            
        case DeviceOrientationPortraitUpsideDown:
            angle = 180;
            break;
        case DeviceOrientationLandscapeRight:
            angle = 90;
            break;
    }
    m_desiredAngle = angle;
}

float RenderingEngine1::RotationDirection() const
{
    float delta = m_desiredAngle - m_currentAngle;
    if (delta == 0) {
        return 0;
    }
    
    bool counterclockwise = ((delta > 0 && delta <= 180) || (delta < -180));
    return counterclockwise ? +1 : -1;
}

void RenderingEngine1::UpdateAnimation(float timeStep)
{
    float direction = RotationDirection();
    if (direction == 0) {
        return;
    }
    
    float degrees = timeStep * 360 * ResolutionPerSecond;
    m_currentAngle += degrees * direction;
    
    // ensure that the angle stays within [0, 360]
    if (m_currentAngle >= 360) {
        m_currentAngle -= 360;
    }
    else if (m_currentAngle < 0) {
        m_currentAngle += 360;
    }
    
    // If the rotation direction changed, then we overshot the desired angle
    if (RotationDirection() != direction) {
        m_currentAngle = m_desiredAngle;
    }
}













