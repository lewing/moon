/*
 * Automatically generated, do not edit this file directly
 */

#ifndef __MOONLIGHT_C_BINDING_H__
#define __MOONLIGHT_C_BINDING_H__

#include <glib.h>
// This should probably be changed to somehow not include c++ headers.
#include "animation.h"
#include "animation2.h"
#include "brush.h"
#include "canvas.h"
#include "clock.h"
#include "collection.h"
#include "contentcontrol.h"
#include "control.h"
#include "dependencyobject.h"
#include "dependencyproperty.h"
#include "deployment.h"
#include "downloader.h"
#include "eventargs.h"
#include "frameworkelement.h"
#include "geometry.h"
#include "grid.h"
#include "media.h"
#include "panel.h"
#include "runtime.h"
#include "shape.h"
#include "stackpanel.h"
#include "style.h"
#include "stylus.h"
#include "template.h"
#include "text.h"
#include "tilesource.h"
#include "transform.h"
#include "trigger.h"
#include "type.h"
#include "uielement.h"
#include "usercontrol.h"

G_BEGIN_DECLS

#if SL_2_0
/**
 * Application
 **/
/* @GeneratePInvoke */
Application *application_new (void);
#endif

/**
 * ArcSegment
 **/
/* @GeneratePInvoke */
ArcSegment *arc_segment_new (void);

#if SL_2_0
/**
 * AssemblyPart
 **/
/* @GeneratePInvoke */
AssemblyPart *assembly_part_new (void);
#endif

#if SL_2_0
/**
 * AssemblyPartCollection
 **/
/* @GeneratePInvoke */
AssemblyPartCollection *assembly_part_collection_new (void);
#endif

/**
 * BeginStoryboard
 **/
/* @GeneratePInvoke */
BeginStoryboard *begin_storyboard_new (void);

/**
 * BezierSegment
 **/
/* @GeneratePInvoke */
BezierSegment *bezier_segment_new (void);

/**
 * Brush
 **/
/* @GeneratePInvoke */
Brush *brush_new (void);

/**
 * Canvas
 **/
/* @GeneratePInvoke */
Canvas *canvas_new (void);

/**
 * Collection
 **/
/* @GeneratePInvoke */
int collection_add (Collection *instance, Value *value);

/* @GeneratePInvoke */
void collection_clear (Collection *instance);

/* @GeneratePInvoke */
bool collection_contains (Collection *instance, Value *value);

/* @GeneratePInvoke */
int collection_get_count (Collection *instance);

/* @GeneratePInvoke */
Type::Kind collection_get_element_type (Collection *instance);

#if SL_2_0
/* @GeneratePInvoke */
Value *collection_get_value_at_with_error (Collection *instance, int index, MoonError *error);
#endif

/* @GeneratePInvoke */
int collection_index_of (Collection *instance, Value *value);

/* @GeneratePInvoke */
bool collection_insert (Collection *instance, int index, Value *value);

/* @GeneratePInvoke */
bool collection_remove (Collection *instance, Value *value);

#if SL_2_0
/* @GeneratePInvoke */
bool collection_remove_at_with_error (Collection *instance, int index, MoonError *error);
#endif

#if SL_2_0
/* @GeneratePInvoke */
bool collection_set_value_at_with_error (Collection *instance, int index, Value *value, MoonError *error);
#endif

/**
 * ColorAnimation
 **/
/* @GeneratePInvoke */
ColorAnimation *color_animation_new (void);

/**
 * ColorAnimationUsingKeyFrames
 **/
/* @GeneratePInvoke */
ColorAnimationUsingKeyFrames *color_animation_using_key_frames_new (void);

/**
 * ColorKeyFrame
 **/
/* @GeneratePInvoke */
ColorKeyFrame *color_key_frame_new (void);

/**
 * ColorKeyFrameCollection
 **/
/* @GeneratePInvoke */
ColorKeyFrameCollection *color_key_frame_collection_new (void);

#if SL_2_0
/**
 * ColumnDefinition
 **/
