/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */

#include "pal-gtk.h"

#include "runtime.h"
#include "window-gtk.h"
#include "im-gtk.h"

/// we put all the events here since nothing outside of this file
/// should be relying on the gtk internals.
#include <gdk/gdkkeysyms.h>

static Key MapKeyvalToKey (guint keyval)
{
	switch (keyval) {
	case GDK_BackSpace:				return KeyBACKSPACE;
	case GDK_Tab: case GDK_ISO_Left_Tab:		return KeyTAB;
	case GDK_Return: case GDK_KP_Enter:		return KeyENTER;
	case GDK_Shift_L: case GDK_Shift_R:		return KeySHIFT;
	case GDK_Control_L: case GDK_Control_R:		return KeyCTRL;
	case GDK_Alt_L: case GDK_Alt_R:			return KeyALT;
	case GDK_Caps_Lock:				return KeyCAPSLOCK;
	case GDK_Escape:				return KeyESCAPE;
	case GDK_space: case GDK_KP_Space:		return KeySPACE;
	case GDK_Page_Up: case GDK_KP_Page_Up:		return KeyPAGEUP;
	case GDK_Page_Down: case GDK_KP_Page_Down:	return KeyPAGEDOWN;
	case GDK_End: case GDK_KP_End:			return KeyEND;
	case GDK_Home: case GDK_KP_Home:		return KeyHOME;
	case GDK_Left: case GDK_KP_Left:		return KeyLEFT;
	case GDK_Up: case GDK_KP_Up:			return KeyUP;
	case GDK_Right: case GDK_KP_Right:		return KeyRIGHT;
	case GDK_Down: case GDK_KP_Down:		return KeyDOWN;
	case GDK_Insert: case GDK_KP_Insert:		return KeyINSERT;
	case GDK_Delete: case GDK_KP_Delete:		return KeyDELETE;
	case GDK_0:					return KeyDIGIT0;
	case GDK_1:					return KeyDIGIT1;
	case GDK_2:					return KeyDIGIT2;
	case GDK_3:					return KeyDIGIT3;
	case GDK_4:					return KeyDIGIT4;
	case GDK_5:					return KeyDIGIT5;
	case GDK_6:					return KeyDIGIT6;
	case GDK_7:					return KeyDIGIT7;
	case GDK_8:					return KeyDIGIT8;
	case GDK_9:					return KeyDIGIT9;
	case GDK_a: case GDK_A:				return KeyA;
	case GDK_b: case GDK_B:				return KeyB;
	case GDK_c: case GDK_C:				return KeyC;
	case GDK_d: case GDK_D:				return KeyD;
	case GDK_e: case GDK_E:				return KeyE;
	case GDK_f: case GDK_F:				return KeyF;
	case GDK_g: case GDK_G:				return KeyG;
	case GDK_h: case GDK_H:				return KeyH;
	case GDK_i: case GDK_I:				return KeyI;
	case GDK_j: case GDK_J:				return KeyJ;
	case GDK_k: case GDK_K:				return KeyK;
	case GDK_l: case GDK_L:				return KeyL;
	case GDK_m: case GDK_M:				return KeyM;
	case GDK_n: case GDK_N:				return KeyN;
	case GDK_o: case GDK_O:				return KeyO;
	case GDK_p: case GDK_P:				return KeyP;
	case GDK_q: case GDK_Q:				return KeyQ;
	case GDK_r: case GDK_R:				return KeyR;
	case GDK_s: case GDK_S:				return KeyS;
	case GDK_t: case GDK_T:				return KeyT;
	case GDK_u: case GDK_U:				return KeyU;
	case GDK_v: case GDK_V:				return KeyV;
	case GDK_w: case GDK_W:				return KeyW;
	case GDK_x: case GDK_X:				return KeyX;
	case GDK_y: case GDK_Y:				return KeyY;
	case GDK_z: case GDK_Z:				return KeyZ;
		
	case GDK_F1: case GDK_KP_F1:			return KeyF1;
	case GDK_F2: case GDK_KP_F2:			return KeyF2;
	case GDK_F3: case GDK_KP_F3:			return KeyF3;
	case GDK_F4: case GDK_KP_F4:			return KeyF4;
	case GDK_F5:					return KeyF5;
	case GDK_F6:					return KeyF6;
	case GDK_F7:					return KeyF7;
	case GDK_F8:					return KeyF8;
	case GDK_F9:					return KeyF9;
	case GDK_F10:					return KeyF10;
	case GDK_F11:					return KeyF11;
	case GDK_F12:					return KeyF12;
		
	case GDK_KP_0:					return KeyNUMPAD0;
	case GDK_KP_1:					return KeyNUMPAD1;
	case GDK_KP_2:					return KeyNUMPAD2;
	case GDK_KP_3:					return KeyNUMPAD3;
	case GDK_KP_4:					return KeyNUMPAD4;
	case GDK_KP_5:					return KeyNUMPAD5;
	case GDK_KP_6:					return KeyNUMPAD6;
	case GDK_KP_7:					return KeyNUMPAD7;
	case GDK_KP_8:					return KeyNUMPAD8;
	case GDK_KP_9:					return KeyNUMPAD9;
		
	case GDK_KP_Multiply: case GDK_asterisk:	return KeyMULTIPLY;
	case GDK_KP_Add: case GDK_plus:			return KeyADD;
	case GDK_KP_Subtract: case GDK_minus:		return KeySUBTRACT;
	case GDK_KP_Decimal: case GDK_period:  		return KeyDECIMAL;
	case GDK_KP_Divide: case GDK_slash:		return KeyDIVIDE;
		
	default:
		return KeyUNKNOWN;
	}
}


