#include "occulussensorcontroller.h"

OcculusSensorController::OcculusSensorController(QObject *parent) : QObject(parent)
{

}

int OcculusSensorController::init(){

    ovrResult ovrResult = ovr_Initialize(nullptr);
    if (OVR_FAILURE(ovrResult)){
        qDebug() << "init failed";
        return -1;
    }
    ovrGraphicsLuid luid;
    ovrResult = ovr_Create(&session, &luid);
    if (OVR_FAILURE(ovrResult))
    {
        qDebug() << "create session failed";
        ovr_Shutdown();
        return -2;
    }

    ovrHmdDesc desc = ovr_GetHmdDesc(session);
    ovrSizei resolution = desc.Resolution;

    return 0;

}