/* @GeneratePInvoke */
ColumnDefinition *column_definition_new (void);
#endif

#if SL_2_0
/* @GeneratePInvoke */
double column_definition_get_actual_width (ColumnDefinition *instance);
#endif

#if SL_2_0
/**
 * ColumnDefinitionCollection
 **/
/* @GeneratePInvoke */
ColumnDefinitionCollection *column_definition_collection_new (void);
#endif

#if SL_2_0
/**
 * ContentControl
 **/
/* @GeneratePInvoke */
ContentControl *content_control_new (void);
#endif

#if SL_2_0
/**
 * Control
 **/
/* @GeneratePInvoke */
Control *control_new (void);
#endif

#if SL_2_0
/**
 * ControlTemplate
 **/
/* @GeneratePInvoke */
ControlTemplate *control_template_new (void);
#endif

#if SL_2_0
/**
 * DeepZoomImageTileSource
 **/
/* @GeneratePInvoke */
DeepZoomImageTileSource *deep_zoom_image_tile_source_new (void);
#endif

/**
 * DependencyObject
 **/
/* @GeneratePInvoke */
DependencyObject *dependency_object_find_name (DependencyObject *instance, const char *name, Type::Kind *element_kind);

#if SL_2_0
/* @GeneratePInvoke */
Value *dependency_object_get_default_value_with_error (DependencyObject *instance, Types *additional_types, DependencyProperty *property, MoonError *error);
#endif

/* @GeneratePInvoke */
const char *dependency_object_get_name (DependencyObject *instance);

/* @GeneratePInvoke */
Type::Kind dependency_object_get_object_type (DependencyObject *instance);

#if SL_2_0
/* @GeneratePInvoke */
Value *dependency_object_get_value_no_default_with_error (DependencyObject *instance, Types *additional_types, DependencyProperty *property, MoonError *error);
#endif

#if SL_2_0
/* @GeneratePInvoke */
Value *dependency_object_get_value_with_error (DependencyObject *instance, Types *additional_types, Type::Kind whatami, DependencyProperty *property, MoonError *error);
#endif

/* @GeneratePInvoke */
void dependency_object_set_name (DependencyObject *instance, const char *name);

/* @GeneratePInvoke */
void dependency_object_set_value (DependencyObject *instance, DependencyProperty *property, Value *value);

/**
 * DependencyObjectCollection
 **/
/* @GeneratePInvoke */
DependencyObjectCollection *dependency_object_collection_new (void);

/**
 * DependencyProperty
 **/
/* @GeneratePInvoke */
DependencyProperty *dependency_property_get_dependency_property (Type::Kind type, const char *name);

/* @GeneratePInvoke */
const char *dependency_property_get_name (DependencyProperty *instance);

/* @GeneratePInvoke */
Type::Kind dependency_property_get_property_type (DependencyProperty *instance);

/* @GeneratePInvoke */
bool dependency_property_is_attached (DependencyProperty *instance);

/* @GeneratePInvoke */
bool dependency_property_is_nullable (DependencyProperty *instance);

DependencyProperty *dependency_property_register (Type::Kind type, const char *name, Value *default_value);

#if SL_2_0
/* @GeneratePInvoke */
DependencyProperty *dependency_property_register_full (Types *additional_types, Type::Kind type, const char *name, Value *default_value, Type::Kind vtype, bool attached, bool read_only, bool always_change, NativePropertyChangedHandler *changed_callback);
#endif

#if SL_2_0
/* @GeneratePInvoke */
DependencyProperty *dependency_property_register_managed_property (Types *additional_types, const char *name, Type::Kind property_type, Type::Kind owner_type, bool attached, NativePropertyChangedHandler *callback);
#endif

#if SL_2_0
/**
 * Deployment
 **/
/* @GeneratePInvoke */
Deployment *deployment_new (void);
#endif

/**
 * DiscreteColorKeyFrame
 **/
/* @GeneratePInvoke */
DiscreteColorKeyFrame *discrete_color_key_frame_new (void);

