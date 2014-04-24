/*! \file TMReader.c
 * \brief A UHF tag reading appplication.
 *
 * This uses the Mercury API from ThingMagic to operate on one of their M6E readers
 * deployed within an embedded device.
 *
 */

#include <tm_reader.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <malloc.h>
#include <time.h>
#include "Socket_Comms/socket_comms.h"
#include "ConfigUtils/configUtils.h"


#define CONFIG_PATH "./config.app"
#define VERSION_NUM "0.90 ALPHA"
// ConfigDetails is a structure created within the configUtils files
ConfigDetails details; /*!< Brought in from configUtils.h */
TMR_Reader r;
FILE* logFile;
int EXITSIGNAL = 0;
/**
 * Signal handler for this application
 */
void sigHandle ( int signal )
{
    TMR_stopReading ( &r );
    TMR_destroy ( &r );
    printf ( "\nFreeing memory and closing gracefully...\n" );
    //configFreeAllocated(&details);
    fclose ( logFile );
	EXITSIGNAL = 1;
    //exit ( 0 );
}
void errx ( int exitval, const char *fmt, ... )
{
    va_list ap;

    va_start ( ap, fmt );
    vfprintf ( stderr, fmt, ap );

    exit ( exitval );
}

void checkerr ( TMR_Reader* rp, TMR_Status ret, int exitval, const char *msg )
{
    if ( TMR_SUCCESS != ret ) {
        errx ( exitval, "Error %s: %s\n", msg, TMR_strerr ( rp, ret ) );
    }
}

void serialPrinter ( bool tx, uint32_t dataLen, const uint8_t data[],
                     uint32_t timeout, void *cookie )
{
    FILE *out = cookie;
    uint32_t i;

    fprintf ( out, "%s", tx ? "Sending: " : "Received:" );
    for ( i = 0; i < dataLen; i++ ) {
        if ( i > 0 && ( i & 15 ) == 0 )
            fprintf ( out, "\n		   " );
        fprintf ( out, " %02x", data[i] );
    }
    fprintf ( out, "\n" );
}

void stringPrinter ( bool tx,uint32_t dataLen, const uint8_t data[],uint32_t timeout, void *cookie )
{
    FILE *out = cookie;

    fprintf ( out, "%s", tx ? "Sending: " : "Received:" );
    fprintf ( out, "%s\n", data );
}

void timestamp(FILE* output){
  time_t ltime;
  ltime = time(NULL);
  fprintf(output,"%s ", asctime(localtime(&ltime)));
}
void callback ( TMR_Reader *reader, const TMR_TagReadData *t, void *cookie );
void exceptionCallback ( TMR_Reader *reader, TMR_Status error, void *cookie );

