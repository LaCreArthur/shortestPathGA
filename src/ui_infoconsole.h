/********************************************************************************
** Form generated from reading UI file 'infoconsole.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFOCONSOLE_H
#define UI_INFOCONSOLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InfoConsole
{
public:
    QPlainTextEdit *info_text;

    void setupUi(QWidget *InfoConsole)
    {
        if (InfoConsole->objectName().isEmpty())
            InfoConsole->setObjectName(QStringLiteral("InfoConsole"));
        InfoConsole->resize(400, 300);
        info_text = new QPlainTextEdit(InfoConsole);
        info_text->setObjectName(QStringLiteral("info_text"));
        info_text->setGeometry(QRect(0, 0, 401, 301));
        QFont font;
        font.setFamily(QStringLiteral("DejaVu Sans Mono"));
        info_text->setFont(font);
        info_text->setReadOnly(true);

        retranslateUi(InfoConsole);

        QMetaObject::connectSlotsByName(InfoConsole);
    } // setupUi

    void retranslateUi(QWidget *InfoConsole)
    {
        InfoConsole->setWindowTitle(QApplication::translate("InfoConsole", "Info Console", 0));
    } // retranslateUi

};

namespace Ui {
    class InfoConsole: public Ui_InfoConsole {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFOCONSOLE_H
