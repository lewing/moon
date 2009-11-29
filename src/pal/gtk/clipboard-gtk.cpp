/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */

#include "pal-gtk.h"

#include "clipboard-gtk.h"

#include "runtime.h"
#include "window-gtk.h"

MoonClipboardGtk::MoonClipboardGtk (MoonWindowGtk *win, MoonClipboardType clipboardType)
{
	GtkWidget *widget = GTK_WIDGET (win->GetPlatformWindow ());
	GdkWindow *window = GDK_WINDOW (widget->window);
	GdkDisplay *display = gdk_drawable_get_display (GDK_DRAWABLE (window));

	GdkAtom gdk_type;

	switch (clipboardType) {
	case MoonClipboard_Clipboard: gdk_type = GDK_SELECTION_CLIPBOARD; break;
	case MoonClipboard_Primary:   gdk_type = GDK_SELECTION_PRIMARY; break;
	}

	if (display)
		clipboard = gtk_clipboard_get_for_display (display, gdk_type);
	else
		clipboard = NULL;
}

void
MoonClipboardGtk::SetSelection (const char *text, int length)
{
}

void
MoonClipboardGtk::SetText (const char *text, int length)
{
	gtk_clipboard_set_text (clipboard, text, length);
}

class AsyncClosure {
public:
	AsyncClosure (MoonClipboard *clipboard, MoonClipboardGetTextCallback cb, gpointer data) : clipboard (clipboard), cb (cb), cb_data (data) { }
	MoonClipboard *clipboard;
	MoonClipboardGetTextCallback cb;
	gpointer cb_data;
};

void
MoonClipboardGtk::async_get_text (GtkClipboard *gtk_clipboard, const char *text, gpointer data)
{
	AsyncClosure *closure = (AsyncClosure*)data;
	MoonClipboard *clipboard = closure->clipboard;
	MoonClipboardGetTextCallback cb = closure->cb;
	gpointer cb_data = closure->cb_data;

	delete closure;

	cb (clipboard, text, cb_data);
}

void
MoonClipboardGtk::AsyncGetText (MoonClipboardGetTextCallback cb, gpointer data)
{
	AsyncClosure *closure = new AsyncClosure (this, cb, data);
	gtk_clipboard_request_text (clipboard, async_get_text, closure);
}

char*
MoonClipboardGtk::GetText ()
{
	return gtk_clipboard_wait_for_text (clipboard);
}