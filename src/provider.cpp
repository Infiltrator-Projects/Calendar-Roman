// SPDX-License-Identifier: GPL-3.0-or-later
/* One native Roman chronology exposed through the stable Common calendar ABI. */
#include <infiltratr/core.h>
#include <infiltratr/temporal_calendar.h>
#include "calendar-custom.h"
#include "calendar-engine-types.h"
#include "julian-day.h"
#include "catalogue.h"
#include <string.h>
#include <type_traits>

static_assert(std::is_standard_layout_v<InfiltratrTemporalCalendarFields>);

static bool native_fields(int64_t jdn, CalendarCalendarFields *out)
{
    if (!out || !calendar_julian_jdn_representable(jdn))
        return false;
    calendar_custom_fields_from_jdn(RESOURCE_MODE, jdn, out);
    return true;
}

static bool fields(const InfiltratrTemporalCalendarProvider *,
                   int64_t jdn, InfiltratrTemporalCalendarFields *result)
{
    CalendarCalendarFields value;
    if (!result || !native_fields(jdn, &value))
        return false;
    *result = InfiltratrTemporalCalendarFields{
        value.year, value.month, value.day, value.auxiliary,
        value.special != FALSE
    };
    return true;
}

static bool format(const InfiltratrTemporalCalendarProvider *,
                   int64_t jdn, InfiltratrTemporalCalendarDatePart part,
                   char *buffer, size_t capacity, size_t *length)
{
    CalendarCalendarFields value;
    if (!buffer || capacity == 0 ||
        part < INFILTRATR_TEMPORAL_CALENDAR_DATE_PART_DAY ||
        part > INFILTRATR_TEMPORAL_CALENDAR_DATE_PART_FULL ||
        !native_fields(jdn, &value))
        return false;

    gchar *text = calendar_custom_format(RESOURCE_MODE, &value,
                                        (CalendarDatePart)part);
    if (!text || !*text) {
        g_free(text);
        return false;
    }
    const size_t size = strlen(text);
    if (size >= capacity) {
        g_free(text);
        return false;
    }
    memcpy(buffer, text, size + 1U);
    if (length) *length = size;
    g_free(text);
    return true;
}

static bool start(const InfiltratrTemporalCalendarProvider *,
                  int64_t jdn, int64_t *result)
{
    CalendarCalendarFields checked;
    if (!result || !native_fields(jdn, &checked))
        return false;
    const int64_t candidate = calendar_custom_month_start(RESOURCE_MODE, jdn);
    if (!native_fields(candidate, &checked))
        return false;
    *result = candidate;
    return true;
}

static bool navigate(int64_t jdn, int32_t amount, int64_t *result,
                     bool by_year)
{
    CalendarCalendarFields checked;
    if (!result || !native_fields(jdn, &checked))
        return false;
    const int64_t candidate = by_year
        ? calendar_custom_add_years(RESOURCE_MODE, jdn, amount)
        : calendar_custom_add_months(RESOURCE_MODE, jdn, amount);
    if (!native_fields(candidate, &checked))
        return false;
    *result = candidate;
    return true;
}

static bool periods(const InfiltratrTemporalCalendarProvider *,
                    int64_t jdn, int32_t amount, int64_t *result)
{ return navigate(jdn, amount, result, false); }
static bool years(const InfiltratrTemporalCalendarProvider *,
                  int64_t jdn, int32_t amount, int64_t *result)
{ return navigate(jdn, amount, result, true); }

static const InfiltratrTemporalResourceMetadata metadata = {
    sizeof(InfiltratrTemporalResourceMetadata),
    INFILTRATR_TEMPORAL_RESOURCE_METADATA_ABI,
    INFILTRATR_TEMPORAL_RESOURCE_CALENDAR,
    RESOURCE_ID, INFILTRATR_TEMPORAL_DISPLAY_NAME, false, false, false
};
static const InfiltratrTemporalCalendarProvider provider = {
    sizeof(InfiltratrTemporalCalendarProvider),
    INFILTRATR_TEMPORAL_CALENDAR_PROVIDER_ABI,
    RESOURCE_ID, fields, format, start, periods, years
};

#if defined(_WIN32)
#define RESOURCE_EXPORT __declspec(dllexport)
#else
#define RESOURCE_EXPORT __attribute__((visibility("default")))
#endif
extern "C" RESOURCE_EXPORT const InfiltratrTemporalResourceMetadata *
infiltratr_temporal_resource_metadata_v1(void);
extern "C" RESOURCE_EXPORT const InfiltratrTemporalCalendarProvider *
infiltratr_temporal_calendar_provider_v1(void);
extern "C" RESOURCE_EXPORT const InfiltratrTemporalResourceMetadata *
infiltratr_temporal_resource_metadata_v1(void) { return &metadata; }
extern "C" RESOURCE_EXPORT const InfiltratrTemporalCalendarProvider *
infiltratr_temporal_calendar_provider_v1(void) { return &provider; }
