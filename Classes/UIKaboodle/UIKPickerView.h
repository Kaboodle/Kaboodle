//
//  UIKPickerView.h
//  UIKPickerView
//

#import <UIKit/UIKit.h>

@protocol UIKPickerViewDataSource, UIKPickerViewDelegate;

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

#pragma mark -
#pragma mark UIKPickerViewDataSource

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

#pragma mark -
#pragma mark UIKPickerViewDelegate

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
