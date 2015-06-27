/* ERPWindow_prefs.h
 *
 * Copyright (C) 2013,2014,2015 Paul Boersma
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

prefs_begin (ERPWindow)
	// overridden:
		prefs_add_bool   (ERPWindow, showSelectionViewer,            1, true)
		prefs_add_enum_with_data   (ERPWindow, scalp_colourScale,              1, kGraphics_colourScale, BLUE_TO_RED)
		prefs_add_enum   (ERPWindow, sound_scalingStrategy,          1, kTimeSoundEditor_scalingStrategy, DEFAULT)
		prefs_add_double (ERPWindow, sound_scaling_height,           1, U"20e-6")
		prefs_add_double (ERPWindow, sound_scaling_minimum,          1, U"-10e-6")
		prefs_add_double (ERPWindow, sound_scaling_maximum,          1, U"10e-6")
		prefs_add_double (ERPWindow, picture_bottom,                 1, U"0.0")
		prefs_add_double (ERPWindow, picture_top,                    1, U"0.0 (= auto)")
		prefs_add_bool   (ERPWindow, spectrogram_show,               1, false);
		prefs_add_double (ERPWindow, spectrogram_viewFrom,           1, U"0.0")   // Hz
		prefs_add_double (ERPWindow, spectrogram_viewTo,             1, U"60.0")   // Hz
		prefs_add_double (ERPWindow, spectrogram_windowLength,       1, U"0.5")   // seconds
		prefs_add_double (ERPWindow, spectrogram_dynamicRange,       1, U"40.0")   // dB
		prefs_add_long   (ERPWindow, spectrogram_timeSteps,          1, U"1000")
		prefs_add_long   (ERPWindow, spectrogram_frequencySteps,     1, U"250")
		prefs_add_enum   (ERPWindow, spectrogram_method,             1, kSound_to_Spectrogram_method, DEFAULT)
		prefs_add_enum   (ERPWindow, spectrogram_windowShape,        1, kSound_to_Spectrogram_windowShape, DEFAULT)
		prefs_add_bool   (ERPWindow, spectrogram_autoscaling,        1, true)
		prefs_add_double (ERPWindow, spectrogram_maximum,            1, U"100.0")   // dB/Hz
		prefs_add_double (ERPWindow, spectrogram_preemphasis,        1, U"0.0")   // dB/octave
		prefs_add_double (ERPWindow, spectrogram_dynamicCompression, 1, U"0.0")
		prefs_add_bool   (ERPWindow, spectrogram_picture_garnish,    1, true)
prefs_end (ERPWindow)

/* End of file ERPWindow_prefs.h */
