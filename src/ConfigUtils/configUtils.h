/*! @file configUtils.h
 * @brief A few functions to deal with configuration reading
 *
 * This library will allow a configuration file to be
 * specified that can be used in an application and read in using
 * dynamically allocated memory. It is quite specific to this 
 * application so if you but would make a great starting place for
 * additions made for other applications. 
 *
 * @author SAIT RADLab 
 * @bug No known bugs at this time
 * 
 */

#ifndef CONFIG_UTILS_SEEN
#define CONFIG_UTILS_SEEN

#define BUFFERSIZE 128


/*!@struct ConfigDetails
 * @brief This structure will hold all the data read from file.
 * @var ConfigDetails::URI
 * This is the section of the web address that goes between
 * @var ConfigDetails::IP
 * The IP address and the tag list propegated at the end.
 * @var ConfigDetails:READERID
 * The Reader Identifier eg SqueezeChuteReader
 * @var ConfigDetails::POWERLEVEL
 * Integer representation of the power level to set the reader to. Min: 500 Max 3000
 * @var ConfigDetails::buf
 * This is a temporary string buffer for strings to be brought in
 * @var ConfigDetails::pPROPERTIES
 * An array of pointers to store the strings which are coming in from the config.app file
 * @var ConfigDetails::FILEPATH
 * String The filepath that the config.app file is located at. 
 */
struct ConfigDetails
{
  char URI[256];
  char IP[32];
  char READERID[64];
  int POWERLEVEL;
  char buf[BUFFERSIZE];
  char* pPROPERTIES[10];
  char* FILEPATH;
};

/*! @typedef ConfigDetails
 * @brief Data Structure to contain all structures used in functions
 *
 * I'm hoping this shows up because I'm losing my patience
 * in oh so many ways. How much time can I devote to this before i
 * makes me do bad things to people?
 */
typedef struct ConfigDetails ConfigDetails;
/*! @brief Container for data read in from config.app file.
 *
 * Thi container will hold all the data required by the TMR app for
 * setting up and running.
 *
 * @sa ConfigDetails
 */
ConfigDetails details;

/*! @brief Just a line of text.
 * Function to read in data from config file
 *
 * This function will receive a relative location in the file system of a text file
 * containing the information required for it's proper execution.
 * @param configLocation Location of the config.app file in the file system.
 * @return Integer value of 0 for success, or -1 for error
 */
int getPathFromConfig(char* configLocation, ConfigDetails* details);
/*! @brief Frees dynamically allocated memory from structures
 *
 * As the brief says, just some freeing of memory going on here.
 * Nothing else to see. Move along, move along.
 */
void configFreeAllocated(ConfigDetails* details);

void strip_newline(char* str, int size);

#endif
