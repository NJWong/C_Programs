/** 
* @file decompress.h
* @brief This file contains prototypes for functions that decompress RLEplay 
* files.
*
* @author Robyn A. McNamara
* @date April 2016
*/

#include <stdio.h>
#include <stdint.h>

/**
* @brief Represents a RLEplay file
*/
typedef struct {
	FILE *fp; 	/// pointer into the @c .rle file
	int cols;	/// width of the image in pixels
	int rows;	/// height of the image in pixels
	int pixels;	/// number of pixels per frame

} RLEFile;


// Constants
/// byte offset for red channel
#define RED 0

/// byte offset for green channel
#define GREEN 1

/// byte offset for blue channel
#define BLUE 2


/**
 *
 * @brief Decompresses the file passed in as @p infile.
 *
 * If @p output_basename is non-null, it is interpreted as the desired 
 * basename for output PPMs; otherwise, the decompressed data is sent to 
 * @c stdout with the integer -1  between frames. 
 *
 * If output is to be stored to PPMs, they will be named 
 *  <output_basename>-0001.ppm, <output_basename>-0002.ppm, etc.
 *
 * @param infile Pointer to the input file, which has been opened but not validated
 * @param output_basename Desired basename for output PPMs, or NULL for output to @c stdout
 * 
 */
void decompress_file(FILE* infile, char* output_basename);


/** 
 * @brief Sets the appropriate byte in @param buffer, given the @p colour and 
 * @p pixcount.  
 * 
 * Assumes @c RED, @c GREEN, and @c BLUE are appropriately defined.
 *
 * @param buffer Array containing pixel values in 8-bit RGB format
 * @param colour One of @c RED, @c GREEN, or @c BLUE
 * @param pixcount Index of the pixel whose channel is to be set
 */
void set_channel_value(uint8_t* buffer, uint8_t colour, int, uint8_t pixcount);

/** 
 * @brief Decodes a single channel into @p buffer.
 *
 * The buffer should exist before this function is called.  To fully
 * create the image, @c decode_channel() must be called on the red, green,
 * and blue channels, passing the same buffer in at each call.  Once this
 * has been done, @p buffer will contain a stream of 24-bit RGB pixels, 
 * suitable for writing to a binary PPM.
 *
 * @param infile Reference to the RLEFile struct containing the input stream
 * @param colour One of @c RED, @c GREEN, or @c BLUE
 * @param buffer Buffer in which the channel will be set
 */
void decode_channel(RLEFile* infile, uint8_t colour, uint8_t*);
