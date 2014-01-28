//
//  UIPickerViewDelegate.h
//  Kaboodle
//

#import <UIKit/UIKit.h>

@class UIKPickerView;

// The delegate of a UIPickerView object must adopt this protocol and implement
// at least some of its methods to provide the picker view with the data it
// needs to construct itself.
//
// The delegate implements the optional methods of this protocol to return
// height, width, row title, and the view content for the rows in each
// component. It must also provide the content for each component’s row, either
// as a string or a view. Typically the delegate implements other optional
// methods to respond to new selections or deselections of component rows.
@protocol UIKPickerViewDelegate<NSObject>
@optional

// Called by the picker view when it needs the row width to use for drawing row
// content.
- (CGFloat)pickerView:(UIKPickerView *)pickerView
    widthForComponent:(NSInteger)component;

// Called by the picker view when it needs the row height to use for drawing row
// content.
- (CGFloat)pickerView:(UIKPickerView *)pickerView rowHeightForComponent:(NSInteger)component;

// Called by the picker view when it needs the title to use for a given row in a
// given component.
- (NSString *)pickerView:(UIKPickerView *)pickerView
             titleForRow:(NSInteger)row
            forComponent:(NSInteger)component;

// Called by the picker view when it needs the view to use for a given row in a
// given component.
//
// If the previously used view (the view parameter) is adequate, return that. If
// you return a different view, the previously used view is released. The picker
// view centers the returned view in the rectangle for row.
- (UIView *)pickerView:(UIKPickerView *)pickerView
            viewForRow:(NSInteger)row
          forComponent:(NSInteger)component
           reusingView:(UIView *)view;

// Called by the picker view when the user selects a row in a component.
//
// To determine what value the user selected, the delegate uses the row index to
// access the value at the corresponding position in the array used to construct
// the component.
- (void)pickerView:(UIKPickerView *)pickerView
      didSelectRow:(NSInteger)row
       inComponent:(NSInteger)component;

@end