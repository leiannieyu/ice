// **********************************************************************
//
// Copyright (c) 2001
// MutableRealms, Inc.
// Huntsville, AL, USA
//
// All Rights Reserved
//
// **********************************************************************

#include <Ice/Ice.h>
#include <AdminI.h>
#include <Forward.h>

using namespace std;
using namespace Ice;
using namespace IcePack;

void
usage(const char* n)
{
    cerr << "Usage: " << n << " [options]\n";
    cerr <<	
	"Options:\n"
	"-h, --help           Show this message.\n"
	"-v, --version        Display the Ice version.\n"
	"--nowarn             Don't print any security warnings.\n"
	;
}

int
run(int argc, char* argv[], CommunicatorPtr communicator, bool nowarn)
{
    PropertiesPtr properties = communicator->getProperties();

    string adminEndpoints = properties->getProperty("Ice.Adapter.Admin.Endpoints");
    if(adminEndpoints.length() != 0 && !nowarn)
    {
	cerr << argv[0] << ": warning: administrative endpoints `Ice.Adapter.Admin.Endpoints' enabled" << endl;
    }

    string forwardEndpoints = properties->getProperty("Ice.Adapter.Forward.Endpoints");
    if(forwardEndpoints.length() == 0)
    {
	cerr << argv[0] << ": `Ice.Adapter.Forward.Endpoints' property is not set" << endl;
	return EXIT_FAILURE;
    }

    AdminPtr admin = new AdminI(communicator);
    ObjectLocatorPtr forward = new Forward(admin);

    if (adminEndpoints.length() != 0)
    {
	ObjectAdapterPtr adminAdapter = communicator->createObjectAdapter("Admin");
	adminAdapter->add(admin, "admin");
	adminAdapter->activate();
    }

    ObjectAdapterPtr forwardAdapter = communicator->createObjectAdapter("Forward");
    forwardAdapter->setObjectLocator(forward);
    forwardAdapter->activate();

    communicator->waitForShutdown();
    return EXIT_SUCCESS;
}

int
main(int argc, char* argv[])
{
    PropertiesPtr properties = getDefaultProperties(argc, argv);

    bool nowarn = false;
    for (int i = 1; i < argc; ++i)
    {
	if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
	{
	    usage(argv[0]);
	    return EXIT_SUCCESS;
	}
	else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0)
	{
	    cout << ICE_STRING_VERSION << endl;
	    return EXIT_SUCCESS;
	}
	else if(strcmp(argv[i], "--nowarn") == 0)
	{
	    nowarn = true;
	}
	else
	{
	    cerr << argv[0] << ": unknown option `" << argv[i] << "'" << endl;
	    usage(argv[0]);
	    return EXIT_FAILURE;
	}
    }

    int status;
    CommunicatorPtr communicator;

    try
    {
	communicator = initializeWithProperties(properties);
	status = run(argc, argv, communicator, nowarn);
    }
    catch(const LocalException& ex)
    {
	cerr << ex << endl;
	status = EXIT_FAILURE;
    }

    if (communicator)
    {
	try
	{
	    communicator->destroy();
	}
	catch(const LocalException& ex)
	{
	    cerr << ex << endl;
	    status = EXIT_FAILURE;
	}
    }

    return status;
}
