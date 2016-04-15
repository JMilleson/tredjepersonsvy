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

    qDebug() << "Oculus initiated";
    return 0;

}

int OcculusSensorController::updateTracking(){

    using namespace OVR;
    OVR::Posef pose;

    ovrTrackingState ts = ovr_GetTrackingState(session, ovr_GetTimeInSeconds(), ovrFalse);
    if (ts.StatusFlags & (ovrStatus_OrientationTracked | ovrStatus_PositionTracked)) {
        pose = ts.HeadPose.ThePose;
        pose.Rotation.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(&this->sensorData.yaw, &this->sensorData.roll, &this->sensorData.pitch);
        this->sensorData.yaw = qRadiansToDegrees(this->sensorData.yaw);
        this->sensorData.roll = qRadiansToDegrees(this->sensorData.roll);
        this->sensorData.pitch = qRadiansToDegrees(this->sensorData.pitch);
        return 0;

        /*yaw = yaw * 180 / 3.1416;
        yaw += 200;
        printf("compass: %f\n", yaw);
        printf("target height: %d\n", TARGET_HEIGHT);
        //sprintf_s(sendbuf, "%6.1f", yaw);
        sendbuf[0] = STX;
        sendbuf[1] = '0' + (((int)(yaw / 100)) % 10);
        sendbuf[2] = '0' + (((int)(yaw / 10)) % 10);
        sendbuf[3] = '0' + (((int)yaw) % 10);
        sendbuf[4] = '0' + (((int)(TARGET_HEIGHT / 1000)) % 10);
        sendbuf[5] = '0' + (((int)(TARGET_HEIGHT / 100)) % 10);
        sendbuf[6] = '0' + (((int)(TARGET_HEIGHT / 10)) % 10);
        sendbuf[7] = '0' + (((int)TARGET_HEIGHT) % 10);
        sendbuf[8] = ETX;
        sendbuf[9] = '\0';

        iResult = send(ConnectSocket, sendbuf, 10, 0);
        if (iResult == SOCKET_ERROR) {
            printf("send failed: %d\n", WSAGetLastError());
        }*/

    } else {
        return -1;
    }
}

QJsonObject OcculusSensorController::getSensorDataAsJSON()
{

    QJsonObject sensordata
    {
        {"pitch", (int)sensorData.pitch},
        {"yaw", (int)(sensorData.yaw * -100)},
        {"roll", (int)sensorData.roll}
    };

    QJsonObject data = {
        {"sensordataoculus", sensordata}
    };

    return data;
}
