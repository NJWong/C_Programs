int rledecode(int argc, char **argv);
int validate_args(int argc, char **argv);
int check_number_of_args(int argc);
int parse_arg(int arg_index, char *arg);
int handle_arg1(char *filepath);
int rleplay_file_exists(char *filepath);
int handle_arg2(char *arg2);
int is_valid_prefix(char *prefix);
int handle_arg3(char *arg3);
int handle_arg4(char *arg4);
void decode_rlefile(char **argv, int to_ppm);
void get_dimensions(FILE *rlefile, int *dimensions);
void bypass_id_string (FILE *rlefile);
void get_width(FILE *rlefile, char *width_string);
void get_height(FILE *rlefile, char *height_string);
void decompress_and_store_key_frame_data(FILE *rlefile, unsigned char *key_frame_data, int width, int height);
void separate_channel_values(unsigned char *key_frame_data,
                             unsigned char *red_frame_data,
                             unsigned char *green_frame_data,
                             unsigned char *blue_frame_data,
                             int image_pixels);
void send_frame_to_ppm(int width, int height,
                       unsigned char *red_frame_data,
                       unsigned char *green_frame_data,
                       unsigned char *blue_frame_data,
                       char *prefix,  int frame_counter);
void send_frame_to_stdout(int width, int height,
                          unsigned char *red_frame_data,
                          unsigned char *green_frame_data,
                          unsigned char *blue_frame_data);