#ifndef OCCULUSSENSORCONTROLLER_H
#define OCCULUSSENSORCONTROLLER_H

#include <QObject>
#include <OVR_CAPI.h>
#include <OVR_Math.h>
#include <QtDebug>

class OcculusSensorController : public QObject
{
    Q_OBJECT

private:
    ovrSession session = NULL;

public:
    explicit OcculusSensorController(QObject *parent = 0);
    int init();
signals:

public slots:
};

#endif // OCCULUSSENSORCONTROLLER_H
