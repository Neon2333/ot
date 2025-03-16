/********************************************************************************
** Form generated from reading UI file 'autofontsizelabel.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUTOFONTSIZELABEL_H
#define UI_AUTOFONTSIZELABEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AutoFontSizeLabel
{
public:

    void setupUi(QWidget *AutoFontSizeLabel)
    {
        if (AutoFontSizeLabel->objectName().isEmpty())
            AutoFontSizeLabel->setObjectName("AutoFontSizeLabel");
        AutoFontSizeLabel->resize(400, 300);

        retranslateUi(AutoFontSizeLabel);

        QMetaObject::connectSlotsByName(AutoFontSizeLabel);
    } // setupUi

    void retranslateUi(QWidget *AutoFontSizeLabel)
    {
        AutoFontSizeLabel->setWindowTitle(QCoreApplication::translate("AutoFontSizeLabel", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AutoFontSizeLabel: public Ui_AutoFontSizeLabel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUTOFONTSIZELABEL_H
