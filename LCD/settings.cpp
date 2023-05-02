#include "settings.h"
#include <QVBoxLayout>

Settings::Settings(QWidget *parent)
    : QWidget(parent), brightness_(50)
{
    setupUi();
}

void Settings::setupUi()
{
    // Create a brightness slider
    brightnessSlider_ = new QSlider(Qt::Horizontal, this);
    brightnessSlider_->setRange(0, 100);
    brightnessSlider_->setValue(getBrightness()); // Set initial value from settings
    connect(brightnessSlider_, &QSlider::valueChanged, this, &Settings::onBrightnessChanged);

    // Create a label for the brightness slider
    brightnessLabel_ = new QLabel(tr("Brightness"), this);
    brightnessLabel_->setAlignment(Qt::AlignCenter);

    // Add the widgets to the layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(brightnessLabel_);
    layout->addWidget(brightnessSlider_);
}

int Settings::getBrightness() const
{
    return brightness_;
}

void Settings::setBrightness(int brightness)
{
    brightness_ = brightness;
}

void Settings::onBrightnessChanged(int value)
{
    setBrightness(value);
}

