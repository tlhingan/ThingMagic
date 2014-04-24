/** @file configUtils.c
 *  @brief Actual implementation of the two functions in the header.
 *
 * This file imports the data stored in the config.app file which contains
 * all the configuration data for the TMR application. This inlcudes:
 * The location of the file
 * An IP the data will be sent to.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "configUtils.h"

// if using the 'main' function below for testing, uncomment this define
// and use the proper location of the config.app file.
#ifdef DEBUG
#define CONFIG_FILE "./config.app"
#endif

int getPathFromConfig ( char* configLocation, ConfigDetails* details )
{
    FILE *fp;
    int counter = 0;
    size_t length = 0;

#ifdef DEBUG
    printf ( "Just before copying filepath\n" );
#endif

    details->FILEPATH = configLocation;

#ifdef DEBUG
    printf ( "just opening the file handle\n" );
    printf ( "File path is %s\n", details->FILEPATH );
#endif

    fp = fopen ( details->FILEPATH, "r" );
    details->buf[0] = '\0';
    if ( fp == NULL )
        return 0;
    while ( fgets ( details->buf, BUFFERSIZE, fp ) != NULL ) {
        if ( details->buf[0] == '#' ){

	    
	  continue;
	} 
#ifdef DEBUG
        printf ( "Buf is %s and the length is %zd\n", details->buf, strlen ( details->buf ) );
#endif


        length = strlen ( details->buf );
        strip_newline ( details->buf, length );
        length = strlen ( details->buf );
#ifdef DEBUG
        printf ( "Buf is %s and the length is %zd\n", details->buf, length );
        printf ( "Counter is %d\n", counter );
        printf ( "details->buf is %zd chars long \n", strlen ( details->buf ) );
#endif

        details->pPROPERTIES[counter] = malloc ( length + 1 );
        if ( !details->pPROPERTIES[counter] ) return -1;
#ifdef DEBUG
        printf ( "About to assign dynamic memory and assigning \n" );
#endif
        strncpy ( details->pPROPERTIES[counter], details->buf, length+1);
        if ( counter == 3 )
            details->POWERLEVEL = atoi ( details->pPROPERTIES[counter] );
#ifdef DEBUG
        printf ( "pProperties[%d] is %s\n", counter, details->pPROPERTIES[counter] );
#endif
        counter++;
#ifdef DEBUG
        printf ( "Bottom of loop\n" );
#endif
    }

#ifdef DEBUG

    printf ( "ReaderID2 is: %s!\n", details->pPROPERTIES[0] );
    printf ( "IP2 is: %s\n", details->pPROPERTIES[1] );
    printf ( "URI2 is: %s\n", details->pPROPERTIES[2] );
    printf ( "URI2 is %zd chars long\n", strlen( details->pPROPERTIES[2] ) );
    printf ( "Powerlevel2 is: %s\n", details->pPROPERTIES[3] );
    printf ( "PowerLevel2 as an integer is: %d\n", details->POWERLEVEL );
    printf ( "We're using the reader2 located at: %s\n", details->pPROPERTIES[4] );
    printf ( "Closing the file pointer\n" );
    printf ( "Size of the pointer array: %zd\n", sizeof ( details->pPROPERTIES ) );
#endif

    fclose ( fp );
    return 1;
}

void configFreeAllocated(ConfigDetails* details)
{
    size_t i;
    for ( i = 0; i < sizeof ( details->pPROPERTIES ) / 8; ++i ) {
        free ( details->pPROPERTIES[i] );
        details->pPROPERTIES[i] = NULL;
    }
}

void strip_newline ( char* str, int size )
{
    int i;

    for ( i=0; i < size; i++ ) {
        if ( str[i] == '\n' ) {
            str[i] = '\0';

#ifdef DEBUG
            printf ( "Newline found!!!\n" );
#endif
            return;
        }
    }
}

#ifdef DEBUG
int main ( void )
{
    char* configFile = CONFIG_FILE;
	ConfigDetails details; 
    printf ( "configFile contains %s\n", configFile );
    printf ( "Going into getPathFromConfig\n" );
    printf ( "%d is returned\n", getPathFromConfig ( configFile, &details ) );
    configFreeAllocated(&details);
    return 0;
}
#endif
