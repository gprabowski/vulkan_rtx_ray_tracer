#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>

namespace Ui {
class Options;
}

class Options : public QDialog
{
    Q_OBJECT

public:
    explicit Options(QWidget *parent = nullptr);
    ~Options();
    bool getAO();
    double getAOtMin();
    double getAOtMax();
    uint getAORays();

private:
    Ui::Options *ui;
};

#endif // OPTIONS_H
