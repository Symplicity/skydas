//
//  skydasDetailViewController.h
//  Skydas
//
//  Created by Moose on 1/16/14.
//  Copyright (c) 2014 Moose. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface skydasDetailViewController : UIViewController <UISplitViewControllerDelegate>

@property (strong, nonatomic) id detailItem;

@property (weak, nonatomic) IBOutlet UILabel *detailDescriptionLabel;
@end
