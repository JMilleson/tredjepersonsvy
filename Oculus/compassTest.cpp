// Include the OculusVR SDK
	#include <OVR_CAPI.h>
	void Application()
	{
	   ovrResult result = ovr_Initialize(nullptr);
	   if (OVR_FAILURE(result))
	       return;

	   ovrSession session;
	   ovrGraphicsLuid luid;
	   result = ovr_Create(&session, &luid);
	   if (OVR_FAILURE(result))
	   {
	      ovr_Shutdown();
	      return;
	   }

	   ovrHmdDesc desc = ovr_GetHmdDesc(session);
	   ovrSizei resolution = desc.Resolution;

	   ovr_Destroy(session);
	   ovr_Shutdown();
	}


