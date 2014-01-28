//
//  UIKPickerViewDataSource.h
//  Kaboodle
//

#import <UIKit/UIKit.h>

@class UIKPickerView;

// The UIKPickerViewDataSource protocol must be adopted by an object that
// mediates between a UIKPickerView object and your application’s data model for
// that picker view. The data source provides the picker view with the number of
// components, and the number of rows in each component, for displaying the
// picker view data. Both methods in this protocol are required.
@protocol UIKPickerViewDataSource<NSObject>
@required

// Called by the picker view when it needs the number of components. (required)
- (NSInteger)numberOfComponentsInPickerView:(UIKPickerView *)pickerView;

// Called by the picker view when it needs the number of rows for a specified
// component. (required)
- (NSInteger)pickerView:(UIKPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component;

@end
