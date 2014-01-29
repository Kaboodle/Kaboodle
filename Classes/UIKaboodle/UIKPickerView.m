//
//  UIKPickerView.m
//  Kaboodle

#import "UIKPickerView.h"

@class UIKComponent;

#pragma mark -
#pragma mark UIKComponentDelegate

// An UIKPickerView object is a delegate of each component it manages. The
// UIKPickerView object implements this protocol and acts as a switchboard to
// forward messages it receives from a component to the picker view's delegate.
//
// The protocol extends the UIScrollViewDelegate in order to know when scrolling
// or dragging ended.
@protocol UIKComponentDelegate <UIScrollViewDelegate>

// Called by the component when it needs the view to use for a given row.
//
// The UIPickerView object will forward the message on to its delegate and call
// pickerView:viewForRow:forComponent:reusingView: or pickerView:titleForRow:forComponent:
- (UIView *)component:(UIKComponent *)component
           viewForRow:(NSInteger)row
          reusingView:(UIView *)view;

// Called by the picker view when the user selects a row in a component.
//
// The UIPickerView object will forward the message on to its delegate and call
// pickerView:didSelectRow:inComponent:
- (void)component:(UIKComponent *)component didSelectRow:(NSInteger)row;

@end

#pragma mark -
#pragma mark UIKComponent

// The UIKComponent class is a private class that implements objects, called
// components, that use a spinning-wheel or slot-machine metaphor to show one or
// more sets of values. Each component is responsible for drawing its rows and
// handling touches, and for notifying its parent picker view of any events. 
@interface UIKComponent : UIScrollView

@property (nonatomic, assign) id<UIKComponentDelegate> delegate;

// properties for managing row state
@property (nonatomic, assign) NSInteger selectedRow;
@property (nonatomic, assign) CGFloat rowHeight;
@property (nonatomic, assign) NSInteger numberOfRows;

// properties for managing row views
@property (nonatomic, strong) NSMutableSet *recycledViews;
@property (nonatomic, strong) NSMutableArray *visibleViews;
@property (nonatomic, strong) NSMutableArray *visibleIndices;

// Returns the view used by the component for a given row.
//
// Returns the cached view provided by the delegate in the component:viewForRow:reusingView:
// method. Returns nil if the specified row of the component is not visible,
// i.e., not in the cache.  This will be called by viewForRow:forComponent: from
// the parent picker view.
- (UIView *)viewForRow:(NSInteger)row;

// Selects a row in the component. This will be called by selectRow:inComponent:animated:
// from the parent picker view.
- (void)selectRow:(NSInteger)row animated:(BOOL)animated;

// Adjusts the content offset of the component so that the row view closest to
// the center of the picker view will be the selected row. Snapping occurs when
// scrolling or dragging ends and called from the corresponding delegate methods.
- (void)snapToCenter;

@end

@implementation UIKComponent

@dynamic delegate;
@synthesize selectedRow = _selectedRow;
@synthesize rowHeight = _rowHeight;
@synthesize numberOfRows = _numberOfRows;
@synthesize recycledViews = _recycledViews;
@synthesize visibleViews = _visibleViews;
@synthesize visibleIndices = _visibleIndices;

#pragma mark -
#pragma mark Initialization

#define kDefaultRowHeight  44.0

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        [self initialize];
    }
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {
    self = [super initWithCoder:aDecoder];
    if (self) {
        [self initialize];
    }
    return self;
}

- (void)initialize {
    self.selectedRow = 0;
    self.rowHeight = kDefaultRowHeight;
    self.numberOfRows = 0;
    self.recycledViews = [[NSMutableSet alloc] init];
    self.visibleViews = [[NSMutableArray alloc] init];
    self.visibleIndices = [[NSMutableArray alloc] init];
    
    self.showsHorizontalScrollIndicator = NO;
    self.showsVerticalScrollIndicator = NO;
    
    UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc]
                                   initWithTarget:self
                                   action:@selector(handleTap:)];
    [self addGestureRecognizer:tap];
}

#pragma mark -
#pragma mark Public Properties

- (NSInteger)selectedRow {
    if (self.numberOfRows > 0) {
        return _selectedRow;
    } else {
        return -1;
    }
}

- (void)setSelectedRow:(NSInteger)selectedRow {
    _selectedRow = selectedRow;
    
    [self.delegate component:self didSelectRow:selectedRow];
}

