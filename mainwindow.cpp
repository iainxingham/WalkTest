#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dbobj.h"
#include "addpat.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(!ClinicalDB) ClinicalDB = new DBObj();
    ClinicalDB->db_setpath("../Database/test1.sqlite");
    ClinicalDB->db_init();

    model_pats.setQuery(model_pats_query);

    ui->patsView->setModel(&model_pats);
    ui->patsView->show();
}

MainWindow::~MainWindow()
{
    delete ClinicalDB;
    delete ui;
}


void MainWindow::on_addpatButton_clicked()
{
    addPat *newpat;

    newpat = new addPat(this);
    newpat->exec();
    delete newpat;
}
