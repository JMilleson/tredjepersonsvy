#ifndef OCCULUSSENSORCONTROLLER_H
#define OCCULUSSENSORCONTROLLER_H

#include <QtMath>
#include <QObject>
#include <OVR_CAPI.h>
#include <OVR_Math.h>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
struct SensorData {
    float pitch, yaw, roll;
};

class OcculusSensorController : public QObject
{
    Q_OBJECT

private:
    ovrSession session = NULL;
    SensorData sensorData;
public:
    explicit OcculusSensorController(QObject *parent = 0);
    int init();
    int updateTracking();
    SensorData getSensorData(){return sensorData;}
    QJsonObject getSensorDataAsJSON();
signals:

public slots:
};

#endif // OCCULUSSENSORCONTROLLER_H