class MoonKeyEventGtk : public MoonKeyEvent {
public:
	MoonKeyEventGtk (GdkEvent *event)
	{
		this->event = (GdkEventKey*)gdk_event_copy (event);

		key = MapKeyvalToKey (this->event->keyval);
	}

	virtual ~MoonKeyEventGtk ()
	{
		gdk_event_free ((GdkEvent*)event);
	}

	virtual MoonEvent* Clone ()
	{
		return new MoonKeyEventGtk ((GdkEvent*)event);
	}

	virtual gpointer GetPlatformEvent ()
	{
		return event;
	}

	virtual Key GetSilverlightKey ()
	{
		return key;
	}

	virtual int GetPlatformKeycode ()
	{
		return event->hardware_keycode;
	}

	virtual int GetPlatformKeyval ()
	{
		return event->keyval;
	}

	virtual gunichar GetUnicode ()
	{
		return gdk_keyval_to_unicode (event->keyval);
	}

	virtual MoonModifier GetModifiers ()
	{
		return (MoonModifier) event->state;
	}


	virtual bool IsModifier ()
	{
#if GTK_CHECK_VERSION(2,10,0)
	if (gtk_check_version(2,10,0))
		return event->is_modifier;
	else
#endif
		switch (event->keyval) {
		case GDK_Shift_L:
		case GDK_Shift_R:
		case GDK_Control_L:
		case GDK_Control_R:
		case GDK_Meta_L:
		case GDK_Meta_R:
		case GDK_Alt_L:
		case GDK_Alt_R:
		case GDK_Super_L:
		case GDK_Super_R:
		case GDK_Hyper_L:
		case GDK_Hyper_R:
			return true;
		default:
			return false;
		}
	}

private:
	GdkEventKey *event;

	Key key;
};

static void GetStylusInfoFromDevice (GdkDevice *gdk_device, TabletDeviceType *type, bool *is_inverted)
{
	if (!gdk_device)
		return;

	switch (gdk_device->source) {
	case GDK_SOURCE_PEN:
	case GDK_SOURCE_ERASER:
		*type = TabletDeviceTypeStylus;
		break;
	case GDK_SOURCE_MOUSE:
	case GDK_SOURCE_CURSOR: /* XXX not sure where to lump this in..  in the stylus block? */
	default:
		*type = TabletDeviceTypeMouse;
		break;
	}

	*is_inverted = (gdk_device->source == GDK_SOURCE_ERASER);
}

