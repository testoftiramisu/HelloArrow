//
//  GLView.m
//  HelloArrow
//
//  Created by Denys Khlivnyy on 7/4/14.
//  Copyright (c) 2014 Denys Khlivnyy. All rights reserved.
//

#import "GLView.h"

const bool ForceES1 = false;

@implementation GLView

+ (Class) layerClass
{
    // Overrides the default implementation of layerClass to return an OpenGL-friendly layer type.
    return [CAEAGLLayer class];
}

    /* 
     initWithFrame method calls the factory method to instantiate the C++ renderer.
     It sets up two event handlers:
        drawView: fires every time the screen refreshes.
        didRotate: responds to orientation changes.
     */
- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Retrieve the layer property from the base class (UIView)
        // and downcast it from CALayer into a CAEAGLLayer
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *) super.layer;
        
        // Set the opaque layer property to indicate that we dont need Quartz to handle transparency
        eaglLayer.opaque = YES;
        
        // Create an EAGLContext object, and indicate version of OpenGL we need: ES 1.1
        m_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
        
        // Tell the EAGLContext to make itself current
        // Change to kEAGLRenderingAPIOpenGLES1 for start with ES 1.1
        EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
        m_context = [[EAGLContext alloc] initWithAPI:api];
        
        if (!m_context || ForceES1) {
            api = kEAGLRenderingAPIOpenGLES1;
            m_context = [[EAGLContext alloc] initWithAPI:api];
        }
        
        if (!m_context || ![EAGLContext setCurrentContext:m_context]) {
            return nil;
        }
        
        // Allow the application to work on older devices
        // while leveraging ES 2.0 on newer devices.
        if (api == kEAGLRenderingAPIOpenGLES1) {
            NSLog(@"Using OpenGL ES 1.1");
            m_renderingEngine = CreateRenderer1();
        } else {
            NSLog(@"Using OpenGL ES 2.0");
            m_renderingEngine = CreateRenderer2();
        }
        
        //  GLView is responsible for allocating the renderbuffer’s storage.
        //  Allocation of the renderbuffer isn’t done with the rendering engine because it requires Objective-C.
        [m_context renderbufferStorage:GL_RENDERBUFFER
                          fromDrawable: eaglLayer];
        
        m_renderingEngine -> Initialize(CGRectGetWidth(frame), CGRectGetHeight(frame));
        
        [self drawView: nil];
        m_timestamp = CACurrentMediaTime();
        
        CADisplayLink *displayLink;
        displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawView:)];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        
        [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(didRotate:)
                                                     name:UIDeviceOrientationDidChangeNotification
                                                   object:nil];
    }
    return self;
}

    /* 
     didRotate event handler casts the iPhone-specific UIDeviceOrientation 
     portable DeviceOrientation type and then passes it on to the rendering engine.
     */
- (void)didRotate:(NSNotification *)notification
{
    UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation];
    m_renderingEngine -> OnRotate((DeviceOrientation) orientation);
    [self drawView: nil];
}

#pragma mark - draw
    /*
     The drawView method, called in response to a displayLink event:
     computes the elapsed time since it was last called 
     and passes that value into the renderer’s UpdateAnimation method. 
     This allows the renderer to update any animations or physics that it might be controlling.
     drawView method issues the Render command and presents the render buffer to the screen.
     */
- (void)drawView:(CADisplayLink *)displayLink
{    
    if (displayLink != nil) {
        float elapsedSeconds = displayLink.timestamp - m_timestamp;
        m_timestamp = displayLink.timestamp;
        m_renderingEngine -> UpdateAnimation(elapsedSeconds);
    }
    
    m_renderingEngine -> Render();
    [m_context presentRenderbuffer:GL_RENDERBUFFER];
}

@end
