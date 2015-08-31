/**
 * @mainpage Scribo Server
 *
 * A word processor with encrypt and decrypt functions
 *
 * You are able to create or load files
 *
 * Format the text whatever you want
 *
 * Administrate the server with the server software
 *
 * @author Rune Krauss
 *
 * @since 2015
 *
 * @copyright (c) by Rune Krauss
 *
 * @version 0.1
 */

/**
 * @file main.cpp
 * The main window is loaded and main settings are set
 */

#include <QCoreApplication>
#include "scriboserver.h"

/**
 * Start point of the program which executes the core application
 * @param argc Slots of command line parameter
 * @param argv Values of command line parameter
 * @return The executing main window
 */
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Create an instance of the server and then start it.
    ScriboServer server;
    server.startServer();

    return a.exec();
}
