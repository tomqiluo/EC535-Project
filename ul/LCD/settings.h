#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QSlider>
#include <QLabel>

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);

private:
    QSlider *brightnessSlider_;
    QLabel *brightnessLabel_;

    int brightness_;

    void setupUi();
    int getBrightness() const;
    void setBrightness(int brightness);

private slots:
    void onBrightnessChanged(int value);
};

#endif // SETTINGS_H

