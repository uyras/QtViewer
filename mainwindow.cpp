#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QPainter>
#include <QMessageBox>
#include <QTableWidget>
#include <QtEvents>
#include <QDebug>
#include "Part.h"
#include "Vect.h"

#include <iostream>
#include <cstring>
#include <string>
#include <cmath>
#include <fstream>
#include <vector>
#include <map>
#include <ctime>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    sysprop(this)
{
    ui->setupUi(this);

    //нопочки и контрллы
    connect(ui->scaler,SIGNAL(valueChanged(int)),ui->surface,SLOT(scaleTo(int)));

    //масштабирование визуальной области
    connect(ui->surface, SIGNAL(scaleUp()), this, SLOT(scaleUp()));
    connect(ui->surface, SIGNAL(scaleDown()), this, SLOT(scaleDown()));

    //изменение состояния магнитной системы
    connect(this, SIGNAL(updateSys()), ui->surface->scene(), SLOT(update()));
    connect(ui->surface->scene(), SIGNAL(systemChanged()), this, SLOT(recalcSystemInfo()));

    //открываем закрываем окно свойств системы
    ui->widget_2->layout()->addWidget(&sysprop);
    connect(ui->systemProperties, SIGNAL(triggered(bool)), &sysprop, SLOT(setVisible(bool)));
    connect(&sysprop,SIGNAL(visibilityChanged(bool)),ui->systemProperties,SLOT(setChecked(bool)));

    //обновляем данные в диалоге свойств
    QSettings settings;
    ui->systemProperties->setChecked(settings.value("settings/systemProperties",true).toBool());
    ui->systemProperties->triggered(settings.value("settings/systemProperties",true).toBool());
    ui->doubleArrows->setChecked(settings.value("settings/doubleArrows",true).toBool());
    ui->autoScale->setChecked(settings.value("settings/autoScale",true).toBool());

    //освобождаем шорткат при запуске действия
    connect(ui->menuBar,SIGNAL(triggered(QAction*)),this,SLOT(releaseModifiers()));

    //подгружаем примеры
    initExamples();
}

MainWindow::~MainWindow()
{
    //сохраняем галочки
    QSettings settings;
    settings.setValue("settings/systemProperties", ui->systemProperties->isChecked());
    settings.setValue("settings/doubleArrows", ui->doubleArrows->isChecked());
    settings.setValue("settings/autoScale", ui->autoScale->isChecked());

    sysprop.close();
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<ui->systemProperties->isChecked();
    press = event->modifiers();
    if (press & Qt::ControlModifier){
        ui->modeBox->setCurrentIndex(1);
    }
    qDebug()<<press;
    QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    qDebug()<<(event->modifiers()^press);
    if ((event->modifiers()^press) & Qt::ControlModifier){
        ui->modeBox->setCurrentIndex(0);
    }
    press = event->modifiers();
    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::saveAsParticles(QString fname){
    if (this->filename.isEmpty()){
        QMessageBox(QMessageBox::Critical,
                    "Ошибка сохранения",
                    "Файл не открыт, сохранение невозможно!").exec();
        return;
    }

    QSettings settings;
    if (fname.isEmpty())
        fname = QFileDialog::getSaveFileName(this,"Выберите файл для сохранения",settings.value("settings/lastPath",QString()).toString());

    if (!fname.isEmpty()) {
        settings.setValue("settings/lastPath",fname);
        if (QFile::exists(fname)){
            QMessageBox msgBox;
            msgBox.setText(QString("Файл %1 существует.").arg(fname));
            msgBox.setInformativeText("Перезаписать файл?");
            msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Cancel);

            if (!msgBox.exec()) {
                return;
            }
        }

        sys.save(fname);
        setFileName(fname);
    }
}

void MainWindow::saveParticles()
{
    saveAsParticles(filename);
}

void MainWindow::loadParticles(QString filename){

    QSettings settings;

    if (filename.isEmpty())
        filename = QFileDialog::getOpenFileName(this, "Открытие файла магнитной системы",
            settings.value("settings/lastPath",QString()).toString(),
            tr("Файл магнитной системы (*.mfsys)"));

    if (!filename.isEmpty()) {
        settings.setValue("settings/lastPath",filename);
        sys.load(filename);
        if (sys.size()>0){
            ui->surface->scene()->init(&sys);
            setFileName(filename);
        } else {
            QMessageBox(QMessageBox::Critical,
                        "Ошибка чтения",
                        "Указанный файл имеет неверный формат, либо система пуста").exec();
        }
    }
    //устанавливаем автоматические коэффициенты
    this->toggleAutoCoff(ui->autoScale->isChecked());

    //ставим в зону видимости все изображение
    emit ui->surface->dbgSlot();
}

