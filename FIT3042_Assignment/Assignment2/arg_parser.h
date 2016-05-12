/************************************************************
* Desc   : Function that checks whether the command line
*          arguments are valid or not.
*
* Params : argc - the number of given command line arguments
*          argv - an array of the command line arguments
*           
* Return : 0 for success, -1 for failure.
************************************************************/
int validate_args(int argc, char **argv);

/************************************************************
* Desc    : Function that checks if there is the correct
*           number of arguments passed through the command line.
*
*           Minimum is 1 - the delay (in ms) between frames.
*           Maximum is 4 - to specify brightness, contrast,
*                          and saturation.
*
* Params  : argc - the number of given command line arguments
*           argv - an array of the command line arguments
*           
* Return  : 0 for success, -1 for failure.
************************************************************/
int check_number_of_args(int argc);


/************************************************************
* Desc    : Function that validates an argument based on the
*           assignment specification. 
*
* Params  : arg_index - the index of the arg to validate.
*           argv - the list of command line arguments.
*           
* Return  : 0 for success, -1 for failure.
************************************************************/
int validate_arg(int arg_index, char **argv);