#ifndef READSB_EXPORT_H
#define READSB_EXPORT_H

#if defined __GNUC__
#if __GNUC__ >= 4
#define __READSB_EXPORT __attribute__((visibility("default")))
#define __READSB_IMPORT __attribute__((visibility("default")))
#else
#define __READSB_EXPORT
#define __READSB_IMPORT
#endif
#endif

#ifndef libreadsb_STATIC
#ifdef libreadsb_EXPORTS
#define READSB_API __READSB_EXPORT
#else
#define READSB_API __READSB_IMPORT
#endif
#else
#define READSB_API
#endif
#endif /* READSB_EXPORT_H */