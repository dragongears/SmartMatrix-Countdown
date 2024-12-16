#ifndef Gencon_h
#define Gencon_h

extern const struct bitmap_t bitmap_image;

struct bitmap_t {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */
  unsigned char	 pixel_data[32 * 32 * 3 + 1];
};

#endif  /* _Gencon_h */
