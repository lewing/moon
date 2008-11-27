/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * binding.cpp: data binding
 *
 * Contact:
 *   Moonlight List (moonlight-list@lists.ximian.com)
 *
 * Copyright 2008 Novell, Inc. (http://www.novell.com)
 *
 * See the LICENSE file included with the distribution for details.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "binding.h"

char *
Binding::GetPropertyPath ()
{
	return this->property_path;	
}

void
Binding::SetPropertyPath (char *path)
{
	this->property_path = path;	
}

BindingMode
Binding::GetBindingMode ()
{
	return this->binding_mode;	
}

void
Binding::SetBindingMode (BindingMode mode)
{
	this->binding_mode = mode;
}

bool
Binding::GetIsSealed ()
{
	return this->is_sealed;	
}

void
Binding::SetIsSealed (bool sealed)
{
	this->is_sealed = sealed;
}

BindingExpressionBase::BindingExpressionBase ()
{
	binding = NULL;
	source = NULL;
	target = NULL;
	target_property = NULL;
}

BindingExpressionBase::~BindingExpressionBase ()
{
	delete binding;
}

void
BindingExpressionBase::SetBinding (Binding *binding)
{
	delete this->binding;
	this->binding = binding;
}

void
BindingExpressionBase::SetSource (FrameworkElement *element)
{
	// FIXME: do we want to ref the element? or at least listen for the destroy signal?
	this->source = element;
}

void
BindingExpressionBase::SetTarget (FrameworkElement *element)
{
	// We have to listen to property changes on the target
	this->target = element;	
}

void
BindingExpressionBase::AttachListener (PropertyChangeHandler handler, gpointer user_data)
{
	if (source && binding && binding->GetPropertyPath () && handler) {
		DependencyProperty *property = DependencyProperty::GetDependencyProperty (source->GetType ()->GetKind (), binding->GetPropertyPath ());
		if (property)
			source->AddPropertyChangeHandler (property, handler, user_data);
	}
}

void
BindingExpressionBase::DetachListener (PropertyChangeHandler handler)
{
	if (source && binding && binding->GetPropertyPath () && handler) {
		DependencyProperty *property = DependencyProperty::GetDependencyProperty (source->GetType ()->GetKind (), binding->GetPropertyPath ());
		if (property)
			source->RemovePropertyChangeHandler (property, handler);
	}
}

Value *
BindingExpressionBase::GetValue ()
{
	if (source && binding && binding->GetPropertyPath ()) {
		DependencyProperty *property = DependencyProperty::GetDependencyProperty (source->GetType ()->GetKind (), binding->GetPropertyPath ());
		if (property)
			return source->GetValue (property);
	}
	
	return NULL;
}

void
BindingExpressionBase::UpdateSource (Value *value)
{
	if (source && binding && binding->GetPropertyPath ()) {
		DependencyProperty *property = DependencyProperty::GetDependencyProperty (source->GetType ()->GetKind (), binding->GetPropertyPath ());
		if (property)
			source->SetValue (property, value);
	}
}
