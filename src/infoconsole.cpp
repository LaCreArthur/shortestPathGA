#include "infoconsole.h"
#include "ui_infoconsole.h"

InfoConsole::InfoConsole(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoConsole)
{
    ui->setupUi(this);
    this->setFixedSize(400, 300);

    QPalette p;

    // La console, c'est du blanc sur fond noir
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::white);
    ui->info_text->setPalette(p);
}

void InfoConsole::write_impl(const QString& info, bool time)
{
    QString current_time = QTime::currentTime().toString();
    ui->info_text->appendHtml((time ? current_time : "") + " - " + info);
}

void InfoConsole::write(const QString& info)
{
    this->write_impl(info, false);
}

void InfoConsole::write_compl(const QString& info)
{
    this->write_impl("(" + info + ")", true);
}

void InfoConsole::write_error(const QString& info)
{
    this->write_impl("<font color=\"Red\">Erreur : " + info + "</font>", true);
}

void InfoConsole::write_error_compl(const QString& info)
{
    this->write_impl("<font color=\"Red\">(" + info + ")</font>", false);
}

void InfoConsole::clear()
{
    ui->info_text->clear();
}

InfoConsole::~InfoConsole()
{
    delete ui;
}
