//
//  UIKPickerView.h
//  UIKPickerView
//

#import <UIKit/UIKit.h>
#import "UIKPickerViewDataSource.h"
#import "UIKPickerViewDelegate.h"

#pragma mark -
#pragma mark UIKPickerView

// This is a reimplementation of UIKit's UIPickerView. The current version does
// not have any chrome. Also the picker is cyclic and scrolls infinitely.
//
// TODO:
// - fix layout when autoresizing, e.g., orientation change
// - add toggle for vertical and horizontal orientations
// - add toggle for cyclic and non-cyclic modes.
// - draw the chrome from UIPickerView and add a switch to disable it
// - chrome should be tintable
// - stop on selected row instead of snapping to it
// - default to UIPickerView scrolling behavior and chrome
//
// Since this is based on UIPickerView, refer to its documentation for more
// information: https://developer.apple.com/library/ios/documentation/uikit/reference/UIPickerView_Class/Reference/UIPickerView.html#//apple_ref/occ/cl/UIPickerView
@interface UIKPickerView : UIView

@property (nonatomic, assign) IBOutlet id<UIKPickerViewDataSource> dataSource;
@property (nonatomic, assign) IBOutlet id<UIKPickerViewDelegate> delegate;

// Gets the number of components for the picker view. (read-only)
//
// A UIPickerView object fetches the value of this property from the data source
// and caches it. The default value is zero.
@property(nonatomic, readonly) NSInteger numberOfComponents;

// A Boolean value that determines whether the selection indicator is displayed.
//
// If the value of the property is YES, the picker view shows a clear overlay
// across the current row. The default value of this property is NO.
@property(nonatomic) BOOL showsSelectionIndicator;

// Returns the number of rows for a component.
//
// A picker view fetches the value of this property from the data source and
// caches it. The default value is zero.
- (NSInteger)numberOfRowsInComponent:(NSInteger)component;

// Returns the size of a row for a component.
//
// A picker view fetches the value of this property by calling the
// pickerView:widthForComponent: and pickerView:rowHeightForComponent:
// delegate methods, and caches it. The default value is (0, 0).
- (CGSize)rowSizeForComponent:(NSInteger)component;

// Returns the view used by the picker view for a given row and component.
//
// The view provided by the delegate in the pickerView:viewForRow:forComponent:reusingView:
// method. Returns nil if the specified row of the component is not visible or
// if the delegate does not implement pickerView:viewForRow:forComponent:reusingView:.
- (UIView *)viewForRow:(NSInteger)row forComponent:(NSInteger)component;

// Reloads all components of the picker view.
//
// Calling this method causes the picker view to query the delegate for new data
// for all components.
- (void)reloadAllComponents;

// Reloads a particular component of the picker view.
//
// Calling this method causes the picker view to query the delegate for new data
// for the given component.
- (void)reloadComponent:(NSInteger)component;

// Selects a row in a specified component of the picker view.
- (void)selectRow:(NSInteger)row
      inComponent:(NSInteger)component
         animated:(BOOL)animated;

// Returns the index of the selected row in a given component.
//
// The return value is a zero-indexed number identifying the selected row, or -1
// if no row is selected.
- (NSInteger)selectedRowInComponent:(NSInteger)component;

@end
