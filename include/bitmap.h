#ifndef Bitmap_h
#define Bitmap_h

struct bitmap_t {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */
  unsigned char	 pixel_data[32 * 32 * 3 + 1];
};

#endif  /* _Bitmap_h */
