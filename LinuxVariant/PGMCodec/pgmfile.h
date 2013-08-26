/*
 * Description of header for files containing pgm images
 */
#ifdef __cplusplus
extern "C" {
#endif
#pragma once
struct pgmfile {
	int	pgm_width;		/* width (pixels) of image */
	int	pgm_height;		/* height (pixels) of image */
	int	pgm_depth;		/* depth (8) of pixel */
};
#ifdef __cplusplus
}
#endif
