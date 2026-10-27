# Ancient Roman Calendar Notation

**InfiltratorOS calendar resource** · **Stable ID:** `roman` · **Module:** `Ancient Roman Calendar Notation.so`

Kalends, Nones and Ides date naming over a proleptic Julian calendar.

**Historical scope/accuracy:** This is not the pre-Julian Roman absolute calendar.

**Intended installation:** `/System/Resources/Calendars/Ancient Roman Calendar Notation.so` (case-sensitive)

The provider owns fields, localized formatting, period starts and navigation. Field extraction determines the supported date domain; every other operation must reject the same invalid input. Switching calendar presentation cannot rewrite canonical instants or event ordering.

## Status

**Source copied and build verified.** This repository contains the implementation of `roman`, the supporting code it needs, a one-provider `Makefile` and focused regression fixtures. The copy is based on Common 1.19.78 at `8d60662114e3`; the original Common source remains unchanged.

Run `make` to produce `build/Ancient Roman Calendar Notation.so` and `make check` to run this provider's checks. No Common checkout or Common archive is required. The current combined Common package and its installation ownership remain in place; this copy does not introduce an independent installer or release.

See [source provenance and build details](SOURCE.md), [source manifest](SOURCE-MANIFEST.json) and [recorded validation](VALIDATION-RESULTS.json).

## Documentation

- [Model and evidence](docs/MODEL.md)
- [Architecture and ownership](docs/ARCHITECTURE.md)
- [Validation requirements](docs/VALIDATION.md)
- [Roadmap](docs/ROADMAP.md)
- [Documentation index](docs/README.md)

## Existing authoritative sources

- [Libraries/Common temporal sources](https://github.com/Infiltrator-Projects/Libraries/tree/main/resources/temporal)
- [Current canonical build catalogue](https://github.com/Infiltrator-Projects/Libraries/blob/main/resources/temporal/Makefile)
- [Calendar chronology and clock models](https://github.com/Infiltrator-Projects/Calendar/blob/main/docs/MODELS.md)
- [Historical fidelity audit](https://github.com/Infiltrator-Projects/Libraries/blob/main/docs/TEMPORAL_HISTORICAL_FIDELITY.md)
- [Temporal discovery and executable provider ABI](https://github.com/Infiltrator-Projects/Libraries/blob/main/docs/TEMPORAL_PROVIDERS.md)