#pragma mark -
#pragma mark Public Methods

- (UIView *)viewForRow:(NSInteger)row {
    NSInteger i = -1;
    for (NSNumber *visibileIndex in self.visibleIndices) {
        if ([visibileIndex integerValue] == row) {
            i = [self.visibleIndices indexOfObject:visibileIndex];
        }
    }
    
    if (i >= 0) {
        return [self.visibleViews objectAtIndex:i];
    }

    return nil;
}

- (void)selectRow:(NSInteger)row animated:(BOOL)animated; {
    // the following logic assumes the scrollView is not animating and
    // the selected index is already snapped to center
    
    // Determine how many indices |row| is from the currently selected row in
    // both the positive and negative direction.
    NSInteger indexOffsetPositive;
    NSInteger indexOffsetNegative;
    if (self.selectedRow > row) {
        indexOffsetNegative = row - self.selectedRow;
        indexOffsetPositive = self.numberOfRows + indexOffsetNegative;
    } else {
        indexOffsetPositive = row - self.selectedRow;
        indexOffsetNegative = indexOffsetPositive - self.numberOfRows;
    }
    
    // Pick the smallest offset 
    NSInteger indexOffset;
    if (abs(indexOffsetPositive) > abs(indexOffsetNegative))    {
        indexOffset = indexOffsetNegative;
    } else {
        indexOffset = indexOffsetPositive;
    }
    
    // Map the index offset to points and apply the offset
    CGFloat offsetY = self.rowHeight * indexOffset;
    CGPoint contentOffset = self.contentOffset;
    contentOffset.y += offsetY;
    [self setContentOffset:contentOffset animated:animated];
    
    // Update the selected row
    self.selectedRow = row;
}

- (void)snapToCenter {
    // Cancel if the user is currently dragging or the scrollview is animating.
    // This should solve any race condition bugs.
    if (self.dragging || self.decelerating) return;
    
    CGPoint center = CGPointMake(CGRectGetMidX(self.bounds), CGRectGetMidY(self.bounds));
    UIView *viewClosestToCenter;
    for (UIView *view in self.visibleViews) {
        if (CGRectContainsPoint(view.frame, center)) {
            viewClosestToCenter = view;
            break;
        }
    }
    
    CGFloat distanceFromCenter = viewClosestToCenter.center.y - center.y;
    CGPoint contentOffset = self.contentOffset;
    contentOffset.y += distanceFromCenter;
    [self setContentOffset:contentOffset animated:YES];
    
    NSUInteger selectedIndex = [self.visibleViews indexOfObject:viewClosestToCenter];
    self.selectedRow = [[self.visibleIndices objectAtIndex:selectedIndex] integerValue];
}

#pragma mark -
#pragma mark Gesture Handlers

- (void)handleTap:(UITapGestureRecognizer *)recognizer {
    CGPoint tapPoint = [recognizer locationInView:self];
    
    NSInteger tappedRow = -1;
    for (UIView *view in self.visibleViews) {
        if (CGRectContainsPoint(view.frame, tapPoint)) {
            tappedRow = [[self.visibleIndices objectAtIndex:[self.visibleViews indexOfObject:view]] integerValue];
            break;
        }
    }
    
    if (tappedRow >= 0) {
        [self selectRow:tappedRow animated:YES];
    }
}

#pragma mark -
#pragma mark Layout

// The infinite scrolling technique is adapted from Session 104 at WWDC 2011,
// illustrated by the StreetScroller sample code. The view recycling technique
// is adapted from Sessiong 104 at WWDC 2010, illustrated by the PhotoScroller
// sample code.
//
// Videos of the sessions are available via Apple's Developer site
//
// StreetScroller: https://developer.apple.com/library/ios/#samplecode/StreetScroller/Introduction/Intro.html#//apple_ref/doc/uid/DTS40011102
// PhotoScroller: https://developer.apple.com/library/ios/#samplecode/PhotoScroller/Introduction/Intro.html#//apple_ref/doc/uid/DTS40010080
- (void)layoutSubviews {
    [super layoutSubviews];
    
    [self recenterIfNecessary];
    
    // tile content in visible bounds
    CGRect visibleBounds = [self bounds];
    CGFloat minimumVisibleY = CGRectGetMinY(visibleBounds);
    CGFloat maximumVisibleY = CGRectGetMaxY(visibleBounds);
     
    [self tileViewsFromMinY:minimumVisibleY toMaxY:maximumVisibleY];
}

