int parse_arguments(int argc, char **argv);
int rleplay_file_exists(char *filename);
int handle_arg1(char *filename);
int handle_arg2(char *arg2);
int handle_arg3(char *arg3);
int handle_arg4(char *arg4);
void output_to_files();
void output_to_stdout();
int is_valid_prefix(char *prefix);
char * create_path(char *folder, char *filename);