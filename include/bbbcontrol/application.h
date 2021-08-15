#ifndef APPLICATION_H
#define APPLICATION_H

#include <thread>

#include "fawkes/console/console.h"
#include "fawkes/console/command_console.h"
#include "fawkes/console/command_help.h"
#include "fawkes/core/command/command_debug.h"
#include "fawkes/core/command/command_handler.h"
#include "fawkes/core/debug.h"
#include "fawkes/core/incbin.h"
#include "fawkes/http/server.h"
#include "fawkes/http/client.h"

class Application
{
public:
    Application();
    ~Application();

    void run();

    void serveBundle( Fawkes::Http::Request *request
                    , Fawkes::Http::Response *response );
    void serveIndex( Fawkes::Http::Request *request
                   , Fawkes::Http::Response *response );
    void serveApi( Fawkes::Http::Request *request
                 , Fawkes::Http::Response *response );

private:
    Fawkes::Console *mConsole;
    Fawkes::Debug mDebug;
    Fawkes::CommandHandler mCommandHandler;
    Fawkes::Http::Server mServer;
    Fawkes::Http::Router mRouter;
    
    Fawkes::CommandConsole mCommandConsole;
    Fawkes::CommandDebug mCommandDebug;
    Fawkes::CommandHelp mCommandHelp;
};

#endif // APPLICATION_H