// recenter content periodically to achieve impression of infinite scrolling
- (void)recenterIfNecessary {
    CGPoint currentOffset = [self contentOffset];
    CGFloat contentHeight = [self contentSize].height;
    CGFloat centerOffsetY = (contentHeight - [self bounds].size.height) / 2.0;
    CGFloat distanceFromCenter = fabs(currentOffset.y - centerOffsetY);
    
    if (distanceFromCenter > (contentHeight / 4.0)) {
        self.contentOffset = CGPointMake(currentOffset.x, centerOffsetY);
        
        // move content by the same amount so it appears to stay still
        for (UIView *view in self.visibleViews) {
            CGPoint center = view.center;
            center.y += (centerOffsetY - currentOffset.y);
            view.center = center;
        }
    }
}

- (UIView *)dequeueRecycledView {
    UIView *view = [self.recycledViews anyObject];
    if (view) {
        [self.recycledViews removeObject:view];
    }
    return view;
}

- (UIView *)insertViewForRow:(NSInteger)row {
    UIView *view = [self dequeueRecycledView];
    view = [self.delegate component:self viewForRow:row reusingView:view];
    
    [self addSubview:view];
    
    return view;
}

- (CGFloat)placeNewViewCenter:(CGFloat)center forRow:(NSInteger)row {
    UIView *view = [self insertViewForRow:row];
    
    // add forwardmost view at the end of the array
    [self.visibleViews addObject:view]; 
    [self.visibleIndices addObject:[NSNumber numberWithInteger:row]];
    
    CGRect frame = [view frame];
    frame.size.height = self.rowHeight;
    frame.origin.x = ([self bounds].size.width - frame.size.width)/2;
    frame.origin.y = center - frame.size.height/2;
    [view setFrame:frame];
    
    return CGRectGetMaxY(frame);
}

- (CGFloat)placeNewViewBelow:(CGFloat)bottomEdge forRow:(NSInteger)row {
    UIView *view = [self insertViewForRow:row];
    
    // add forwardmost view at the end of the array
    [self.visibleViews addObject:view]; 
    [self.visibleIndices addObject:[NSNumber numberWithInteger:row]];
    
    CGRect frame = [view frame];
    frame.size.height = self.rowHeight;
    frame.origin.x = ([self bounds].size.width - frame.size.width)/2;
    frame.origin.y = bottomEdge;
    [view setFrame:frame];
    
    return CGRectGetMaxY(frame);
}

- (CGFloat)placeNewViewAbove:(CGFloat)topEdge forRow:(NSInteger)row {
    UIView *view = [self insertViewForRow:row];
    
    // add backwardmost view at the beginning of the array
    [self.visibleViews insertObject:view atIndex:0]; 
    [self.visibleIndices insertObject:[NSNumber numberWithInteger:row]
                              atIndex:0];
    
    CGRect frame = [view frame];
    frame.size.height = self.rowHeight;
    frame.origin.x = ([self bounds].size.width - frame.size.width)/2;
    frame.origin.y = topEdge - frame.size.height;
    [view setFrame:frame];
    
    return CGRectGetMinY(frame);
}

