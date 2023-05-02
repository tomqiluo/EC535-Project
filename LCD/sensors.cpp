#include "sensors.h"

Sensors::Sensors(const QString& labelText, const QString& data, const QString& unit, QWidget *parent)
    : QWidget(parent)
{
    QString fullLabelText = QString("%1: %2 %3").arg(labelText, data, unit);
    dataLabel_ = new QLabel(fullLabelText, this);

    // Set the alignment of the label to top-left corner
    dataLabel_->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    // Set the font size to 14
    QFont font = dataLabel_->font();
    font.setPointSize(14);
    dataLabel_->setFont(font);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(dataLabel_);
    setLayout(layout);
}

void Sensors::setDataLabel(const QString& data)
{
    QString labelText = QString("%1").arg(data);
    dataLabel_->setText(labelText);
}
