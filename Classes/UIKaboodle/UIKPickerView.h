//
//  UIKPickerView.h
//  Kaboodle
//

#import <UIKit/UIKit.h>
#import "UIKPickerViewDataSource.h"
#import "UIKPickerViewDelegate.h"

/** This is a reimplementation of UIKit's <a href="https://developer.apple.com/library/IOs/documentation/UIKit/Reference/UIPickerView_Class/Reference/UIPickerView.html">UIPickerView</a>. The UIKPickerView class implements objects,
 called picker views, that use a spinning-wheel or slot-machine metaphor to show one or more sets of
 values. Users select values by rotating the wheels so that the desired row of values aligns with a 
 selection indicator.
 
 There is currently no UIKDatePicker subclass.
 
 The user interface provided by a picker view consists of components and rows. A component is a 
 wheel, which has a series of items (rows) at indexed locations on the wheel. Each component also 
 has an indexed location (left to right) in a picker view. Each row on a component has content, 
 which is either a string or a view object such as a label (UILabel) or an image (UIImageView).
 
 Since this is based on <a href="https://developer.apple.com/library/IOs/documentation/UIKit/Reference/UIPickerView_Class/Reference/UIPickerView.html">UIPickerView</a>, refer to its documentation for more information.
 
 TODO:
 
 - fix layout when autoresizing, e.g., orientation change
 - add toggle for vertical and horizontal orientations
 - add toggle for cyclic and non-cyclic modes. Currently, the picker is cyclic and scrolls infinitely.
 - draw the chrome from UIPickerView and add a switch to disable it
 - add iOS6 and iOS7 chrome
 - chrome should be tintable
 - stop on selected row instead of snapping to it
 - default to UIPickerView scrolling behavior and chrome
 */
@interface UIKPickerView : UIView

///-------------------------------------------------------------------------------------------------
/// @name Getting the Dimensions of the View Picker
///-------------------------------------------------------------------------------------------------

/** Gets the number of components for the picker view. (read-only)
 
 A UIPickerView object fetches the value of this property from the data source and caches it. The
 default value is zero.
 */
@property(nonatomic, readonly) NSInteger numberOfComponents;

/** Returns the number of rows for a component.
 
 A picker view fetches the value of this property from the data source and caches it. The default
 value is zero.
 
 @param component A zero-indexed number identifying a component.
 @return The number of rows in the given component.
 */
- (NSInteger)numberOfRowsInComponent:(NSInteger)component;

/** Returns the size of a row for a component.
 
 A picker view fetches the value of this property by calling the pickerView:widthForComponent: and
 pickerView:rowHeightForComponent: delegate methods, and caches it. The default value is (0, 0).
 
 @param component A zero-indexed number identifying a component.
 @return The size of rows in the given component. This is generally the size required to display the 
 largest string or view used as a row in the component.
 
 */
- (CGSize)rowSizeForComponent:(NSInteger)component;

///-------------------------------------------------------------------------------------------------
/// @name Reloading the View Picker
///-------------------------------------------------------------------------------------------------

/** Reloads all components of the picker view.
 
 Calling this method causes the picker view to query the delegate for new data for all components.
 */
- (void)reloadAllComponents;

/** Reloads a particular component of the picker view.
 
 Calling this method causes the picker view to query the delegate for new data for the given
 component.
 
 @param component A zero-indexed number identifying a component.
 */
- (void)reloadComponent:(NSInteger)component;

///-------------------------------------------------------------------------------------------------
/// @name Selecting Rows in the View Picker
///-------------------------------------------------------------------------------------------------

/** Selects a row in a specified component of the picker view.
 
 @param row A zero-indexed number identifying a row of component.
 @param component A zero-indexed number identifying a component.
 @param animated YES to animate the selection by spinning the wheel (component) to the new value; 
 if you specify NO, the new selection is shown immediately.
 */
- (void)selectRow:(NSInteger)row inComponent:(NSInteger)component animated:(BOOL)animated;

/** Returns the index of the selected row in a given component.
 
 @param component A zero-indexed number identifying a component.
 @return A zero-indexed number identifying the selected row, or -1 if no row is selected.
 */
- (NSInteger)selectedRowInComponent:(NSInteger)component;

///-------------------------------------------------------------------------------------------------
/// @name Returning the View for a Row and Component
///-------------------------------------------------------------------------------------------------

/** Returns the view used by the picker view for a given row and component.
 
 The view provided by the delegate in the pickerView:viewForRow:forComponent:reusingView: method.
 Returns nil if the specified row of the component is not visible or if the delegate does not
 implement pickerView:viewForRow:forComponent:reusingView:.
 
 @param row A zero-indexed number identifying a row of component.
 @param component A zero-indexed number identifying a component.
 */
- (UIView *)viewForRow:(NSInteger)row forComponent:(NSInteger)component;

///-------------------------------------------------------------------------------------------------
/// @name Specifying the Delegate
///-------------------------------------------------------------------------------------------------

/** The delegate for the picker view.
 
 The delegate must adopt the UIKPickerViewDelegate protocol and implement the required methods to
 return the drawing rectangle for rows in each component. It also provides the content for each
 component’s row, either as a string or a view, and it typically responds to new selections or 
 deselections.
 */
@property (nonatomic, assign) IBOutlet id<UIKPickerViewDelegate> delegate;

///-------------------------------------------------------------------------------------------------
/// @name Specifying the Data Source
///-------------------------------------------------------------------------------------------------

/** The data source for the picker view.
 
 The data source must adopt the UIKPickerViewDataSource protocol and implement the required methods
 to return the number of components and the number of rows in each component.
 */
@property (nonatomic, assign) IBOutlet id<UIKPickerViewDataSource> dataSource;

///-------------------------------------------------------------------------------------------------
/// @name Managing the Appearance of the Picker View
///-------------------------------------------------------------------------------------------------

/** A Boolean value that determines whether the selection indicator is displayed.
 
 If the value of the property is YES, the picker view shows a clear overlay across the current row.
 The default value of this property is NO.
 */
@property(nonatomic) BOOL showsSelectionIndicator;

@end