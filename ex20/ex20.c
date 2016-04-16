/**
 * @file ex20.c
 * @brief debug macro test
 * @author peter liu <peter.liu.work@gmail.com.tw>
 * @version 0.0
 * @date 2016-04-04
 */

#include "dbg.h"
#include <stdlib.h>
#include <stdio.h>

void test_debug(void)
{
    // notice you don't need the \n
    debug( "I have black hair." );

    // pass argument as printf
    debug( "I am %d years old.", 37 );
}


void test_log_err()
{
    Log_err( "I believe everything is broken." );
    Log_err( "There are %d problems in %s.", 22, "space" );
}

void test_log_warn()
{
    Log_warn( "You can safely ignore this." );
    Log_warn( "Maybe consider looking at: %s.", "/etc/passwd" );
}

void test_log_info()
{
    Log_info( "Well i did something mundance." );
    Log_info( "It happened %f times today.", 1.3f );
}

int test_check( char * fileName )
{
    FILE *input = NULL;
    char *block = NULL;

    block = malloc( 100 );
    check_mem( block ); // should work

    input = fopen( fileName, "r" );
    check( input, "Failed to open %s.", fileName );

    free( block );
    fclose( input );
    return 0;

error:
    if( block ) free( block );
    if( input ) fclose( input );
    return -1;
}

int test_sentinel( int code )
{
    char *temp = malloc( 100 );
    check_mem( temp );

    switch( code ) {
        case 1:
            Log_info( "It works." );
            break;

        default:
            sentinel( "I shouldn't run." );

    }

    free( temp );
    return 0;

error:
    if( temp ) free( temp );
    return -1;
}

int test_check_mem()
{
    char *test = NULL;
    check_mem( test );

    free( test );
    return 0;

error:
    return -1;
}

int test_check_debug()
{
    int i = 0;
    check_debug( i != 0, "Oops, I was 0." );

    return 0;

error:
    return -1;
}


/**
 * @brief main
 *
 * @param argc
 * @param argv
 *
 * @return
 */
/* ----------------------------------------------------------------------------*/
int main( int argc, char ** argv )
{
    check( argc == 2, "Need an argument." );

    test_debug();
    test_log_err();
    test_log_warn();
    test_log_info();

    check( test_check( "ex20.c" ) == 0, "failed with ex20.c." );
    check( test_check( argv[1] ) == 0, "faile with argv." );
    check( test_sentinel( 1 ) == 0, "test_sentinel failed ( 1 )." );
    check( test_sentinel( 100 ) == 0, "test_sentinel failed ( 100 )." );
    check( test_check_mem() == 0, "test_check_mem failed." );
    check( test_check_debug() == 0, "test_check_debug failed." );



    return 0;

error:
    return 1;
}
