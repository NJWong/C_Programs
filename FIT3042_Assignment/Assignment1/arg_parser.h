void parse_arguments(int argc, char **argv);
int rleplay_file_exists(char *filename);
void handle_arg1(char *filename);
void handle_arg2(char *arg2);
void handle_arg3(char *arg3);
void handle_arg4(char *arg4);
void output_to_files();
void output_to_stdout();
int is_valid_prefix(char *prefix);