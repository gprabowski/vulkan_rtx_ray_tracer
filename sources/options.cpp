#include "options.h"
#include "ui_options.h"

Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);
}

Options::~Options()
{
    delete ui;
}


bool Options::getAO() {
   return ui->AOcheckBox->isChecked();
}
double Options::getAOtMin() {
    return ui->AOtMinSpinBox->value();
}
double Options::getAOtMax() {
    return ui->AOtMaxSpinBox->value();
}
uint Options::getAORays() {
    return ui->AOnumRays->value();
}