/**
 * DiscreteDoubleKeyFrame
 **/
/* @GeneratePInvoke */
DiscreteDoubleKeyFrame *discrete_double_key_frame_new (void);

#if SL_2_0
/**
 * DiscreteObjectKeyFrame
 **/
/* @GeneratePInvoke */
DiscreteObjectKeyFrame *discrete_object_key_frame_new (void);
#endif

/**
 * DiscretePointKeyFrame
 **/
/* @GeneratePInvoke */
DiscretePointKeyFrame *discrete_point_key_frame_new (void);

/**
 * DoubleAnimation
 **/
/* @GeneratePInvoke */
DoubleAnimation *double_animation_new (void);

/**
 * DoubleAnimationUsingKeyFrames
 **/
/* @GeneratePInvoke */
DoubleAnimationUsingKeyFrames *double_animation_using_key_frames_new (void);

/**
 * DoubleCollection
 **/
/* @GeneratePInvoke */
DoubleCollection *double_collection_new (void);

/**
 * DoubleKeyFrame
 **/
/* @GeneratePInvoke */
DoubleKeyFrame *double_key_frame_new (void);

/**
 * DoubleKeyFrameCollection
 **/
/* @GeneratePInvoke */
DoubleKeyFrameCollection *double_key_frame_collection_new (void);

/**
 * Downloader
 **/
Downloader *downloader_new (void);

/**
 * DrawingAttributes
 **/
/* @GeneratePInvoke */
DrawingAttributes *drawing_attributes_new (void);

/**
 * Ellipse
 **/
/* @GeneratePInvoke */
Ellipse *ellipse_new (void);

/**
 * EllipseGeometry
 **/
/* @GeneratePInvoke */
EllipseGeometry *ellipse_geometry_new (void);

/**
 * EventObject
 **/
/* @GeneratePInvoke */
int event_object_add_handler (EventObject *instance, const char *event_name, EventHandler handler, gpointer data);

void event_object_drain_unrefs (void);

Surface *event_object_get_surface (EventObject *instance);

/* @GeneratePInvoke */
const char *event_object_get_type_name (EventObject *instance);

/* @GeneratePInvoke */
void event_object_ref (EventObject *instance);

/* @GeneratePInvoke */
void event_object_remove_handler (EventObject *instance, const char *event_name, EventHandler handler, gpointer data);

/* @GeneratePInvoke */
void event_object_unref (EventObject *instance);

/**
 * EventTrigger
 **/
/* @GeneratePInvoke */
EventTrigger *event_trigger_new (void);

/**
 * FrameworkElement
 **/
/* @GeneratePInvoke */
FrameworkElement *framework_element_new (void);

#if SL_2_0
/**
 * FrameworkTemplate
 **/
/* @GeneratePInvoke */
FrameworkTemplate *framework_template_new (void);
#endif

/**
 * GeneralTransform
 **/
/* @GeneratePInvoke */
GeneralTransform *general_transform_new (void);

/**
 * Geometry
 **/
/* @GeneratePInvoke */
Geometry *geometry_new (void);

/**
 * GeometryCollection
 **/
/* @GeneratePInvoke */
GeometryCollection *geometry_collection_new (void);

/**
 * GeometryGroup
 **/
/* @GeneratePInvoke */
GeometryGroup *geometry_group_new (void);

/**
 * Glyphs
 **/
/* @GeneratePInvoke */
Glyphs *glyphs_new (void);

/**
 * GradientBrush
 **/
/* @GeneratePInvoke */
GradientBrush *gradient_brush_new (void);

/**
 * GradientStop
 **/
/* @GeneratePInvoke */
GradientStop *gradient_stop_new (void);

/**
 * GradientStopCollection
 **/
/* @GeneratePInvoke */
GradientStopCollection *gradient_stop_collection_new (void);

#if SL_2_0
/**
 * Grid
 **/
/* @GeneratePInvoke */
Grid *grid_new (void);
#endif

/**
 * Image
 **/
/* @GeneratePInvoke */
Image *image_new (void);