class MoonButtonEventGtk : public MoonButtonEvent {
public:
	MoonButtonEventGtk (GdkEvent *event)
	{
		this->event = (GdkEventButton*)gdk_event_copy (event);
	}

	virtual ~MoonButtonEventGtk ()
	{
		gdk_event_free ((GdkEvent*)event);
	}

	virtual MoonEvent* Clone ()
	{
		return new MoonButtonEventGtk ((GdkEvent*)event);
	}

	virtual gpointer GetPlatformEvent ()
	{
		return event;
	}
	
	virtual Point GetPosition ()
	{
		return Point (event->x, event->y);
	}

	virtual double GetPressure ()
	{
		double pressure = 0.0;
		if (!event->device || !gdk_event_get_axis ((GdkEvent*)event, GDK_AXIS_PRESSURE, &pressure))
			pressure = 0.0;
		return pressure;
	}

	virtual void GetStylusInfo (TabletDeviceType *type, bool *is_inverted)
	{
		GetStylusInfoFromDevice (event->device, type, is_inverted);
	}

	virtual MoonModifier GetModifiers ()
	{
		g_assert_not_reached ();
	}

	bool IsRelease ()
	{
		return event->type == GDK_BUTTON_RELEASE;
	}

	int GetButton ()
	{
		return event->button;
	}

	// the number of clicks.  gdk provides them as event->type ==
	// GDK_3BUTTON_PRESS/GDK_2BUTTON_PRESS/GDK_BUTTON_PRESS
	virtual int GetNumberOfClicks ()
	{
		switch (event->type) {
		case GDK_BUTTON_PRESS: return 1;
		case GDK_2BUTTON_PRESS: return 2;
		case GDK_3BUTTON_PRESS: return 3;
		default: return 0;
		}
	}

private:
	GdkEventButton *event;
};


class MoonMotionEventGtk : public MoonMotionEvent {
public:
	MoonMotionEventGtk (GdkEvent *event)
	{
		this->event = (GdkEventMotion*)gdk_event_copy (event);
	}

	virtual ~MoonMotionEventGtk ()
	{
		gdk_event_free ((GdkEvent*)event);
	}

	virtual MoonEvent* Clone ()
	{
		return new MoonMotionEventGtk ((GdkEvent*)event);
	}

	virtual gpointer GetPlatformEvent ()
	{
		return event;
	}

	virtual Point GetPosition ()
	{
		return Point (event->x, event->y);
	}

	virtual double GetPressure ()
	{
		double pressure = 0.0;
		if (!event->device || !gdk_event_get_axis ((GdkEvent*)event, GDK_AXIS_PRESSURE, &pressure))
			pressure = 0.0;
		return pressure;
	}

	virtual void GetStylusInfo (TabletDeviceType *type, bool *is_inverted)
	{
		GetStylusInfoFromDevice (event->device, type, is_inverted);
	}

	virtual MoonModifier GetModifiers ()
	{
		g_assert_not_reached ();
	}

private:
	GdkEventMotion *event;
};

class MoonCrossingEventGtk : public MoonCrossingEvent {
public:
	MoonCrossingEventGtk (GdkEvent *event)
	{
		this->event = (GdkEventCrossing*)gdk_event_copy (event);
	}

	virtual ~MoonCrossingEventGtk ()
	{
		gdk_event_free ((GdkEvent*)event);
	}

	virtual MoonEvent* Clone ()
	{
		return new MoonCrossingEventGtk ((GdkEvent*)event);
	}

	virtual gpointer GetPlatformEvent ()
	{
		return event;
	}

	virtual Point GetPosition ()
	{
		return Point (event->x, event->y);
	}

	virtual double GetPressure ()
	{
		return 0.0;
	}

	virtual void GetStylusInfo (TabletDeviceType *type, bool *is_inverted)
	{
	}

	virtual MoonModifier GetModifiers ()
	{
		g_assert_not_reached ();
	}
	
	virtual bool IsEnter ()
	{
		return event->type == GDK_ENTER_NOTIFY;
	}

private:
	GdkEventCrossing *event;
};

