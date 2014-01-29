//
//  UIPickerViewDelegate.h
//  Kaboodle
//

#import <UIKit/UIKit.h>

@class UIKPickerView;

/** The delegate of a UIKPickerView object must adopt this protocol and implement at least some of
 its methods to provide the picker view with the data it needs to construct itself.

 The delegate implements the optional methods of this protocol to return height, width, row title, 
 and the view content for the rows in each component. It must also provide the content for each 
 component’s row, either as a string or a view. Typically the delegate implements other optional 
 methods to respond to new selections or deselections of component rows.
 
 TODO:
 
 - implement pickerView:attributedTitleForRow:forComponent:
 */
@protocol UIKPickerViewDelegate<NSObject>
@optional

///-------------------------------------------------------------------------------------------------
/// @name Setting the Dimensions of the Picker View
///-------------------------------------------------------------------------------------------------

/** Called by the picker view when it needs the row height to use for drawing row content.
 
 @param pickerView An object representing the picker view requesting the data.
 @param component A zero-indexed number identifying a component of pickerView. Components are 
 numbered left-to-right.
 @return A float value indicating the height of the row in points.
 */
- (CGFloat)pickerView:(UIKPickerView *)pickerView rowHeightForComponent:(NSInteger)component;

/** Called by the picker view when it needs the row width to use for drawing row content.
 
 @param pickerView The picker view requesting this information.
 @param component A zero-indexed number identifying a component of pickerView. Components are
 numbered left-to-right.
 @return A float value indicating the width of the row in points.
 */
- (CGFloat)pickerView:(UIKPickerView *)pickerView widthForComponent:(NSInteger)component;

///-------------------------------------------------------------------------------------------------
/// @name Setting the Content of Component Rows
///-------------------------------------------------------------------------------------------------

/** Called by the picker view when it needs the title to use for a given row in a given component.
 
 If you implement both this method and the pickerView:attributedTitleForRow:forComponent: method, 
 the picker view prefers the pickerView:attributedTitleForRow:forComponent: method. However, if that
 method returns nil, the picker view falls back to using the string returned by this method.
 
 @param pickerView An object representing the picker view requesting the data.
 @param row A zero-indexed number identifying a row of component. Rows are numbered top-to-bottom.
 @param component A zero-indexed number identifying a component of pickerView. Components are
 numbered left-to-right.
 @return The string to use as the title of the indicated component row.
 */
- (NSString *)pickerView:(UIKPickerView *)pickerView
             titleForRow:(NSInteger)row
            forComponent:(NSInteger)component;

/** Called by the picker view when it needs the title to use for a given row in a given component.
 
 To determine what value the user selected, the delegate uses the row index to access the value at
 the corresponding position in the array used to construct the component.
 
 @param pickerView An object representing the picker view requesting the data.
 @param row A zero-indexed number identifying a row of component. Rows are numbered top-to-bottom.
 @param component A zero-indexed number identifying a component of pickerView. Components are
 numbered left-to-right.
 @return The attributed string to use as the title of the indicated component row.
 */
//- (NSString *)pickerView:(UIKPickerView *)pickerView
//   attributedTitleForRow:(NSInteger)row
//            forComponent:(NSInteger)component;

/** Called by the picker view when it needs the view to use for a given row in a given component.

 If the previously used view (the view parameter) is adequate, return that. If you return a 
 different view, the previously used view is released. The picker view centers the returned view in 
 the rectangle for row.
 
 @param pickerView An object representing the picker view requesting the data.
 @param row A zero-indexed number identifying a row of component. Rows are numbered top-to-bottom.
 @param component A zero-indexed number identifying a component of pickerView. Components are
 numbered left-to-right.
 @param view A view object that was previously used for this row, but is now hidden and cached by 
 picker view.
 @return The string to use as the title of the indicated component row.
 */
- (UIView *)pickerView:(UIKPickerView *)pickerView
            viewForRow:(NSInteger)row
          forComponent:(NSInteger)component
           reusingView:(UIView *)view;

///-------------------------------------------------------------------------------------------------
/// @name Responding to Row Selection
///-------------------------------------------------------------------------------------------------

/** Called by the picker view when the user selects a row in a component.

 To determine what value the user selected, the delegate uses the row index to access the value at 
 the corresponding position in the array used to construct the component.
 
 @param pickerView An object representing the picker view requesting the data.
 @param row A zero-indexed number identifying a row of component. Rows are numbered top-to-bottom.
 @param component A zero-indexed number identifying a component of pickerView. Components are
 numbered left-to-right.
 */
- (void)pickerView:(UIKPickerView *)pickerView
      didSelectRow:(NSInteger)row
       inComponent:(NSInteger)component;

@end