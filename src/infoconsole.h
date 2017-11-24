#ifndef INFOCONSOLE_H
#define INFOCONSOLE_H

#include <QWidget>
#include <QString>
#include <QPalette>
#include <QTime>

namespace Ui {
class InfoConsole;
}

class InfoConsole : public QWidget
{
    Q_OBJECT

public:
    explicit InfoConsole(QWidget *parent = 0);
    ~InfoConsole();

    void write(const QString&);
    void write_compl(const QString&);
    void write_error(const QString&);
    void write_error_compl(const QString&);

    void clear();

private:
    void write_impl(const QString&, bool);

    Ui::InfoConsole *ui;
};

#endif // INFOCONSOLE_H
