#pragma warning (disable:4786)

#include <vld.h>
#include <stdio.h>
#include <vector>
#include <assert.h>

#include <OB/CORBA.h>
#include <OB/Properties.h>
#include <OB/BootManager.h>
#include "IRemote_impl.h"

using namespace std;

//---------------------------------------------------------------------------
// Set Coraba Port
//---------------------------------------------------------------------------
#if (OB_INTEGER_VERSION < 4010000L)
#	define CORBA_SETPORT(l,p) ((l)->setProperty("ooc.orb.oa.port", p))
#else
#	define CORBA_SETPORT(l,p) ((l)->setProperty("ooc.orb.oa.endpoint", "iiop --port "##p))
#endif
//---------------------------------------------------------------------------

CORBA::ORB_var orb;
//---------------------------------------------------------------------------
int main(int argc, char * argv[])
{
	try
	{
		////-----------------------------------------
		//// CORBA init.
		////-----------------------------------------
#if 0
		orb = CORBA::ORB_init (argc, argv);
#else
		OB::Properties_var dflt = OB::Properties::getDefaultProperties();
		OB::Properties_var props = new OB::Properties(dflt);
		//props -> setProperty("ooc.orb.conc_model", "threaded");
		CORBA_SETPORT (props, "5000");
		orb = OBCORBA::ORB_init (argc, argv, props);
#endif	
		CORBA::Object_var vObj;

		////-----------------------------------------
		//// Get reference to Root POA
		////-----------------------------------------
		vObj = orb->resolve_initial_references("RootPOA");
		PortableServer::POA_var vRootPOA = PortableServer::POA::_narrow(vObj);

		////-----------------------------------------
		//// Activate POA manager
		////-----------------------------------------
		PortableServer::POAManager_var vPOAManager = vRootPOA->the_POAManager();
		vPOAManager->activate();
		//IRemote_impl::POA (vRootPOA, vPOAManager);

		////-----------------------------------------
		//// Boot Manager
		////-----------------------------------------
		vObj = orb->resolve_initial_references ("BootManager");
		OB::BootManager_var vBootManager = OB::BootManager::_narrow (vObj);

		////-----------------------------------------
		//// Create an object
		////-----------------------------------------
		IHost_impl * pHostImpl = new IHost_impl();
		PortableServer::ServantBase_var vHostSrvt = pHostImpl;
		const char * szIHost = idl::srvtName_IHost;
		PortableServer::ObjectId * pHostObjId = PortableServer::string_to_ObjectId (szIHost);
		vObj = pHostImpl->_this();
		vBootManager->add_binding(*pHostObjId, vObj);
		
		////-----------------------------------------
		//// Accept requests
		////-----------------------------------------
		puts (">>>> CORBA Server: running\n");
		orb->run();
		if (!CORBA::is_nil(orb)) 
			orb->destroy(); // Wait for shutdown to complete
	}
	catch(const CORBA::Exception& aError)
	{
		cerr << aError << endl;
		return EXIT_FAILURE;
	}
	catch (...)
	{
		cerr << "Cannot initialize ORB" << endl;
		return 1;
	}
	////-----------------------------------------
	//// Use ORB...
	////-----------------------------------------
	if(!CORBA::is_nil(orb))
	try
	{
		orb->destroy(); // Must destroy!
	}
	catch(const CORBA::Exception& aError)
	{
		cerr << "FINALISATION ERROR:" << endl;
		cerr << aError << endl;
		return EXIT_FAILURE;
	}
	catch (...)
	{
		cerr << "FINALISATION ERROR:" << endl;
		cerr << "unkonwn exception!" << endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
//---------------------------------------------------------------------------
}