void MainWindow::exportImage()
{
    QSettings settings;
    QString fname = QFileDialog::getSaveFileName(this,"Выберите файл для сохранения",settings.value("settings/lastPath",QString()).toString());

    if (!fname.isEmpty()) {
        settings.setValue("settings/lastPath",fname);
        if (QFile::exists(fname)){
            QMessageBox msgBox;
            msgBox.setText(QString("Файл %1 существует.").arg(fname));
            msgBox.setInformativeText("Перезаписать файл?");
            msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Cancel);

            if (!msgBox.exec()) {
                return;
            }
        }

        QImage image(1920,1080,QImage::Format_ARGB32);
        image.fill(Qt::transparent);
        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing);
        ui->surface->scene()->render(&painter);
        bool saveResult = image.save(fname);
        if (!saveResult)
            qDebug()<<"error saving the file!";
    }
}

void MainWindow::toggleAutoCoff(bool ok)
{
    if (ok){
        //считаем авто коэфф.
        const double normalM = 25.,
                normalSpace=50.;
        double averM=0., minSpace;
        //если частиц в системе слишком мало
        if (sys.size()>1)
            minSpace=sys[0]->pos.space(sys[1]->pos);
        else
            minSpace=50;

        Part *temp1, *temp2;
        vector<Part*>::iterator iter1 = sys.parts.begin(), iter2;
        int i=0;
        while (iter1!=sys.parts.end()){
            temp1 = *iter1;
            averM = (averM*(double)i+temp1->m.length())/(double)(i+1);
            iter2=iter1;
            iter2++;
            while (iter2!=sys.parts.end()){
                temp2 = *iter2;
                temp1->pos.space(temp2->pos);
                iter2++;
            }
            iter1++; i++;
        }
        ui->surface->scene()->setCoffs(normalM/averM, normalSpace/minSpace);
    } else {
        //устанавливаем по умлочанию
        ui->surface->scene()->setCoffs(1., 1.);
    }
}

void MainWindow::scaleUp(){
    ui->scaler->setValue(ui->scaler->value() + ui->scaler->pageStep());
}

void MainWindow::scaleDown(){
    ui->scaler->setValue(ui->scaler->value() - ui->scaler->pageStep());
}

void MainWindow::setFileName(QString fname)
{
    this->filename = fname;
    this->setWindowTitle(QString(this->filename).prepend("QtViewer: "));
    ui->saveBtn->setEnabled(!filename.startsWith("examples/"));
}

void MainWindow::recalcSystemInfo()
{
    ui->infoLbl->setText(QString("E=%1").arg(sys.E()));
    emit sysprop.updateData(&sys);
}

void MainWindow::setMinState()
{
    sys.setMinstate(sys.State());
    emit updateSys();
}

void MainWindow::getMinState()
{
    sys.setState(sys.Minstate());
    emit updateSys();
}

void MainWindow::setMaxState()
{
    sys.setMaxstate(sys.State());
    emit updateSys();
}

void MainWindow::getMaxState()
{
    sys.setState(sys.Maxstate());
    emit updateSys();
}

void MainWindow::clearCurrentState()
{
    sys.state.reset();
    emit updateSys();
}

void MainWindow::changeCurrentState()
{
    StateChangeDialog dlg(this,"Изменени состояния системы", QString::fromStdString(sys.State().toString()));
    if (dlg.exec()){
        sys.state.fromString(dlg.getState().toStdString());
        emit updateSys();
    }
}

void MainWindow::setInteractionRange(double val)
{
    sys.setInteractionRange(val);
    emit updateSys();
}

void MainWindow::exampleClicked()
{
    QAction *temp = qobject_cast<QAction*>(sender());
    if (temp)
        loadParticles(QString(temp->text()).prepend("examples/"));
}

void MainWindow::initExamples()
{
    QDir dir("examples");
    if (!dir.exists())
        return;
    QStringList filenames = dir.entryList(QStringList("*.mfsys"),QDir::Files);
    if (filenames.length()==0)
        return ;
    ui->examplesMenu->setEnabled(true);
    QAction* temp;
    for (int i=0; i<filenames.length(); i++){
        temp = new QAction(filenames[i],this);
        ui->examplesMenu->addAction(temp);
        connect(temp,SIGNAL(triggered(bool)),this,SLOT(exampleClicked()));
        qDebug()<<filenames[i];
    }
}

void MainWindow::releaseModifiers()
{
    press=Qt::NoModifier;
    ui->modeBox->setCurrentIndex(0);
    qDebug()<<"release modifiers";
}
