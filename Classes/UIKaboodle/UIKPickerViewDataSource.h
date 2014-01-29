//
//  UIKPickerViewDataSource.h
//  Kaboodle
//

#import <UIKit/UIKit.h>

@class UIKPickerView;

/** The UIKPickerViewDataSource protocol must be adopted by an object that mediates between a 
 UIKPickerView object and your application’s data model for that picker view. The data source 
 provides the picker view with the number of components, and the number of rows in each component, 
 for displaying the picker view data. Both methods in this protocol are required.
 */
@protocol UIKPickerViewDataSource<NSObject>
@required

///-------------------------------------------------------------------------------------------------
/// @name Providing Counts for the Picker View
///-------------------------------------------------------------------------------------------------

/** Called by the picker view when it needs the number of components. (required)
 
 @param pickerView The picker view requesting the data.
 @return The number of components (or “columns”) that the picker view should display.
 */
- (NSInteger)numberOfComponentsInPickerView:(UIKPickerView *)pickerView;

/** Called by the picker view when it needs the number of rows for a specified component. (required)
 
 @param pickerView The picker view requesting the data.
 @param component A zero-indexed number identifying a component of pickerView. Components are 
 numbered left-to-right.
 @return The number of rows for the component.
 */
- (NSInteger)pickerView:(UIKPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component;

@end
