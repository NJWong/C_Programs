void decode_rlefile(char **argv);
void get_dimensions(FILE *rlefile, int *dimensions);
void bypass_id_string (FILE *rlefile);
void get_width(FILE *rlefile, char *width_string);
void get_height(FILE *rlefile, char *height_string);
void decompress_and_store_key_frame_data(FILE *rlefile, unsigned char *key_frame_data, int width, int height);
void separate_channel_values(unsigned char *key_frame_data, unsigned char *red_frame_data, unsigned char *green_frame_data, unsigned char *blue_frame_data, int image_pixels);
void send_frame_to_ppm(int width, int height, unsigned char *red_frame_data, unsigned char *green_frame_data, unsigned char *blue_frame_data, char *prefix,  int frame_counter);
void send_frame_to_stdout(int width, int height, unsigned char *red_frame_data, unsigned char *green_frame_data, unsigned char *blue_frame_data);