#ifndef _Simple_extensions_h_
#define _Simple_extensions_h_
/* Simple_extensions.h
 *
 * Copyright (C) 1994-2011 David Weenink
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 djmw 19950616
 djmw 20020812 GPL header
*/

#include "Data.h"
#include "Graphics.h"
#include "Simple.h"

void SimpleString_init (SimpleString me, const char32 *value);
/* return 0 when value == NULL */

const char32 *SimpleString_c (SimpleString me);
/* return pointer to the string */

int SimpleString_compare (SimpleString me, SimpleString thee);

void SimpleString_append (SimpleString me, SimpleString thee);
void SimpleString_append_c (SimpleString me, const char32 *str);
/* append string to me */

SimpleString SimpleString_concat (SimpleString me, SimpleString thee);
SimpleString SimpleString_concat_c (SimpleString me, const char32 *str);
/* concatenate two strings */

void SimpleString_replace_c (SimpleString me, const char32 *replacement);
/* replace my value with new string */

long SimpleString_length (SimpleString me);
/* return my length */

void SimpleString_draw (SimpleString me, Graphics g, double xWC, double yWC);
/* draw the string */

const char32 * SimpleString_nativize_c (SimpleString me, int educateQuotes);
const char32 * SimpleString_genericize_c (SimpleString me);
/* see longchar.h for info */

#endif /* _Simple_extensions_h_ */
