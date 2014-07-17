//
//  GLView.h
//  HelloArrow
//
//  Created by Denys Khlivnyy on 7/4/14.
//  Copyright (c) 2014 Denys Khlivnyy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import "IRenderingEngine.hpp"

@interface GLView : UIView {
@private
    // Protected fields go there
    
    // Pointer to the EAGL object that manages our OpenGL context
    // EAGL is a API that links the iPhone operating system with OpenGL
    EAGLContext *m_context;
    IRenderingEngine* m_renderingEngine;
    float m_timestamp;
}

// Public methods go there

- (void) drawView: (CADisplayLink *)displayLink;
- (void) didRotate: (NSNotification *)notification;


@end