/**
 * ImageBrush
 **/
/* @GeneratePInvoke */
ImageBrush *image_brush_new (void);

/**
 * InkPresenter
 **/
/* @GeneratePInvoke */
InkPresenter *ink_presenter_new (void);

/**
 * Inline
 **/
/* @GeneratePInvoke */
Inline *inline_new (void);

/**
 * InlineCollection
 **/
/* @GeneratePInvoke */
InlineCollection *inline_collection_new (void);

/**
 * KeyboardEventArgs
 **/
/* @GeneratePInvoke */
bool keyboard_event_args_get_handled (KeyboardEventArgs *instance);

/* @GeneratePInvoke */
void keyboard_event_args_set_handled (KeyboardEventArgs *instance, bool handled);

/**
 * KeyFrameCollection
 **/
/* @GeneratePInvoke */
KeyFrameCollection *key_frame_collection_new (void);

/**
 * KeySpline
 **/
/* @GeneratePInvoke */
KeySpline *key_spline_new (void);

/**
 * Line
 **/
/* @GeneratePInvoke */
Line *line_new (void);

/**
 * LinearColorKeyFrame
 **/
/* @GeneratePInvoke */
LinearColorKeyFrame *linear_color_key_frame_new (void);

/**
 * LinearDoubleKeyFrame
 **/
/* @GeneratePInvoke */
LinearDoubleKeyFrame *linear_double_key_frame_new (void);

/**
 * LinearGradientBrush
 **/
/* @GeneratePInvoke */
LinearGradientBrush *linear_gradient_brush_new (void);

/**
 * LinearPointKeyFrame
 **/
/* @GeneratePInvoke */
LinearPointKeyFrame *linear_point_key_frame_new (void);

/**
 * LineBreak
 **/
/* @GeneratePInvoke */
LineBreak *line_break_new (void);

/**
 * LineGeometry
 **/
/* @GeneratePInvoke */
LineGeometry *line_geometry_new (void);

/**
 * LineSegment
 **/
/* @GeneratePInvoke */
LineSegment *line_segment_new (void);

/**
 * Matrix
 **/
/* @GeneratePInvoke */
Matrix *matrix_new (void);

/**
 * MatrixTransform
 **/
/* @GeneratePInvoke */
MatrixTransform *matrix_transform_new (void);

/**
 * MediaAttribute
 **/
/* @GeneratePInvoke */
MediaAttribute *media_attribute_new (void);

/**
 * MediaAttributeCollection
 **/
/* @GeneratePInvoke */
MediaAttribute *media_attribute_collection_get_item_by_name (MediaAttributeCollection *instance, const char *name);

/* @GeneratePInvoke */
MediaAttributeCollection *media_attribute_collection_new (void);

/**
 * MediaBase
 **/
/* @GeneratePInvoke */
MediaBase *media_base_new (void);

/**
 * MediaElement
 **/
/* @GeneratePInvoke */
MediaElement *media_element_new (void);

/* @GeneratePInvoke */
void media_element_pause (MediaElement *instance);

/* @GeneratePInvoke */
void media_element_play (MediaElement *instance);

#if SL_2_0
/* @GeneratePInvoke */
void media_element_set_stream_source (MediaElement *instance, ManagedStreamCallbacks *stream);
#endif

/* @GeneratePInvoke */
void media_element_stop (MediaElement *instance);

/**
 * MouseEventArgs
 **/
/* @GeneratePInvoke */
bool mouse_event_args_get_handled (MouseEventArgs *instance);

/* @GeneratePInvoke */
void mouse_event_args_get_position (MouseEventArgs *instance, UIElement *relative_to, double *x, double *y);

/* @GeneratePInvoke */
StylusPointCollection *mouse_event_args_get_stylus_points (MouseEventArgs *instance, UIElement *ink_presenter);

/* @GeneratePInvoke */
MouseEventArgs *mouse_event_args_new (void);

/* @GeneratePInvoke */
void mouse_event_args_set_handled (MouseEventArgs *instance, bool handled);

