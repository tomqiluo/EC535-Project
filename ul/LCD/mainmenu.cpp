#include "mainmenu.h"

#include "settings.h"

MainMenu::MainMenu(QWidget * parent): QWidget(parent) {
  // Create the title label
  title = new QLabel("EC 535");
  title -> setAlignment(Qt::AlignHCenter | Qt::AlignTop);
  title -> setStyleSheet("font-size: 24px; font-weight: bold;");

  project_name = new QLabel("Clean Air");
  project_name -> setAlignment(Qt::AlignHCenter);
  project_name -> setStyleSheet("font-size: 16px; font-weight: bold;");

  // Create the authors label
  authors = new QLabel("Ahmet Caliskan\nQi Luo");
  authors -> setAlignment(Qt::AlignHCenter);

  // Create the layout and add the labels
  layout = new QVBoxLayout;
  layout -> addWidget(title);
  layout -> addWidget(project_name);
  layout -> addWidget(authors);

  // Set the layout for the widget
  setLayout(layout);
}
