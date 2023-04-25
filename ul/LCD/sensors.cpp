#include "sensors.h"

Sensors::Sensors(const QString& labelText, const QString& data, const QString& unit, QWidget *parent)
    : QWidget(parent)
{
    QString fullLabelText = QString("%1: %2 %3").arg(labelText, data, unit);
    dataLabel_ = new QLabel(fullLabelText, this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(dataLabel_);
    setLayout(layout);
}

void Sensors::setDataLabel(const QString& data)
{
    QString labelText = QString("%1").arg(data);
    dataLabel_->setText(labelText);
}