#if SL_2_0
/**
 * MultiScaleTileSource
 **/
/* @GeneratePInvoke */
MultiScaleTileSource *multi_scale_tile_source_new (void);
#endif

#if SL_2_0
/**
 * ObjectAnimationUsingKeyFrames
 **/
/* @GeneratePInvoke */
ObjectAnimationUsingKeyFrames *object_animation_using_key_frames_new (void);
#endif

#if SL_2_0
/**
 * ObjectKeyFrame
 **/
/* @GeneratePInvoke */
ObjectKeyFrame *object_key_frame_new (void);
#endif

#if SL_2_0
/**
 * ObjectKeyFrameCollection
 **/
/* @GeneratePInvoke */
ObjectKeyFrameCollection *object_key_frame_collection_new (void);
#endif

/**
 * Panel
 **/
/* @GeneratePInvoke */
Panel *panel_new (void);

/**
 * ParallelTimeline
 **/
/* @GeneratePInvoke */
ParallelTimeline *parallel_timeline_new (void);

/**
 * Path
 **/
/* @GeneratePInvoke */
Path *path_new (void);

/**
 * PathFigure
 **/
/* @GeneratePInvoke */
PathFigure *path_figure_new (void);

/**
 * PathFigureCollection
 **/
/* @GeneratePInvoke */
PathFigureCollection *path_figure_collection_new (void);

/**
 * PathGeometry
 **/
/* @GeneratePInvoke */
PathGeometry *path_geometry_new (void);

/**
 * PathSegment
 **/
/* @GeneratePInvoke */
PathSegment *path_segment_new (void);

/**
 * PathSegmentCollection
 **/
/* @GeneratePInvoke */
PathSegmentCollection *path_segment_collection_new (void);

/**
 * PointAnimation
 **/
/* @GeneratePInvoke */
PointAnimation *point_animation_new (void);

/**
 * PointAnimationUsingKeyFrames
 **/
/* @GeneratePInvoke */
PointAnimationUsingKeyFrames *point_animation_using_key_frames_new (void);

/**
 * PointCollection
 **/
/* @GeneratePInvoke */
PointCollection *point_collection_new (void);

/**
 * PointKeyFrame
 **/
/* @GeneratePInvoke */
PointKeyFrame *point_key_frame_new (void);

/**
 * PointKeyFrameCollection
 **/
/* @GeneratePInvoke */
PointKeyFrameCollection *point_key_frame_collection_new (void);

/**
 * PolyBezierSegment
 **/
/* @GeneratePInvoke */
PolyBezierSegment *poly_bezier_segment_new (void);

/**
 * Polygon
 **/
/* @GeneratePInvoke */
Polygon *polygon_new (void);

/**
 * Polyline
 **/
/* @GeneratePInvoke */
Polyline *polyline_new (void);

/**
 * PolyLineSegment
 **/
/* @GeneratePInvoke */
PolyLineSegment *poly_line_segment_new (void);

/**
 * PolyQuadraticBezierSegment
 **/
/* @GeneratePInvoke */
PolyQuadraticBezierSegment *poly_quadratic_bezier_segment_new (void);

/**
 * QuadraticBezierSegment
 **/
/* @GeneratePInvoke */
QuadraticBezierSegment *quadratic_bezier_segment_new (void);

/**
 * RadialGradientBrush
 **/
/* @GeneratePInvoke */
RadialGradientBrush *radial_gradient_brush_new (void);

/**
 * Rectangle
 **/
/* @GeneratePInvoke */
Rectangle *rectangle_new (void);

/**
 * RectangleGeometry
 **/
/* @GeneratePInvoke */
RectangleGeometry *rectangle_geometry_new (void);

/**
 * ResourceDictionary
 **/
/* @GeneratePInvoke */
ResourceDictionary *resource_dictionary_new (void);

/**
 * RotateTransform
 **/
/* @GeneratePInvoke */
RotateTransform *rotate_transform_new (void);

/**
 * RoutedEventArgs
 **/
