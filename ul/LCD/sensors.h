#ifndef SENSORS_H
#define SENSORS_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class Sensors : public QWidget
{
    Q_OBJECT
public:
    explicit Sensors(const QString& labelText, const QString& data, const QString& unit, QWidget *parent = nullptr);

public slots:
    void setDataLabel(const QString& data);

private:
    QLabel *dataLabel_;
};

#endif // SENSORS_H

