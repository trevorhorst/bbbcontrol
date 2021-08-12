/** ****************************************************************************
 * @file main.cpp
 * @author Trevor Horst
 * @copyright
 * @brief Entry point for the Fawkes Library example application
 * ****************************************************************************/
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>

#include "fawkes/core/option_parser.h"
#include "fawkes/core/json/cjson.h"
#include "fawkes/core/logger/log.h"
#include "fawkes/core/network/unix/unix_client.h"
#include "fawkes/core/network/unix/unix_server.h"
#include "fawkes/core/command/command_handler.h"
#include "fawkes/core/command/command_test.h"
#include "fawkes/core/command/command_debug.h"
#include "fawkes/core/command/command_env.h"

#include "fawkes/console/console.h"
#include "fawkes/console/command_console.h"
#include "fawkes/console/command_help.h"

#include "fawkes/http/server.h"
#include "fawkes/http/client.h"
#include "fawkes/http/router.h"

#include "bbbcontrol/application.h"
#include "bbbcontrol/resources/resources.h"

enum OptionIndex {
    UNKNOWN  = 0
    , HELP
    , NUM_ARGUMENTS
};

const Option::Descriptor usage[] = {
    { UNKNOWN, 0, "" , "", Option::Arg::None, "USAGE: FawkesLibExample [options]\n\n"
                                               "Options:" },
    { HELP, 0, "h" , "help", Option::Arg::None, "  --help  \tPrint usage and exit." },
    { 0, 0, nullptr, nullptr, nullptr, nullptr }
};

void testResources()
{
    LOG_INFO( "%s", Resources::index_html );
}

int main( int argc, char *argv[] )
{
    argc -= ( argc > 0 );
    argv += ( argc > 0 );

    Option::Stats stats( usage, argc, argv );
    std::vector< Option::Option > options( stats.options_max );
    std::vector< Option::Option > buffer( stats.buffer_max );
    Option::Parser parse( usage, argc, argv, &options[ 0 ], &buffer[ 0 ] );

    if( parse.error() ) {
        // Print error and return early with error
        LOG_ERROR( "Unable to parse optional arguments" );
        return 1;
    }

    if( options[ HELP ] ) {
        // Print usage and return early
        Option::printUsage( std::cout, usage );
        return 0;
    }

    Application app;
    app.run();

    return 0;
}
