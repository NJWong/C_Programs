int validate_args(int argc, char **argv);
int check_number_of_args(int argc);
int parse_arg(int arg_index, char **argv);
int handle_arg1(char *filepath);
int rleplay_file_exists(char *filepath);
int handle_arg2(char *arg2);
int is_valid_prefix(char *prefix);
int handle_arg3(char *arg3, char *factor);
// int handle_arg4(char *arg4, char *factor);