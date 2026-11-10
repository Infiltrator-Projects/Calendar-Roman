// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 1993-2026 Shannon Smith
/*
 * Standalone Roman calendar notation over Julian dates.
 * This module owns one calendar; no generic provider registry or operation
 * table is needed between its chronology and the stable Common ABI adapter.
 */
#include "calendar-custom.h"
#include "calendar-helpers.h"
#include "calendar-ancient.h"
#include "julian-day.h"
#include <glib/gi18n-lib.h>

typedef CalendarCalendarMode CalendarMode;
typedef CalendarCalendarFields CalendarFields;

static const gchar *const roman_months[] = {
    "", N_("Ianuarius"), N_("Februarius"), N_("Martius"), N_("Aprilis"),
    N_("Maius"), N_("Iunius"), N_("Iulius"), N_("Augustus"),
    N_("September"), N_("October"), N_("November"), N_("December")
};

static bool valid_mode(CalendarMode mode)
{
    return mode == CALENDAR_CALENDAR_MODE_ROMAN;
}

static bool convert_fields(gint64 jdn, CalendarFields *fields)
{
    gint year, month, day;
    if (fields == NULL || !calendar_julian_jdn_representable(jdn))
        return false;
    calendar_jdn_to_julian(jdn, &year, &month, &day);
    *fields = CalendarFields{year, month, day, 0, FALSE};
    return true;
}

static gchar *format_date(const CalendarFields *fields, CalendarDatePart part)
{
    if (fields->month < 1 || fields->month > 12)
        return g_strdup("");
    if (part == CALENDAR_DATE_PART_DAY)
        return calendar_roman_number(fields->day);
    if (part == CALENDAR_DATE_PART_MONTH)
        return g_strdup(_(roman_months[fields->month]));
    if (part == CALENDAR_DATE_PART_YEAR)
        return calendar_roman_number(calendar_i64_add_saturating(fields->year, 753));
    return calendar_roman_format_date(fields);
}

void calendar_custom_fields_from_jdn(CalendarMode mode, gint64 jdn,
                                     CalendarFields *fields)
{
    g_return_if_fail(fields != NULL);
    *fields = CalendarFields{};
    if (valid_mode(mode))
        (void)convert_fields(jdn, fields);
}

gchar *calendar_custom_format(CalendarMode mode, const CalendarFields *fields,
                              CalendarDatePart part)
{
    if (!valid_mode(mode) || fields == NULL)
        return g_strdup("");
    return format_date(fields, part);
}

gint64 calendar_custom_month_start(CalendarMode mode, gint64 jdn)
{
    CalendarFields fields;
    if (!valid_mode(mode) || !convert_fields(jdn, &fields))
        return G_MAXINT64;
    return calendar_julian_to_jdn(fields.year, fields.month, 1);
}

gint64 calendar_custom_add_months(CalendarMode mode, gint64 jdn, gint amount)
{
    CalendarFields fields;
    if (!valid_mode(mode))
        return jdn;
    if (!convert_fields(jdn, &fields))
        return G_MAXINT64;

    const gint64 serial = calendar_i64_add_saturating(
        calendar_i64_add_saturating(
            calendar_i64_multiply_saturating(fields.year, 12),
            fields.month - 1), amount);
    fields.year = calendar_floor_divide(serial, 12);
    fields.month = (gint)calendar_positive_modulo(serial, 12) + 1;
    fields.day = MIN(fields.day,
                     calendar_julian_month_length(fields.year, fields.month));
    return calendar_julian_to_jdn(fields.year, fields.month, fields.day);
}

gint64 calendar_custom_add_years(CalendarMode mode, gint64 jdn, gint amount)
{
    CalendarFields fields;
    if (!valid_mode(mode))
        return jdn;
    if (!convert_fields(jdn, &fields))
        return G_MAXINT64;

    fields.year = calendar_i64_add_saturating(fields.year, amount);
    fields.day = MIN(fields.day,
                     calendar_julian_month_length(fields.year, fields.month));
    return calendar_julian_to_jdn(fields.year, fields.month, fields.day);
}