class MoonFocusEventGtk : public MoonFocusEvent {
public:
	MoonFocusEventGtk (GdkEvent *event)
	{
		this->event = (GdkEventFocus*)gdk_event_copy (event);
	}

	virtual ~MoonFocusEventGtk ()
	{
		gdk_event_free ((GdkEvent*)event);
	}

	virtual MoonEvent *Clone ()
	{
		return new MoonFocusEventGtk ((GdkEvent*)event);
	}

	virtual gpointer GetPlatformEvent()
	{
		return event;
	}

	virtual bool IsIn ()
	{
		return event->in != 0;
	}

private:
	GdkEventFocus *event;
};

class MoonScrollWheelEventGtk : public MoonScrollWheelEvent {
public:
	MoonScrollWheelEventGtk (GdkEvent *event)
	{
		this->event = (GdkEventScroll*)gdk_event_copy (event);
	}

	virtual ~MoonScrollWheelEventGtk ()
	{
		gdk_event_free ((GdkEvent*)event);
	}

	virtual MoonEvent* Clone ()
	{
		return new MoonScrollWheelEventGtk ((GdkEvent*)event);
	}

	virtual gpointer GetPlatformEvent ()
	{
		return event;
	}

	virtual Point GetPosition ()
	{
		return Point (event->x, event->y);
	}

	virtual double GetPressure ()
	{
		double pressure = 0.0;
		if (!event->device || !gdk_event_get_axis ((GdkEvent*)event, GDK_AXIS_PRESSURE, &pressure))
			pressure = 0.0;
		return pressure;
	}

	virtual void GetStylusInfo (TabletDeviceType *type, bool *is_inverted)
	{
		GetStylusInfoFromDevice (event->device, type, is_inverted);
	}

	virtual MoonModifier GetModifiers ()
	{
		g_assert_not_reached ();
	}

#define MOON_SCROLL_WHEEL_DELTA 10

	virtual int GetWheelDelta ()
	{
		/* we only handle UP/DOWN scroll events for the time being */
		switch (event->direction) {
		case GDK_SCROLL_UP:
			return MOON_SCROLL_WHEEL_DELTA;
		case GDK_SCROLL_DOWN:
			return -MOON_SCROLL_WHEEL_DELTA;

		default:
		case GDK_SCROLL_LEFT:
		case GDK_SCROLL_RIGHT:
			return 0;
		}
	}

private:
	GdkEventScroll *event;
};

/// our windowing system

cairo_surface_t *
MoonWindowingSystemGtk::CreateSurface ()
{
	// FIXME...
	g_assert_not_reached ();
}

MoonWindow *
MoonWindowingSystemGtk::CreateWindow (bool fullscreen, int width, int height, MoonWindow *parentWindow, Surface *surface)
{
	MoonWindowGtk *gtkwindow = new MoonWindowGtk (fullscreen, width, height, parentWindow, surface);
	RegisterWindow (gtkwindow);
	return gtkwindow;
}

MoonWindow *
MoonWindowingSystemGtk::CreateWindowless (int width, int height, PluginInstance *forPlugin)
{
	// FIXME...
	g_assert_not_reached ();
}

// older gtk+ (like 2.8 used in SLED10) don't support icon-less GTK_MESSAGE_OTHER
#ifndef GTK_MESSAGE_OTHER
#define GTK_MESSAGE_OTHER	GTK_MESSAGE_INFO
#endif

