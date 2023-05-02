# Add the appropriate modules
QT       += core gui widgets charts

TARGET = lcd
TEMPLATE = app

# Add the source files
SOURCES += \
    main.cpp \
    settings.cpp \
    mainmenu.cpp \
    sensors.cpp

# Add the header files
HEADERS += \
    settings.h \
    mainmenu.h \
	sensors.h
