#include "addpat.h"
#include "ui_addpat.h"
#include "dbobj.h"

#include <QIntValidator>
#include <QMessageBox>

addPat::addPat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addPat)
{
    ui->setupUi(this);

    // Minimum and maximum height of 50 cm and 250 cm
    ui->heightEdit->setValidator(new QIntValidator(50, 250, this));
    // Min and max weight of 25 kg and 300 kg
    ui->weightEdit->setValidator(new QDoubleValidator(25.0, 300.0, 1, this));
}

addPat::~addPat()
{
    delete ui;
}

void addPat::on_buttonBox_accepted()
{
    // Pressed "OK"
    int patid;
    DB_Sex s;

    if(!ClinicalDB->valid_rxr(ui->rxrEdit->text())) {
        QMessageBox msg;

        msg.setWindowTitle("Invalid hospital number");
        msg.setText("Please check the hospital number. It doesn't appear to be correct.");
        msg.setIcon(QMessageBox::Warning);
        msg.exec();

        return;
    }

    if(ClinicalDB->db_rxrexists(ui->rxrEdit->text())) {
        patid = ClinicalDB->db_getpatid(ui->rxrEdit->text());
        // Warning - patient already exisits in database
        // ?? Screen to check details
    }
    else {
        // Need to check behaviour if sex hasn't been specified
        if(ui->sexMradioButton->isChecked()) s = DB_Sex::Male;
        else if(ui->sexFradioButton->isChecked())s = DB_Sex::Female;

        patid = ClinicalDB->db_getpatid(ui->rxrEdit->text(),
                                        ui->nhsEdit->text(),
                                        ui->fnameEdit->text(),
                                        ui->snameEdit->text(),
                                        ui->dobEdit->date(),
                                        s);
    }

    if(patid < 0) {
        QMessageBox msg;

        msg.setWindowTitle("Can't add patient to database");
        msg.setText("This may be due to an invalid hospital number or there may be a problem with the database itself");
        msg.setIcon(QMessageBox::Warning);
        msg.exec();

        return;
    }

    // Add height
    if(!ui->heightEdit->text().isEmpty()) ClinicalDB->db_insertphysiology(patid, DB_Physiology::Height, ui->heightEdit->text().toDouble());
    // Add weight
    if(!ui->weightEdit->text().isEmpty()) ClinicalDB->db_insertphysiology(patid, DB_Physiology::Weight, ui->weightEdit->text().toDouble());
}