/* @GeneratePInvoke */
DependencyObject *routed_event_args_get_source (RoutedEventArgs *instance);

/* @GeneratePInvoke */
RoutedEventArgs *routed_event_args_new (void);

/* @GeneratePInvoke */
void routed_event_args_set_source (RoutedEventArgs *instance, DependencyObject *el);

#if SL_2_0
/**
 * RowDefinition
 **/
/* @GeneratePInvoke */
double row_definition_get_actual_height (RowDefinition *instance);
#endif

#if SL_2_0
/* @GeneratePInvoke */
RowDefinition *row_definition_new (void);
#endif

#if SL_2_0
/**
 * RowDefinitionCollection
 **/
/* @GeneratePInvoke */
RowDefinitionCollection *row_definition_collection_new (void);
#endif

/**
 * Run
 **/
/* @GeneratePInvoke */
Run *run_new (void);

/**
 * ScaleTransform
 **/
/* @GeneratePInvoke */
ScaleTransform *scale_transform_new (void);

#if SL_2_0
/**
 * Setter
 **/
/* @GeneratePInvoke */
Setter *setter_new (void);
#endif

#if SL_2_0
/**
 * SetterBase
 **/
/* @GeneratePInvoke */
SetterBase *setter_base_new (void);
#endif

#if SL_2_0
/**
 * SetterBaseCollection
 **/
/* @GeneratePInvoke */
SetterBaseCollection *setter_base_collection_new (void);
#endif

/**
 * Shape
 **/
/* @GeneratePInvoke */
Shape *shape_new (void);

/**
 * SkewTransform
 **/
/* @GeneratePInvoke */
SkewTransform *skew_transform_new (void);

/**
 * SolidColorBrush
 **/
/* @GeneratePInvoke */
SolidColorBrush *solid_color_brush_new (void);

/**
 * SplineColorKeyFrame
 **/
/* @GeneratePInvoke */
SplineColorKeyFrame *spline_color_key_frame_new (void);

/**
 * SplineDoubleKeyFrame
 **/
/* @GeneratePInvoke */
SplineDoubleKeyFrame *spline_double_key_frame_new (void);

/**
 * SplinePointKeyFrame
 **/
/* @GeneratePInvoke */
SplinePointKeyFrame *spline_point_key_frame_new (void);

#if SL_2_0
/**
 * StackPanel
 **/
/* @GeneratePInvoke */
StackPanel *stack_panel_new (void);
#endif

/**
 * Storyboard
 **/
/* @GeneratePInvoke */
bool storyboard_begin (Storyboard *instance);

/* @GeneratePInvoke */
void storyboard_pause (Storyboard *instance);

/* @GeneratePInvoke */
void storyboard_resume (Storyboard *instance);

/* @GeneratePInvoke */
void storyboard_seek (Storyboard *instance, TimeSpan timespan);

/* @GeneratePInvoke */
void storyboard_stop (Storyboard *instance);

/* @GeneratePInvoke */
Storyboard *storyboard_new (void);

/**
 * Stroke
 **/
/* @GeneratePInvoke */
bool stroke_hit_test (Stroke *instance, StylusPointCollection *stylusPoints);

/* @GeneratePInvoke */
Stroke *stroke_new (void);

/**
 * StrokeCollection
 **/
/* @GeneratePInvoke */
StrokeCollection *stroke_collection_hit_test (StrokeCollection *instance, StylusPointCollection *stylusPoints);

/* @GeneratePInvoke */
StrokeCollection *stroke_collection_new (void);

#if SL_2_0
/**
 * Style
 **/
/* @GeneratePInvoke */
Style *style_new (void);
#endif

/**
 * StylusInfo
 **/
/* @GeneratePInvoke */
StylusInfo *stylus_info_new (void);

/**
 * StylusPoint
 **/
/* @GeneratePInvoke */
StylusPoint *stylus_point_new (void);

/**
 * StylusPointCollection
 **/
/* @GeneratePInvoke */
StylusPointCollection *stylus_point_collection_new (void);

