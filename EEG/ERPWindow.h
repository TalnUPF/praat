#ifndef _ERPWindow_h_
#define _ERPWindow_h_
/* ERPWindow.h
 *
 * Copyright (C) 2012,2013,2014,2015 Paul Boersma
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "SoundEditor.h"
#include "ERP.h"

Thing_define (ERPWindow, SoundEditor) { public:
	const char32 * v_getChannelName (long channelNumber)
		override {
			ERP erp = (ERP) our data;
			return erp -> channelNames [channelNumber];
		}
	void v_drawSelectionViewer ()
		override;
	bool v_hasPitch ()
		override { return false; }
	bool v_hasIntensity ()
		override { return false; }
	bool v_hasFormants ()
		override { return false; }
	bool v_hasPulses ()
		override { return false; }
	void v_prefs_addFields (EditorCommand cmd)
		override;
	void v_prefs_setValues (EditorCommand cmd)
		override;
	void v_prefs_getValues (EditorCommand cmd)
		override;

	#include "ERPWindow_prefs.h"
};

/**
	Create an ERPWindow.
*/
ERPWindow ERPWindow_create (const char32 *title, ERP data);

/* End of file ERPWindow.h */
#endif
