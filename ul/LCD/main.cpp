#include <QApplication>

#include <QTabWidget>

#include <QTimer>

#include <QFile>

#include <QTextStream>

#include "settings.h"

#include "mainmenu.h"

#include "sensors.h"

int main(int argc, char * argv[]) {
  QApplication app(argc, argv);

  QTabWidget * tabWidget = new QTabWidget;
  tabWidget -> setTabPosition(QTabWidget::West);
  tabWidget -> setTabShape(QTabWidget::Triangular);

  Settings * settings = new Settings(tabWidget);
  MainMenu * mainMenu = new MainMenu(tabWidget);
  Sensors * coData = new Sensors("CO", "", "ppm", tabWidget);
  Sensors * co2Data = new Sensors("CO2", "", "ppm", tabWidget);
  Sensors * humidityData = new Sensors("Humidity", "", "%", tabWidget);
  Sensors * temperatureData = new Sensors("Temperature", "", "°C", tabWidget);
  Sensors * vocData = new Sensors("VOC", "", "ppb", tabWidget);

  tabWidget -> addTab(mainMenu, "Main Menu");
  tabWidget -> addTab(coData, "CO Data");
  tabWidget -> addTab(co2Data, "CO2 Data");
  tabWidget -> addTab(humidityData, "Humidity");
  tabWidget -> addTab(temperatureData, "Temperature");
  tabWidget -> addTab(vocData, "VOC Data");

  tabWidget -> addTab(settings, "Settings");

  tabWidget -> show();

  // Set up a timer to read the text file once a second
  QFile file("sampledata.txt");
  QTextStream in ( & file);
  QStringList lines;
  while (!in.atEnd()) {
    QString line = in.readLine();
    lines.append(line);
  }
  file.close();

  QTimer * timer = new QTimer;
  int nextLine = 0; // Variable to keep track of the next line to read from the file
  QObject::connect(timer, & QTimer::timeout, [ & ]() {
    QFile file("sampledata.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      QTextStream in ( & file);
      for (int i = 0; i < nextLine; i++) {
        in.readLine(); // Skip lines that have already been read
      }
      QString line = in.readLine();
      if (!line.isEmpty()) {
        QStringList fields = line.split(",");
        if (fields.count() == 5) { // Update count to 5 for temperature and VOC data
          // Update the sensor data labels with formatted data
          QString coDataLabel = QString("CO: %1 ppm").arg(fields[0].trimmed());
          QString co2DataLabel = QString("CO2: %1 ppm").arg(fields[1].trimmed());
          QString humidityDataLabel = QString("Humidity: %1 %").arg(fields[2].trimmed());
          QString temperatureDataLabel = QString("Temperature: %1 °C").arg(fields[3].trimmed());
          QString vocDataLabel = QString("VOC: %1 ppb").arg(fields[4].trimmed());
          coData -> setDataLabel(coDataLabel);
          co2Data -> setDataLabel(co2DataLabel);
          humidityData -> setDataLabel(humidityDataLabel);
          temperatureData -> setDataLabel(temperatureDataLabel);
          vocData -> setDataLabel(vocDataLabel);
        }
        nextLine++; // Increment the nextLine variable
      }
      file.close();
    }
  });

  timer -> start(1000);

  return app.exec();
}
