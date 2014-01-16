//
//  skydasAppDelegate.h
//  Skydas
//
//  Created by Moose on 1/16/14.
//  Copyright (c) 2014 Moose. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface skydasAppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;

@property (readonly, strong, nonatomic) NSManagedObjectContext *managedObjectContext;
@property (readonly, strong, nonatomic) NSManagedObjectModel *managedObjectModel;
@property (readonly, strong, nonatomic) NSPersistentStoreCoordinator *persistentStoreCoordinator;

- (void)saveContext;
- (NSURL *)applicationDocumentsDirectory;

@end