int
MoonWindowingSystemGtk::ShowMessageBox (const char *caption, const char *text, int buttons)
{
	if (!caption || !text)
		return MESSAGE_BOX_RESULT_NONE;

	// NOTE: this dialog is displayed even WITHOUT any user action
	//if (!Deployment::GetCurrent ()->GetSurface ()->IsUserInitiatedEvent ())
	//	return MESSAGE_BOX_RESULT_NONE;

	GtkButtonsType bt = buttons == MESSAGE_BOX_BUTTON_OK ? GTK_BUTTONS_OK : GTK_BUTTONS_OK_CANCEL;

	GtkWidget *widget = gtk_message_dialog_new (NULL,
						GTK_DIALOG_MODAL,
						GTK_MESSAGE_OTHER,
						bt,
						text);

	gtk_window_set_title (GTK_WINDOW (widget), caption);
	
	gint result = gtk_dialog_run (GTK_DIALOG (widget));
	gtk_widget_destroy (widget);

	switch (result) {
	case GTK_RESPONSE_OK:
		return MESSAGE_BOX_RESULT_OK;
	case GTK_RESPONSE_CANCEL:
		return MESSAGE_BOX_RESULT_CANCEL;
	case GTK_RESPONSE_YES:
		return MESSAGE_BOX_RESULT_YES;
	case GTK_RESPONSE_NO:
		return MESSAGE_BOX_RESULT_NO;
	case GTK_RESPONSE_NONE:
	default:
		return MESSAGE_BOX_RESULT_NONE;
	}
}

void
MoonWindowingSystemGtk::RegisterWindow (MoonWindow *window)
{
}

void
MoonWindowingSystemGtk::UnregisterWindow (MoonWindow *window)
{
}

guint
MoonWindowingSystemGtk::AddTimeout (gint priority, gint ms, MoonSourceFunc timeout, gpointer data)
{
	return g_timeout_add_full (priority, ms, (GSourceFunc)timeout, data, NULL);
}

void
MoonWindowingSystemGtk::RemoveTimeout (guint timeoutId)
{
	g_source_remove (timeoutId);
}

MoonIMContext*
MoonWindowingSystemGtk::CreateIMContext ()
{
	return new MoonIMContextGtk ();
}

MoonEvent*
MoonWindowingSystemGtk::CreateEventFromPlatformEvent (gpointer platformEvent)
{
	GdkEvent *gdk = (GdkEvent*)platformEvent;

	switch (gdk->type) {
	case GDK_MOTION_NOTIFY: {
		GdkEventMotion *mev = (GdkEventMotion*)gdk;
		if (mev->is_hint) {
#if GTK_CHECK_VERSION(2,12,0)
			if (gtk_check_version (2, 12, 0)) {
				gdk_event_request_motions (mev);
			}
			else
#endif
			{
				int ix, iy;
				GdkModifierType state;
				gdk_window_get_pointer (mev->window, &ix, &iy, (GdkModifierType*)&state);
				mev->x = ix;
				mev->y = iy;
			}    
		}

		return new MoonMotionEventGtk (gdk);
	}
	case GDK_BUTTON_PRESS:
	case GDK_2BUTTON_PRESS:
	case GDK_3BUTTON_PRESS:
	case GDK_BUTTON_RELEASE:
		return new MoonButtonEventGtk (gdk);

	case GDK_KEY_PRESS:
	case GDK_KEY_RELEASE:
		return new MoonKeyEventGtk (gdk);

	case GDK_ENTER_NOTIFY:
	case GDK_LEAVE_NOTIFY:
		return new MoonCrossingEventGtk (gdk);

	case GDK_FOCUS_CHANGE:
		return new MoonFocusEventGtk (gdk);

	case GDK_SCROLL:
		return new MoonScrollWheelEventGtk (gdk);
	default:
		printf ("unhandled gtk event %d\n", gdk->type);
		return NULL;
	}
}

guint
MoonWindowingSystemGtk::GetCursorBlinkTimeout (MoonWindow *window)
{
	GdkScreen *screen;
	GtkWidget *widget;
	GtkSettings *settings;
	guint timeout;

	if (!(widget = GTK_WIDGET (window->GetPlatformWindow ())))
		return CURSOR_BLINK_TIMEOUT_DEFAULT;
	
	if (!(screen = gdk_drawable_get_screen (widget->window)))
		return CURSOR_BLINK_TIMEOUT_DEFAULT;
	
	if (!(settings = gtk_settings_get_for_screen (screen)))
		return CURSOR_BLINK_TIMEOUT_DEFAULT;
	
	g_object_get (settings, "gtk-cursor-blink-time", &timeout, NULL);
	
	return timeout;
}
