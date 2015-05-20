#ifndef DRAW_H_
#define DRAW_H_

#define OFF    0
#define ON     1
#define TOGGLE 2

#define UP     0
#define DOWN   1

#define LEFT   0
#define RIGHT  1

#define MIN   0
#define MOUT  1

/* -------------------------- Cube specific stuff -------------------------- */
/* ------------------------------------------------------------------------- */
#define   CUBE_ROWS 8
#define CUBE_LAYERS 8

#define cube_clear() cube_fill(0x00, cube_display_buffer)

/* --------------------------------- Layer --------------------------------- */
/* ------------------------------------------------------------------------- */
#define LAYERS_MASK 0x0F     /* Layers mask */
enum LAYERS{
  LAYER_1 = 0, LAYER_2, LAYER_3, LAYER_4, LAYER_5, LAYER_6, LAYER_7, LAYER_8, 
  ENDOF_LAYERS
};


/* --------------------------------- Row ----------------------------------- */
/* ------------------------------------------------------------------------- */
#define ROW_MASK 0xFF
enum ROWS {
	ROW_1 = 0, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7, ROW_8,
	ENDOF_ROWS
};


/* --------------------------------- Plane --------------------------------- */
/* ------------------------------------------------------------------------- */
enum PLANES {
  PLANE_1 = 0, PLANE_2, PLANE_3, PLANE_4, PLANE_5, PLANE_6, PLANE_7, PLANE_8,
  ENDOF_PLANES
};

/* Plane axis macros */
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2
          
#endif /*DRAW_H_*/
