/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * textblock.cpp: 
 *
 * Contact:
 *   Moonlight List (moonlight-list@lists.ximian.com)
 *
 * Copyright 2007 Novell, Inc. (http://www.novell.com)
 *
 * See the LICENSE file included with the distribution for details.
 */

#include <config.h>

#include <cairo.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include "deployment.h"
#include "textblock.h"
#include "geometry.h"
#include "control.h"
#include "runtime.h"
#include "color.h"
#include "utils.h"
#include "debug.h"
#include "enums.h"
#include "uri.h"

namespace Moonlight {

// Unicode Line Separator (\u2028)
static const char utf8_linebreak[3] = { 0xe2, 0x80, 0xa8 };
#define utf8_linebreak_len 3


//
// TextBlock
//

TextBlock::TextBlock ()
{
	SetObjectType (Type::TEXTBLOCK);
	
	font = new TextFontDescription ();
	
	downloaders = g_ptr_array_new ();
	layout = new TextLayout ();
	font_resource = NULL;
	source = NULL;
	
	actual_height = 0.0;
	actual_width = 0.0;
	setvalue = true;
	was_set = false;
	dirty = true;
}

TextBlock::~TextBlock ()
{
	CleanupDownloaders (true);
	g_ptr_array_free (downloaders, true);
	
	delete layout;
	delete font;
}

bool
TextBlock::InsideObject (cairo_t *cr, double x, double y)
{
	double nx = x, ny = y;
	Size total = GetRenderSize ().Max (GetActualWidth (), GetActualHeight ());
	total = total.Max (ApplySizeConstraints (total));

	TransformPoint (&nx, &ny);

	if (nx < 0 || ny < 0 || nx > total.width || ny > total.height)
		return false;

	return InsideLayoutClip (x, y) && InsideClip (cr, x, y);
}

void
TextBlock::CleanupDownloaders (bool all)
{
	Downloader *downloader;
	guint i;
	
	for (i = 0; i < downloaders->len; i++) {
		downloader = (Downloader *) downloaders->pdata[i];
		
		if (all || downloader != source) {
			downloader->RemoveHandler (Downloader::CompletedEvent, downloader_complete, this);
			downloader->Abort ();
			downloader->unref ();
		}
	}
	
	g_ptr_array_set_size (downloaders, 0);
	
	if (source && !all) {
		g_ptr_array_add (downloaders, source);
	} else {
		source = NULL;
	}
	
	if (all) {
		delete font_resource;
		font_resource = NULL;
	}
}

void
TextBlock::AddFontSource (Downloader *downloader)
{
	downloader->AddHandler (downloader->CompletedEvent, downloader_complete, this);
	g_ptr_array_add (downloaders, downloader);
	downloader->ref ();
	
	if (downloader->Started () || downloader->Completed ()) {
		if (downloader->Completed ())
			DownloaderComplete (downloader);
	} else {
		// This is what actually triggers the download
		downloader->Send ();
	}
}

void
TextBlock::SetFontSource (Downloader *downloader)
{
	char *resource_id;
	
	CleanupDownloaders (true);
	source = downloader;
	
	if (downloader) {
		resource_id = downloader->GetUri ()->ToString ((UriToStringFlags) (UriHidePasswd | UriHideQuery | UriHideFragment));
		font_resource = new FontResource (resource_id);
		AddFontSource (downloader);
		g_free (resource_id);
		return;
	}
	
	UpdateFontDescriptions (true);
	UpdateBounds (true);
	Invalidate ();
	dirty = true;
}

void
TextBlock::AddFontResource (const char *resource)
{
	FontManager *manager = Deployment::GetCurrent ()->GetFontManager ();
	Application *application = Application::GetCurrent ();
	Downloader *downloader;
	char *path;
	Uri *uri;
	
	uri = new Uri ();
	
	if (!application || !uri->Parse (resource) || !(path = application->GetResourceAsPath (GetResourceBase(), uri))) {
		if (IsAttached () && (downloader = GetDeployment ()->CreateDownloader ())) {
			downloader->Open ("GET", resource, FontPolicy);
			AddFontSource (downloader);
			downloader->unref ();
		}
		
		delete uri;
		
		return;
	}
	
	manager->AddResource (resource, path);
	g_free (path);
	delete uri;
}

void
TextBlock::Render (cairo_t *cr, Region *region, bool path_only)
{
	cairo_save (cr);
	ApplyTransform (cr);
	
	if (!path_only)
		RenderLayoutClip (cr);
		
	Paint (cr);
	
	cairo_restore (cr);
}

void
TextBlock::ComputeBounds ()
{
	extents = layout->GetRenderExtents ();
	Thickness padding = *GetPadding ();
	
	extents.x += padding.left;
	extents.y += padding.top;

	extents_with_children = extents;
	
        bounds = IntersectBoundsWithClipPath (extents.GrowBy (effect_padding), false).Transform (&absolute_xform);
	bounds_with_children = bounds;

	ComputeGlobalBounds ();
	ComputeSurfaceBounds ();
}

Point
TextBlock::GetTransformOrigin ()
{
	Point *user_xform_origin = GetRenderTransformOrigin ();
	Size xform_size = ApplySizeConstraints (GetRenderSize ());
	return Point (xform_size.width * user_xform_origin->x, 
		      xform_size.height * user_xform_origin->y);
}

Size
TextBlock::ComputeActualSize ()
{
	Thickness padding = *GetPadding ();
	Size constraint = ApplySizeConstraints (Size (INFINITY, INFINITY));
	Size result = Size (0.0, 0.0);

	if (ReadLocalValue (LayoutInformation::LayoutSlotProperty) || LayoutInformation::GetPreviousConstraint (this)) {
		layout->Layout ();
		layout->GetActualExtents (&actual_width, &actual_height);
	}  else {
		constraint = constraint.GrowBy (-padding);
		Layout (constraint);
	}
	
	result = Size (actual_width, actual_height);
	result = result.GrowBy (padding);
	
	return result;
};

Size
TextBlock::MeasureOverrideWithError (Size availableSize, MoonError *error)
{
	Thickness padding = *GetPadding ();
	Size constraint;
	Size desired;
	
	constraint = availableSize.GrowBy (-padding);
	
	Layout (constraint);
	
	desired = Size (actual_width, actual_height).GrowBy (padding);
	
	return desired;
}

Size
TextBlock::ArrangeOverrideWithError (Size finalSize, MoonError *error)
{
	Thickness padding = *GetPadding ();
	Size constraint;
	Size arranged;
	
	constraint = finalSize.GrowBy (-padding);
	Layout (constraint);
	
	arranged = Size (actual_width, actual_height);
	
	arranged = arranged.Max (constraint);
	layout->SetAvailableWidth (constraint.width);
	
	arranged = arranged.GrowBy (padding);
	
	return finalSize;
}

void
TextBlock::UpdateLayoutAttributes ()
{
	InlineCollection *inlines = GetInlines ();
	TextLayoutAttributes *attrs;
	const char *text;
	int length = 0;
	Inline *item;
	List *runs;
	
	InvalidateMeasure ();
	InvalidateArrange ();
	runs = new List ();
	
	UpdateFontDescription (false);
	
	if (inlines != NULL) {
		int inlines_count = inlines->GetCount ();
		for (int i = 0; i < inlines_count; i++) {
			item = inlines->GetValueAt (i)->AsInline ();
			item->UpdateFontDescription (font_resource, false);
			
 			switch (item->GetObjectType ()) {
			case Type::RUN:
				text = ((Run *) item)->GetText ();
				
				if (text && text[0]) {
					attrs = new TextLayoutAttributes ((ITextAttributes *) item, length);
					runs->Append (attrs);
					
					length += strlen (text);
				}
				
				break;
			case Type::LINEBREAK:
				attrs = new TextLayoutAttributes ((ITextAttributes *) item, length);
				runs->Append (attrs);
				
				length += utf8_linebreak_len;
				break;
			default:
				break;
			}
		}
		
		if (inlines_count > 0)
			was_set = true;
	}
	
	layout->SetText (GetText (), length);
	layout->SetTextAttributes (runs);
}

bool
TextBlock::UpdateFontDescription (bool force)
{
	FontFamily *family = GetFontFamily ();
	bool changed = false;
	
	if (font->SetResource (font_resource))
		changed = true;
	
	if (font->SetFamily (family ? family->source : NULL))
		changed = true;
	
	if (font->SetStretch (GetFontStretch ()->stretch))
		changed = true;
	
	if (font->SetWeight (GetFontWeight ()->weight))
		changed = true;
	
	if (font->SetStyle (GetFontStyle ()->style))
		changed = true;
	
	if (font->SetSize (GetFontSize ()))
		changed = true;
	
	if (font->SetLanguage (GetLanguage ()))
		changed = true;
	
	if (force) {
		font->Reload ();
		changed = true;
	}
	
	if (changed)
		layout->SetBaseFont (font->GetFont ());
	
	return changed;
}

bool
TextBlock::UpdateFontDescriptions (bool force)
{
	InlineCollection *inlines = GetInlines ();
	bool changed = false;
	Inline *item;
	
	changed = UpdateFontDescription (force);
	
	if (inlines != NULL) {
		int inlines_count = inlines->GetCount ();
		for (int i = 0; i < inlines_count; i++) {
			item = inlines->GetValueAt (i)->AsInline ();
			if (item->UpdateFontDescription (font_resource, force))
				changed = true;
		}
		
		if (changed)
			layout->ResetState ();
	}
	
	if (changed) {
		InvalidateMeasure ();
		InvalidateArrange ();
		UpdateBounds (true);
		dirty = true;
	}
	
	return changed;
}

void
TextBlock::Layout (Size constraint)
{
	if (was_set && !GetValueNoDefault (TextBlock::TextProperty)) {
		// If the Text property had been set once upon a time,
		// but is currently empty, Silverlight seems to set
		// the ActualHeight property to the font height. See
		// bug #405514 for details.
		TextFontDescription *desc = new TextFontDescription ();
		FontFamily *family = GetFontFamily ();
		TextFont *font;
		
		desc->SetFamily (family ? family->source : NULL);
		desc->SetStretch (GetFontStretch ()->stretch);
		desc->SetWeight (GetFontWeight ()->weight);
		desc->SetStyle (GetFontStyle ()->style);
		desc->SetSize (GetFontSize ());
		
		font = desc->GetFont ();
		actual_height = font->Height ();
		actual_width = 0.0;
		delete desc;
	} else if (!was_set) {
		// If the Text property has never been set, then its
		// extents should both be 0.0. See bug #435798 for
		// details.
		actual_height = 0.0;
		actual_width = 0.0;
	} else {
		layout->SetMaxWidth (constraint.width);
		layout->Layout ();
		
		layout->GetActualExtents (&actual_width, &actual_height);
	}
	
	dirty = false;
}

void
TextBlock::Paint (cairo_t *cr)
{
	Thickness *padding = GetPadding ();
	Point offset (padding->left, padding->top);
	
	ApplyTransform (cr);
	layout->Render (cr, GetOriginPoint (), offset);
}

char *
TextBlock::GetTextInternal (InlineCollection *inlines)
{
	const char *text;
	GString *block;
	Inline *item;
	char *str;
	
	if (!inlines)
		return g_strdup ("");
	
	block = g_string_new ("");
	
	int inlines_count = inlines->GetCount ();
	for (int i = 0; i < inlines_count; i++) {
		item = inlines->GetValueAt (i)->AsInline ();
		
		switch (item->GetObjectType ()) {
		case Type::RUN:
			text = ((Run *) item)->GetText ();
			
			if (text && text[0])
				g_string_append (block, text);
			break;
		case Type::LINEBREAK:
			g_string_append_len (block, utf8_linebreak, utf8_linebreak_len);
			break;
		default:
			break;
		}
	}
	
	str = block->str;
	g_string_free (block, false);
	
	return str;
}

void
TextBlock::SetTextInternal (const char *text)
{
	InlineCollection *inlines;
	Value *value;
	
	// Note: calling GetValue() may cause the InlineCollection to be
	// autocreated, so we need to prevent reentrancy here.
	setvalue = false;
	
	value = GetValue (TextBlock::InlinesProperty);
	inlines = value->AsInlineCollection ();
	
	if (text) {
		Deployment *deployment = Deployment::GetCurrent ();
		int count = inlines->Array ()->len;
		Run *run = NULL;
		
		if (count > 0 && (value = inlines->GetValueAt (0)) && value->Is (deployment, Type::RUN)) {
			// try reusing the existing run (only if it was autogenerated)
			run = value->AsRun ();
			if (run->GetAutogenerated ()) {
				// clear any additional inlines...
				while (count > 1) {
					inlines->RemoveAt (count - 1);
					count--;
				}
			} else {
				// can't reuse this one...
				run = NULL;
			}
		}
		
		if (run == NULL) {
			inlines->Clear ();
			run = new Run ();
			run->SetAutogenerated (true);
			inlines->Add (run);
			run->unref ();
		}
		
		run->SetText (text);
	} else {
		// setting text to null results in String.Empty
		inlines->Clear ();
		SetValue (TextBlock::TextProperty, Value (""));
	}
	
	setvalue = true;
}

void
TextBlock::OnPropertyChanged (PropertyChangedEventArgs *args, MoonError *error)
{
	bool invalidate = true;
	
	if (args->GetProperty ()->GetOwnerType () != Type::TEXTBLOCK) {
		FrameworkElement::OnPropertyChanged (args, error);
		
		if (args->GetId () == FrameworkElement::LanguageProperty) {
			// a change in xml:lang might change font characteristics
			if (UpdateFontDescriptions (false)) {
				InvalidateMeasure ();
				InvalidateArrange ();
				UpdateBounds (true);
				dirty = true;
			}
		}
		
		/*
		if (args->GetId () == FrameworkElement::WidthProperty) {
			//if (layout->SetMaxWidth (args->GetNewValue()->AsDouble ()))
			//	dirty = true;
			
			UpdateBounds (true);
		}
		*/
		return;
	}
	
	if (args->GetId () == TextBlock::FontFamilyProperty) {
		FontFamily *family = args->GetNewValue () ? args->GetNewValue ()->AsFontFamily () : NULL;
		char **families, *fragment;
		int i;
		
		CleanupDownloaders (false);
		
		if (family && family->source) {
			families = g_strsplit (family->source, ",", -1);
			for (i = 0; families[i]; i++) {
				g_strstrip (families[i]);
				if ((fragment = strchr (families[i], '#'))) {
					// the first portion of this string is the resource name...
					*fragment = '\0';
					AddFontResource (families[i]);
				}
			}
			g_strfreev (families);
		}
		
		if (UpdateFontDescriptions (false))
			dirty = true;
	} else if (args->GetId () == TextBlock::FontSizeProperty) {
		if (UpdateFontDescriptions (false))
			dirty = true;
	} else if (args->GetId () == TextBlock::FontStretchProperty) {
		if (UpdateFontDescriptions (false))
			dirty = true;
	} else if (args->GetId () == TextBlock::FontStyleProperty) {
		if (UpdateFontDescriptions (false))
			dirty = true;
	} else if (args->GetId () == TextBlock::FontWeightProperty) {
		if (UpdateFontDescriptions (false))
			dirty = true;
	} else if (args->GetId () == TextBlock::TextProperty) {
		if (setvalue) {
			// result of a change to the TextBlock.Text property
			const char *text = args->GetNewValue() ? args->GetNewValue()->AsString () : NULL;
			
			SetTextInternal (text);
			UpdateLayoutAttributes ();
			dirty = true;
		} else {
			// result of a change to the TextBlock.Inlines property
			UpdateLayoutAttributes ();
			invalidate = false;
		}
	} else if (args->GetId () == TextBlock::InlinesProperty) {
		if (setvalue) {
			// result of a change to the TextBlock.Inlines property
			InlineCollection *inlines = args->GetNewValue() ? args->GetNewValue()->AsInlineCollection () : NULL;
			
			setvalue = false;
			SetValue (TextBlock::TextProperty, Value (GetTextInternal (inlines), true));
			setvalue = true;
			
			UpdateLayoutAttributes ();
			dirty = true;
		} else {
			// this should be the result of Inlines being autocreated
			UpdateLayoutAttributes ();
			invalidate = false;
		}
	} else if (args->GetId () == TextBlock::LineStackingStrategyProperty) {
		dirty = layout->SetLineStackingStrategy (args->GetNewValue()->AsLineStackingStrategy ());
	} else if (args->GetId () == TextBlock::LineHeightProperty) {
		dirty = layout->SetLineHeight (args->GetNewValue()->AsDouble ());
	} else if (args->GetId () == TextBlock::TextDecorationsProperty) {
		dirty = true;
	} else if (args->GetId () == TextBlock::TextAlignmentProperty) {
		dirty = layout->SetTextAlignment (args->GetNewValue()->AsTextAlignment ());
	} else if (args->GetId () == TextBlock::TextTrimmingProperty) {
		dirty = layout->SetTextTrimming (args->GetNewValue()->AsTextTrimming ());
	} else if (args->GetId () == TextBlock::TextWrappingProperty) {
		dirty = layout->SetTextWrapping (args->GetNewValue()->AsTextWrapping ());
	} else if (args->GetId () == TextBlock::PaddingProperty) {
		dirty = true;
	} else if (args->GetId () == TextBlock::FontSourceProperty) {
		FontSource *fs = args->GetNewValue () ? args->GetNewValue ()->AsFontSource () : NULL;
		FontManager *manager = Deployment::GetCurrent ()->GetFontManager ();
		
		// FIXME: ideally we'd remove the old item from the cache (or,
		// rather, 'unref' it since some other textblocks/boxes might
		// still be using it).
		delete font_resource;
		font_resource = NULL;
		
		if (fs != NULL) {
			switch (fs->type) {
			case FontSourceTypeManagedStream:
				font_resource = manager->AddResource (fs->source.stream);
				break;
			case FontSourceTypeGlyphTypeface:
				font_resource = new FontResource (fs->source.typeface);
				break;
			}
		}
		
		UpdateFontDescriptions (true);
		dirty = true;
	}
	
	if (invalidate) {
		if (dirty) {
			InvalidateMeasure ();
			InvalidateArrange ();
			UpdateBounds (true);
		}

		Invalidate ();
	}
	
	NotifyListenersOfPropertyChange (args, error);
}

void
TextBlock::OnSubPropertyChanged (DependencyProperty *prop, DependencyObject *obj, PropertyChangedEventArgs *subobj_args)
{
	if (prop && prop->GetId () == TextBlock::ForegroundProperty) {
		Invalidate ();
	} else {
		FrameworkElement::OnSubPropertyChanged (prop, obj, subobj_args);
	}
}

void
TextBlock::OnCollectionChanged (Collection *col, CollectionChangedEventArgs *args)
{
	if (!PropertyHasValueNoAutoCreate (TextBlock::InlinesProperty, col)) {
		FrameworkElement::OnCollectionChanged (col, args);
		return;
	}
	
	InlineCollection *inlines = GetInlines ();
	
	if (args->GetChangedAction () == CollectionChangedActionClearing)
		return;
	
	if (!setvalue) {
		// changes being handled elsewhere...
		return;
	}
	
	setvalue = false;
	SetValue (TextBlock::TextProperty, Value (GetTextInternal (inlines), true));
	setvalue = true;
	
	UpdateLayoutAttributes ();
	InvalidateMeasure ();
	InvalidateArrange ();
	UpdateBounds (true);
	Invalidate ();
}

void
TextBlock::OnCollectionItemChanged (Collection *col, DependencyObject *obj, PropertyChangedEventArgs *args)
{
	if (!PropertyHasValueNoAutoCreate (TextBlock::InlinesProperty, col)) {
		FrameworkElement::OnCollectionItemChanged (col, obj, args);
		return;
	}
	
	InlineCollection *inlines = GetInlines ();
	
	if (args->GetId () != Inline::ForegroundProperty) {
		if (args->GetId () == Run::TextProperty) {
			// update our TextProperty
			setvalue = false;
			SetValue (TextBlock::TextProperty, Value (GetTextInternal (inlines), true));
			setvalue = true;
			
			UpdateLayoutAttributes ();
		} else {
			// likely a font property change...
			((Inline *) obj)->UpdateFontDescription (font_resource, true);
		}
		
		// All non-Foreground property changes require
		// recalculating layout which can change the bounds.
		InvalidateMeasure ();
		InvalidateArrange ();
		UpdateBounds (true);
		dirty = true;
	} else {
		// A simple Foreground brush change does not require
		// recalculating layout. Invalidate() and we're done.
	}
	
	Invalidate ();
}

void
TextBlock::downloader_complete (EventObject *sender, EventArgs *calldata, gpointer closure)
{
	((TextBlock *) closure)->DownloaderComplete ((Downloader *) sender);
}

void
TextBlock::DownloaderComplete (Downloader *downloader)
{
	FontManager *manager = Deployment::GetCurrent ()->GetFontManager ();
	char *resource, *filename;
	const char *path;
	Uri *uri;
	
	dirty = true;
	InvalidateMeasure ();
	InvalidateArrange ();
	
	// get the downloaded file path (enforces a mozilla workaround for files smaller than 64k)
	if (!(filename = downloader->GetDownloadedFilename (NULL)))
		return;
	
	g_free (filename);
	
	uri = downloader->GetUri ();
	
	// If the downloaded file was a zip file, this'll get the path to the
	// extracted zip directory, else it will simply be the path to the
	// downloaded file.
	if (!(path = downloader->GetUnzippedPath ()))
		return;
	
	resource = uri->ToString ((UriToStringFlags) (UriHidePasswd | UriHideQuery | UriHideFragment));
	manager->AddResource (resource, path);
	g_free (resource);
	
	if (UpdateFontDescriptions (true)) {
		dirty = true;
		
		UpdateBounds (true);
		Invalidate ();
	}
}

};
