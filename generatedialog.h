#ifndef GENERATEDIALOG_H
#define GENERATEDIALOG_H

#include <QDialog>
#include <QDebug>
#include <cmath>

namespace Ui {
class generateDialog;
}

class generateDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit generateDialog(QWidget *parent = 0);
	~generateDialog();

    int getW();
    int getH();
    double getR();
    double getM();
    double getC();
    int getN();
    double getDistance();
    int getMode(); //режим генерации
	
private slots:
	void on_buttonBox_rejected();

	void on_buttonBox_accepted();

    void recalcInfoLabels();

private:
	Ui::generateDialog *ui;
};

#endif // GENERATEDIALOG_H
