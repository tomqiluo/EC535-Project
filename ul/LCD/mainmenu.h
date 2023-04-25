#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);

private:
    QLabel *title;
    QLabel *authors;
    QLabel *project_name;
    QVBoxLayout *layout;
};

#endif // MAINMENU_H