/**
 * Surface
 **/
/* @GeneratePInvoke */
void surface_attach (Surface *instance, UIElement *toplevel);

/* @GeneratePInvoke */
Downloader *surface_create_downloader (Surface *instance);

/* @GeneratePInvoke */
TimeManager *surface_get_time_manager (Surface *instance);

/* @GeneratePInvoke */
bool surface_get_transparent (Surface *instance);

MoonWindow *surface_get_window (Surface *instance);

/* @GeneratePInvoke */
void surface_paint (Surface *instance, cairo_t *ctx, int x, int y, int width, int height);

/* @GeneratePInvoke */
void surface_resize (Surface *instance, int width, int height);

/* @GeneratePInvoke */
void surface_set_transparent (Surface *instance, bool transparent);

/* @GeneratePInvoke */
Surface *surface_new (MoonWindow *window, bool silverlight2);

/**
 * TextBlock
 **/
/* @GeneratePInvoke */
TextBlock *text_block_new (void);

/**
 * TileBrush
 **/
/* @GeneratePInvoke */
TileBrush *tile_brush_new (void);

/**
 * Timeline
 **/
/* @GeneratePInvoke */
void timeline_set_manual_target (Timeline *instance, DependencyObject *o);

/* @GeneratePInvoke */
Timeline *timeline_new (void);

/**
 * TimelineCollection
 **/
/* @GeneratePInvoke */
TimelineCollection *timeline_collection_new (void);

/**
 * TimelineGroup
 **/
/* @GeneratePInvoke */
TimelineGroup *timeline_group_new (void);

/**
 * TimelineMarker
 **/
/* @GeneratePInvoke */
TimelineMarker *timeline_marker_new (void);

/**
 * TimelineMarkerCollection
 **/
/* @GeneratePInvoke */
TimelineMarkerCollection *timeline_marker_collection_new (void);

/**
 * Transform
 **/
/* @GeneratePInvoke */
Transform *transform_new (void);

/**
 * TransformCollection
 **/
/* @GeneratePInvoke */
TransformCollection *transform_collection_new (void);

/**
 * TransformGroup
 **/
/* @GeneratePInvoke */
TransformGroup *transform_group_new (void);

/**
 * TranslateTransform
 **/
/* @GeneratePInvoke */
TranslateTransform *translate_transform_new (void);

/**
 * TriggerAction
 **/
void trigger_action_fire (TriggerAction *instance);

/* @GeneratePInvoke */
TriggerAction *trigger_action_new (void);

/**
 * TriggerActionCollection
 **/
/* @GeneratePInvoke */
TriggerActionCollection *trigger_action_collection_new (void);

/**
 * TriggerCollection
 **/
/* @GeneratePInvoke */
TriggerCollection *trigger_collection_new (void);

#if SL_2_0
/**
 * Types
 **/
/* @GeneratePInvoke */
void types_free (Types *instance);
#endif

#if SL_2_0
/* @GeneratePInvoke */
Type *types_find (Types *instance, Type::Kind type);
#endif

#if SL_2_0
/* @GeneratePInvoke */
Type::Kind types_register_type (Types *instance, const char *name, void *gc_handle, Type::Kind parent);
#endif

#if SL_2_0
/* @GeneratePInvoke */
Types *types_new (void);
#endif

/**
 * UIElement
 **/
/* @GeneratePInvoke */
void uielement_measure (UIElement *instance, Size availableSize);

/* @GeneratePInvoke */
UIElement *uielement_new (void);

/**
 * UIElementCollection
 **/
/* @GeneratePInvoke */
UIElementCollection *uielement_collection_new (void);

#if SL_2_0
/**
 * UserControl
 **/
/* @GeneratePInvoke */
UserControl *user_control_new (void);
#endif

/**
 * VideoBrush
 **/
/* @GeneratePInvoke */
VideoBrush *video_brush_new (void);

/**
 * VisualBrush
 **/
/* @GeneratePInvoke */
VisualBrush *visual_brush_new (void);


G_END_DECLS

#endif
