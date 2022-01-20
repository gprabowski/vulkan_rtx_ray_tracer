/********************************************************************************
** Form generated from reading UI file 'options.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPTIONS_H
#define UI_OPTIONS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Options
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_3;
    QFormLayout *formLayout;
    QCheckBox *AOcheckBox;
    QLabel *label;
    QDoubleSpinBox *AOtMinSpinBox;
    QDoubleSpinBox *AOtMaxSpinBox;
    QSpinBox *AOnumRays;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_4;
    QFormLayout *formLayout_2;
    QCheckBox *checkBox_2;
    QLabel *label_2;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_5;
    QFormLayout *formLayout_3;
    QCheckBox *checkBox_3;
    QLabel *label_3;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Options)
    {
        if (Options->objectName().isEmpty())
            Options->setObjectName(QString::fromUtf8("Options"));
        Options->resize(235, 727);
        verticalLayout = new QVBoxLayout(Options);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        groupBox_4 = new QGroupBox(Options);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        verticalLayout_3 = new QVBoxLayout(groupBox_4);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        AOcheckBox = new QCheckBox(groupBox_4);
        AOcheckBox->setObjectName(QString::fromUtf8("AOcheckBox"));

        formLayout->setWidget(0, QFormLayout::LabelRole, AOcheckBox);

        label = new QLabel(groupBox_4);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::FieldRole, label);

        AOtMinSpinBox = new QDoubleSpinBox(groupBox_4);
        AOtMinSpinBox->setObjectName(QString::fromUtf8("AOtMinSpinBox"));

        formLayout->setWidget(1, QFormLayout::LabelRole, AOtMinSpinBox);

        AOtMaxSpinBox = new QDoubleSpinBox(groupBox_4);
        AOtMaxSpinBox->setObjectName(QString::fromUtf8("AOtMaxSpinBox"));

        formLayout->setWidget(2, QFormLayout::LabelRole, AOtMaxSpinBox);

        AOnumRays = new QSpinBox(groupBox_4);
        AOnumRays->setObjectName(QString::fromUtf8("AOnumRays"));

        formLayout->setWidget(3, QFormLayout::LabelRole, AOnumRays);

        label_4 = new QLabel(groupBox_4);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout->setWidget(1, QFormLayout::FieldRole, label_4);

        label_5 = new QLabel(groupBox_4);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        formLayout->setWidget(2, QFormLayout::FieldRole, label_5);

        label_6 = new QLabel(groupBox_4);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout->setWidget(3, QFormLayout::FieldRole, label_6);


        verticalLayout_3->addLayout(formLayout);


        verticalLayout_2->addWidget(groupBox_4);

        groupBox = new QGroupBox(Options);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_4 = new QVBoxLayout(groupBox);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        checkBox_2 = new QCheckBox(groupBox);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, checkBox_2);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, label_2);


        verticalLayout_4->addLayout(formLayout_2);


        verticalLayout_2->addWidget(groupBox);

        groupBox_3 = new QGroupBox(Options);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        verticalLayout_5 = new QVBoxLayout(groupBox_3);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        formLayout_3 = new QFormLayout();
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        checkBox_3 = new QCheckBox(groupBox_3);
        checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, checkBox_3);

        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout_3->setWidget(0, QFormLayout::FieldRole, label_3);


        verticalLayout_5->addLayout(formLayout_3);


        verticalLayout_2->addWidget(groupBox_3);


        horizontalLayout->addLayout(verticalLayout_2);


        verticalLayout->addLayout(horizontalLayout);

        buttonBox = new QDialogButtonBox(Options);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(Options);
        QObject::connect(buttonBox, SIGNAL(accepted()), Options, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Options, SLOT(reject()));

        QMetaObject::connectSlotsByName(Options);
    } // setupUi

    void retranslateUi(QDialog *Options)
    {
        Options->setWindowTitle(QCoreApplication::translate("Options", "Dialog", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("Options", "Ambient Occlusion", nullptr));
        AOcheckBox->setText(QString());
        label->setText(QCoreApplication::translate("Options", "Enabled", nullptr));
        label_4->setText(QCoreApplication::translate("Options", "tMin", nullptr));
        label_5->setText(QCoreApplication::translate("Options", "tMax", nullptr));
        label_6->setText(QCoreApplication::translate("Options", "Number of rays", nullptr));
        groupBox->setTitle(QCoreApplication::translate("Options", "Indirect Lighting", nullptr));
        checkBox_2->setText(QString());
        label_2->setText(QCoreApplication::translate("Options", "Enabled", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("Options", "Mirror Recursion", nullptr));
        checkBox_3->setText(QString());
        label_3->setText(QCoreApplication::translate("Options", "Enabled", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Options: public Ui_Options {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTIONS_H
