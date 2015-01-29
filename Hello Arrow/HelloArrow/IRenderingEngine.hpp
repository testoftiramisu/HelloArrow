//
//  IRenderingEngine.hpp
//  HelloArrow
//
//  Created by Denys Khlivnyy on 7/10/14.
//  Copyright (c) 2014 Denys Khlivnyy. All rights reserved.
//

#ifndef HelloArrow_IRenderingEngine_hpp
#define HelloArrow_IRenderingEngine_hpp

// Phisical orientation of a handled device, equivalent to UIDeviceOrientation
enum DeviceOrientation {
    DeviceOrientationUnknown,
    DeviceOrientationPortrait,
    DeviceOrientationPortraitUpsideDown,
    DeviceOrientationLandscapeLeft,
    DeviceOrientationLandscapeRight,
    DeviceOrientationFaceUp,
    DeviceOrientationFaceDown,
};

// Creates an instance of the renderer and sets up various OpenGL state.
struct IRenderingEngine *CreateRenderer1();
struct IRenderingEngine *CreateRenderer2();

// Interface to the OpenGL ES renderer; consumed by GLView.
struct IRenderingEngine {
    virtual void Initialize(int width, int height) = 0;
    virtual void Render() const = 0;
    virtual void UpdateAnimation(float timeStep) = 0;
    virtual void OnRotate(DeviceOrientation newOrientation) = 0; virtual ~IRenderingEngine() {}
};


#endif
