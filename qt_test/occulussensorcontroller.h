#ifndef OCCULUSSENSORCONTROLLER_H
#define OCCULUSSENSORCONTROLLER_H

#include <QObject>

class OcculusSensorController : public QObject
{
    Q_OBJECT
public:
    explicit OcculusSensorController(QObject *parent = 0);

signals:

public slots:
};

#endif // OCCULUSSENSORCONTROLLER_H