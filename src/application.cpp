#include "bbbcontrol/application.h"

INCBIN( index_html, "bbbcontrol/resources/index.html" );
INCBIN( bundle_js, "bbbcontrol/resources/bundle.js" );

/**
 * @brief Constructor
 */
Application::Application()
    : mConsole( &Fawkes::Console::getInstance() )
{
    mDebug.setLevel( LOG_INFO );

    // Bind server route handlers
    auto index  = BIND_ROUTE( &Application::serveIndex );
    auto bundle = BIND_ROUTE( &Application::serveBundle );
    auto api    = BIND_ROUTE( &Application::serveApi );

    // Add route handlers to a router
    mRouter.addRoute( "/", MHD_HTTP_METHOD_GET, index );
    mRouter.addRoute( "/index.html", MHD_HTTP_METHOD_GET, index );
    mRouter.addRoute( "/bundle.js", MHD_HTTP_METHOD_GET, bundle );
    mRouter.addRoute( "/api", MHD_HTTP_METHOD_POST, api );

    // Configure the server with the router
    mServer.applyRouter( mRouter );

    mCommandHandler.addCommand( &mCommandConsole );
    mCommandHandler.addCommand( &mCommandDebug );
    mCommandHandler.addCommand( &mCommandHelp );
}

/**
 * @brief Destructor
 */
Application::~Application()
{

}

void Application::run()
{
    Fawkes::Http::Client client;

    // Create a console instance and provide a client for use by the server
    mConsole->applyClient( &client );

    mServer.listen();
    std::thread *appThread = new std::thread( &Fawkes::Console::run, mConsole );

    appThread->join();
}

/**
 * @brief Serve up the index page
 */
void Application::serveIndex( Fawkes::Http::Request *request
                            , Fawkes::Http::Response *response )
{
    (void)request;
    response->applyStatus( Fawkes::Http::OK );
    response->addHeader( MHD_HTTP_HEADER_CONTENT_TYPE, Fawkes::Http::Server::type_text_html );
    response->send( res_index_html_data, res_index_html_size );
    LOG_INFO( "Index page sent" );
}

/**
 * @brief Serve up the javascript bundle
 */
void Application::serveBundle( Fawkes::Http::Request *request 
                             , Fawkes::Http::Response *response )
{
    (void)request;
    response->applyStatus( Fawkes::Http::OK );
    response->addHeader( MHD_HTTP_HEADER_CONTENT_TYPE, Fawkes::Http::Server::type_text_html );
    response->send( res_bundle_js_data, res_bundle_js_size );
    LOG_INFO( "Index page sent" );

}

/**
 * @brief Handles incoming requests to the API
 */
void Application::serveApi( Fawkes::Http::Request *request
                          , Fawkes::Http::Response *response )
{
    char *responseArray = nullptr;
    int32_t value = mCommandHandler.process( request->body()->data(), &responseArray );

    if( responseArray == nullptr ) {
        LOG_WARN( "Command response is NULL" );
    } else {
        // Http::Response rsp( request->connection() );
        response->applyStatus( Fawkes::Http::OK );
        response->addHeader( MHD_HTTP_HEADER_CONTENT_TYPE, Fawkes::Http::Server::type_text_html );
        response->send( responseArray, strlen( responseArray ) );
    }

    if( responseArray ) {
        free( responseArray );
    }
}
