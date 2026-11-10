/*
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright (C) 1993-2026 Shannon Smith
 *
 * Private Roman calendar formatting helpers.
 */

#ifndef CALENDAR_CALENDAR_ANCIENT_H
#define CALENDAR_CALENDAR_ANCIENT_H

#include "calendar-engine-types.h"
#include "calendar-types.h"

G_BEGIN_DECLS

gchar *calendar_roman_number(gint64 value);
gchar *calendar_roman_format_date(
    const CalendarCalendarFields *fields);

G_END_DECLS

#endif
