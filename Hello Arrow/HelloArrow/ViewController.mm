//
//  ViewController.m
//  HelloArrow
//
//  Created by Denys Khlivnyy on 5/19/16.
//  Copyright © 2016 Denys Khlivnyy. All rights reserved.
//

#import "ViewController.h"
#import "GLView.h"
@interface ViewController ()
@property (nonatomic,strong)GLView *glView;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    _glView = [[GLView alloc]initWithFrame:[[UIScreen mainScreen]bounds]];
    self.view = _glView;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end