- (void)tileViewsFromMinY:(CGFloat)minimumVisibleY toMaxY:(CGFloat)maximumVisibleY {
    // the upcoming tiling logic depends on there already being at least one
    // view in the visible views array, so to kick off the tiling we need to
    // make sure there's at least one view
    NSInteger nextRow = 0;
    if ([self.visibleViews count] == 0) {
        CGFloat centerY = (maximumVisibleY - minimumVisibleY)/2 + minimumVisibleY;
        [self placeNewViewCenter:centerY forRow:nextRow];
    }
    
    // add views that are missing on bottom
    UIView *lastView = [self.visibleViews lastObject];
    CGFloat bottomEdge = CGRectGetMaxY([lastView frame]);
    nextRow = [[self.visibleIndices lastObject] integerValue] + 1;
    while (bottomEdge < maximumVisibleY) {
        if (nextRow == self.numberOfRows) {
            nextRow = 0;
        }
        bottomEdge = [self placeNewViewBelow:bottomEdge forRow:nextRow];
        nextRow++;
    }
    
    // add views that are missing on top
    UIView *firstView = [self.visibleViews objectAtIndex:0];
    CGFloat topEdge = CGRectGetMinY([firstView frame]);
    nextRow = [[self.visibleIndices objectAtIndex:0] integerValue] - 1;
    while (topEdge > minimumVisibleY) {
        if (nextRow < 0) {
            nextRow = self.numberOfRows - 1;
        }
        topEdge = [self placeNewViewAbove:topEdge forRow:nextRow];
        nextRow--;
    }
    
    // remove views that have fallen off bottom edge and put them in the
    // recycle bin
    lastView = [self.visibleViews lastObject];
    while ([lastView frame].origin.y > maximumVisibleY) {
        [lastView removeFromSuperview];
        [self.visibleViews removeLastObject];
        [self.visibleIndices removeLastObject];
        [self.recycledViews addObject:lastView];
        lastView = [self.visibleViews lastObject];
    }
    
    // remove views that have fallen off top edge and put them in the
    // recycle bin
    firstView = [self.visibleViews objectAtIndex:0];
    while (CGRectGetMaxY([firstView frame]) < minimumVisibleY) {
        [firstView removeFromSuperview];
        [self.visibleViews removeObjectAtIndex:0];
        [self.visibleIndices removeObjectAtIndex:0];
        [self.recycledViews addObject:firstView];
        firstView = [self.visibleViews objectAtIndex:0];
    }
}
 
@end

#pragma mark -
#pragma mark UIKPickerView

@interface UIKPickerView () <UIKComponentDelegate>

@property (nonatomic, strong) NSMutableArray *components;

@end

@implementation UIKPickerView

@synthesize dataSource = _dataSource;
@synthesize delegate = _delegate;
@synthesize components = _components;
@synthesize numberOfComponents = _numberOfComponents;

#pragma mark -
#pragma Initialization

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame]; 
    if (self) {
        [self initialize];
    }
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {
    if ((self = [super initWithCoder:aDecoder])) {
        [self initialize];
    }
    return self;
}

- (void)initialize {
    self.components = [[NSMutableArray alloc] init];
}

#pragma mark -
#pragma mark Getting the Dimensions of the View Picker

- (NSInteger)numberOfRowsInComponent:(NSInteger)component {
    if ([self.components count] > 0) {
        return [[self getComponent:component] numberOfRows];
    } else {
        return 0;
    }
}

- (CGSize)rowSizeForComponent:(NSInteger)component {
    if ([self.components count] > 0) {
        return CGSizeMake([self getComponent:component].frame.size.width,
                          [self getComponent:component].rowHeight);
    } else {
        return CGSizeMake(0, 0);
    }
}

#pragma mark -
#pragma mark Reloading the View Picker

- (void)reloadAllComponents {
    self.components = [[NSMutableArray alloc] init];
    
    [self setNeedsLayout];
}

- (void)reloadComponent:(NSInteger)component {
    [self.components replaceObjectAtIndex:component
                               withObject:[self createComponent:component]];
    
    [self setNeedsLayout];
}

#pragma mark -
#pragma mark Selecting Rows in the View Picker

- (void)selectRow:(NSInteger)row
      inComponent:(NSInteger)component
         animated:(BOOL)animated {
    [[self getComponent:component] selectRow:row animated:animated];
}

- (NSInteger)selectedRowInComponent:(NSInteger)component; {
    return [[self getComponent:component] selectedRow];
}

#pragma mark -
#pragma mark Returning the View for a Row and Component

- (UIView *)viewForRow:(NSInteger)row forComponent:(NSInteger)component {
    if ([self.delegate respondsToSelector:@selector(pickerView:viewForRow:forComponent:reusingView:)]) {
        return [[self getComponent:component] viewForRow:row];
    }
    
    return nil;
}

#pragma mark -
#pragma mark Private Methods

- (UIKComponent *)getComponent:(NSInteger)component {
    return [self.components objectAtIndex:component];
}

#pragma mark -
#pragma mark UIKComponentDelegate Methods