int main ( int argc, char *argv[] )
{

#ifndef TMR_ENABLE_BACKGROUND_READS
    errx ( 1, "This sample requires background read functionality.\n"
           "Please enable TMR_ENABLE_BACKGROUND_READS in tm_config.h\n"
           "to run this codelet\n" );
    return -1;
#else

    TMR_Status ret;
    TMR_Region region;
    TMR_TransportListenerBlock tb;
    TMR_ReadListenerBlock rlb;
    TMR_ReadExceptionListenerBlock reb;
    int tempCounter = 0, readerTemp = 0;
    int gPath = 0;

    logFile = fopen ( "TMRLog.log", "a" );
    fprintf(logFile, "Opening logfile at ");
    timestamp(logFile);
    fprintf(logFile, "\n");
    //Install Signal Handler
    signal ( SIGINT, sigHandle );
    //Use configUtils package for dealing with
    // config.app data.
    gPath = getPathFromConfig ( CONFIG_PATH, &details );
    if ( gPath != 1 ) {
        printf ( "Config.app file not found! Please create or move a copy to this location.\n" );
        printf ( "This app will now close.\n" );
        exit ( -1 );
    }
    if ( argv[1] != NULL && strcmp ( argv[1], "-v" ) == 0 ) {
        printf ( "Version is %s\n", VERSION_NUM );
        exit ( 1 );
    }
    if ( details.pPROPERTIES[4] == NULL )
        //if (argc < 2)
    {
        errx ( 1, "Please provide reader URL, such as:\n"
               "tmr:///com4\n"
               "tmr://my-reader.example.com\n" );
    }

    // original line: ret = TMR_create(rp, argv[1]);
    ret = TMR_create ( &r, details.pPROPERTIES[4] );
    checkerr ( &r, ret, 1, "creating reader" );

    if ( TMR_READER_TYPE_SERIAL == r.readerType ) {
        tb.listener = serialPrinter;
    } else {
        tb.listener = stringPrinter;
    }
    tb.cookie = stdout;
#if 0
    TMR_addTransportListener ( rp, &tb );
#endif

    ret = TMR_connect ( &r );
    checkerr ( &r, ret, 1, "connecting reader" );

    int readPowerToSet = details.POWERLEVEL;
    ret = TMR_paramSet ( &r, TMR_PARAM_RADIO_READPOWER,&readPowerToSet );
    checkerr ( &r, ret, 1, "setting read power" );
    TMR_GEN2_TagEncoding myTagEnc;
    TMR_GEN2_Tari myTari;
    TMR_SR_GEN2_QType myQType;
    TMR_SR_GEN2_QStatic myQStatic;
    TMR_SR_GEN2_Q myTestQ;

    myQType = TMR_SR_GEN2_Q_STATIC;
    myQStatic.initialQ = 4;
    myTestQ.u.staticQ = myQStatic;
    myTestQ.type = myQType;

    myTagEnc = TMR_GEN2_MILLER_M_8;
    myTari = TMR_GEN2_TARI_6_25US;


    ret = TMR_paramSet ( &r, TMR_PARAM_GEN2_TAGENCODING, &myTagEnc );
    checkerr ( &r, ret, 1, "setting tagEncodingString" );
    ret = TMR_paramSet ( &r, TMR_PARAM_GEN2_TARI, &myTari );
    checkerr ( &r, ret, 1, "setting Tari" );
    ret = TMR_paramSet ( &r, TMR_PARAM_GEN2_Q, &myTestQ );
    checkerr ( &r, ret, 1, "Setting Gen2_Q" );
	int powersaveon = 0;
	ret = TMR_paramSet(&r, TMR_PARAM_RADIO_ENABLEPOWERSAVE, &powersaveon);
	checkerr(&r, ret, 1, "Setting Powersave to True");
    // Set a read plan that can be adhered to by the reader in order to ensure that
    // all antennaes get a chance to read.

    //configFreeAllocated();
    region = TMR_REGION_NONE;
    ret = TMR_paramGet ( &r, TMR_PARAM_REGION_ID, &region );
    checkerr ( &r, ret, 1, "getting region" );

    if ( TMR_REGION_NONE == region ) {
        TMR_RegionList regions;
        TMR_Region _regionStore[32];
        regions.list = _regionStore;
        regions.max = sizeof ( _regionStore ) /sizeof ( _regionStore[0] );
        regions.len = 0;

        ret = TMR_paramGet ( &r, TMR_PARAM_REGION_SUPPORTEDREGIONS, &regions );
        checkerr ( &r, ret, __LINE__, "getting supported regions" );

        if ( regions.len < 1 ) {
            checkerr ( &r, TMR_ERROR_INVALID_REGION, __LINE__, "Reader doesn't supportany regions" );
        }
        region = regions.list[0];
        ret = TMR_paramSet ( &r, TMR_PARAM_REGION_ID, &region );
        checkerr ( &r, ret, 1, "setting region" );
    }

    rlb.listener = callback;
    rlb.cookie = NULL;

    reb.listener = exceptionCallback;
    reb.cookie = NULL;

    ret = TMR_addReadListener ( &r, &rlb );
    checkerr ( &r, ret, 1, "adding read listener" );

    ret = TMR_addReadExceptionListener ( &r, &reb );
    checkerr ( &r, ret, 1, "adding exception listener" );


    while ( EXITSIGNAL != 1 ) {
        ret = TMR_startReading ( &r );
        checkerr ( &r, ret, 1, "starting reading" );
        //sleep(2);
        ret = TMR_stopReading ( &r );
        //printf("Stopping reading\n");
        checkerr ( &r, ret, 1, "stopping reading" );
        tempCounter++;
        if ( tempCounter < 100 ) {
            TMR_paramGet ( &r, TMR_PARAM_RADIO_TEMPERATURE, &readerTemp );
            printf ( "Temp on reader is %d\n",readerTemp );
			if(readerTemp > 70){
				fprintf(stdout,"WARNING: Temp > 70 Celsius. Sleeping to prevent damage.");
				fprintf(logFile,"WARNING: Temp > 70 Celsius. Sleeping to prevent damage.");
				sleep(5);
			}
            tempCounter = 0;
        }
        sleep ( 0.2 );
    }
    TMR_destroy ( &r );
    configFreeAllocated(&details);
    return 0;

#endif /* TMR_ENABLE_BACKGROUND_READS */
}

void
callback ( TMR_Reader *reader, const TMR_TagReadData *t, void *cookie )
{
    char epcStr[128];
    char uri[256]= "GET http://\0";
    char temp[90];
    int RSSI = 0;
    int socket = 0;

    TMR_bytesToHex ( t->tag.epc, t->tag.epcByteCount, epcStr );
    RSSI = t->rssi;
    int antenna = t->antenna - 1;
    sprintf ( temp, "%d", antenna );

    /**
     * So we're going to reorganize this area. We should build the output
     * string in the correct order so that we're not confusing anyone, most
     * importantly ourselves.
     *
     * pPROPERTIES 0 = Reader ID
     * pPROPERTIES 1 = IP Address to send to
     * pPROPERTIES 2 = partial URL that will be appended (eg /alma/read?)
     * pPROPERTIES 3 = Power Level
     * pPROPERTIES 4 = reader location 
     */
    strcat (uri, details.pPROPERTIES[1]);
    strcat (uri, details.pPROPERTIES[2]);
    strcat (uri, details.pPROPERTIES[0]);
    strcat (uri, "&antenna=");
    strcat (uri, temp);
    strcat (uri, "&TagNumber=");
    strcat (uri, epcStr);
    strcat (uri, "\r\n");
    printf ( "%s\n", uri );

    timestamp(logFile);
    fprintf ( logFile, uri );
    socket = open_socket ( details.pPROPERTIES[1],"80" );
    send_data ( socket, uri );
    close ( socket );
}


void
exceptionCallback ( TMR_Reader *reader, TMR_Status error, void *cookie )
{
    fprintf ( stdout, "Error:%s\n", TMR_strerr ( reader, error ) );
}

