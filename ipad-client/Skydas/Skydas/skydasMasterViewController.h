//
//  skydasMasterViewController.h
//  Skydas
//
//  Created by Moose on 1/16/14.
//  Copyright (c) 2014 Moose. All rights reserved.
//

#import <UIKit/UIKit.h>

@class skydasDetailViewController;

#import <CoreData/CoreData.h>

@interface skydasMasterViewController : UITableViewController <NSFetchedResultsControllerDelegate>

@property (strong, nonatomic) skydasDetailViewController *detailViewController;

@property (strong, nonatomic) NSFetchedResultsController *fetchedResultsController;
@property (strong, nonatomic) NSManagedObjectContext *managedObjectContext;

@end
