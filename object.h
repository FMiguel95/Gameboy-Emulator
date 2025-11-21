#ifndef OBJECT_H
# define OBJECT_H

# include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	u8 y_pos;
	u8 x_pos;
	u8 tile_index;
	u8 attributes;
} object_t;

typedef enum {
	ATTRIBUTE_0,	// CGB palette
	ATTRIBUTE_1,	// CGB palette
	ATTRIBUTE_2,	// CGB palette
	ATTRIBUTE_3,	// Bank
	ATTRIBUTE_4,	// DMG palette
	ATTRIBUTE_5,	// X flip
	ATTRIBUTE_6,	// Y flip
	ATTRIBUTE_7		// Priority
} ATTRIBUTE_bit;

#ifdef __cplusplus
}
#endif

#endif