- (UIView *)component:(UIKComponent *)component
           viewForRow:(NSInteger)row
          reusingView:(UIView *)view {
    NSInteger componentIndex = [self.components indexOfObject:component];
    
    if ([self.delegate respondsToSelector:@selector(pickerView:viewForRow:forComponent:reusingView:)]) {
        return [self.delegate pickerView:self
                              viewForRow:row
                            forComponent:componentIndex
                             reusingView:view];
    } else {
        UILabel *label;
        if ([view isKindOfClass:[UILabel class]]) {
            label = (UILabel *)view;
        } else {
            CGRect frame = CGRectMake(0,
                                      0,
                                      component.frame.size.width,
                                      component.rowHeight);
            label = [[UILabel alloc] initWithFrame:frame];
            label.font = [UIFont boldSystemFontOfSize:20];
        }
        
        if ([self.delegate respondsToSelector:@selector(pickerView:titleForRow:forComponent:)]) {
            label.text = [self.delegate pickerView:self
                                       titleForRow:row
                                      forComponent:componentIndex];
        } else {
            label.text = @"?";
        }
        
        return label;
    }
}

- (void)component:(UIKComponent *)component didSelectRow:(NSInteger)row {
    if ([self.delegate respondsToSelector:@selector(pickerView:didSelectRow:inComponent:)]) {
        NSInteger componentIndex = [self.components indexOfObject:component];
        [self.delegate pickerView:self
                     didSelectRow:row
                      inComponent:componentIndex];
    }
}

- (void)scrollViewDidEndDecelerating:(UIKComponent *)component {
    [component snapToCenter];
}

- (void)scrollViewDidEndDragging:(UIKComponent *)component
                  willDecelerate:(BOOL)decelerate {
    if (!decelerate) [component snapToCenter];
}

#pragma mark -
#pragma mark Layout

- (UIKComponent *)createComponent:(NSInteger)i {
    // calculate component's frame
    CGFloat componentWidth;
    if ([self.delegate respondsToSelector:@selector(pickerView:widthForComponent:)]) {
        componentWidth = [self.delegate pickerView:self widthForComponent:i];
    } else {
        componentWidth = self.bounds.size.width/self.numberOfComponents;
    }
    
    CGRect componentFrame = CGRectMake(0, 0, componentWidth, self.bounds.size.height);
    UIKComponent *component = [[UIKComponent alloc] initWithFrame:componentFrame];
    
    // calculate component's content size
    if ([self.delegate respondsToSelector:@selector(pickerView:rowHeightForComponent:)]) {
        component.rowHeight = [self.delegate pickerView:self
                                  rowHeightForComponent:i];
    }
    component.numberOfRows = [self.dataSource pickerView:self
                                 numberOfRowsInComponent:i];
    component.contentSize = CGSizeMake(componentWidth, 3 * componentFrame.size.height);
    
    // configure other properties
    component.delegate = self;
    component.decelerationRate = 0.994; // Normal = 0.998, Fast = 0.990
    
    return component;
}

- (void)layoutSubviews {
    [super layoutSubviews];
    
    _numberOfComponents = [self.dataSource numberOfComponentsInPickerView:self];
    
    // Since reloadComponent: only updates one component, but still triggers
    // layoutSubviews, it's possible the data source will say there are less
    // components than before, so prune the extra components
    while (self.components.count > self.numberOfComponents) {
        [self.components removeLastObject];
    }
    
    CGFloat offset = 0;
    CGFloat widthOfComponents = offset; // for centering
    for (NSInteger i=0; i < self.numberOfComponents; i++) {
        // get component or create new one
        UIKComponent *component;
        if (i < self.components.count) {
            component = [self.components objectAtIndex:i];
        } else {
            component = [self createComponent:i];
            [self addSubview:component];
            [self.components addObject:component];
        }
        
        // position component at offset
        CGRect frame = component.frame;
        frame.origin.x = offset;
        component.frame = frame;
        
        // increase offset by width of component
        offset += frame.size.width;
        
        widthOfComponents = offset; // for centering
        
        offset += 2; // gutters
    }
    
    // for centering
    CGFloat minimumInset = 11.0; // 0.0 if no chrome
    offset = MAX((self.bounds.size.width - widthOfComponents)/2.0, minimumInset);
    for (NSInteger i=0; i < self.numberOfComponents; i++) {
        UIKComponent *component = [self.components objectAtIndex:i];
        
        CGRect frame = component.frame;
        frame.origin.x += offset;
        component.frame = frame;
    }
}

@end
