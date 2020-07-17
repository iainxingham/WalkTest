#include "walktest.h"
#include "ui_walktest.h"

#include "dbobj.h"

#include <QMessageBox>

WalkTest::WalkTest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WalkTest)
{
    ui->setupUi(this);
    allOk = true;
}

WalkTest::~WalkTest()
{
    delete ui;
}

// Call before WalkTest::exec()
void WalkTest::setSubject(int patid)
{
    if(patid < 0) {
        allOk = false;

        QMessageBox msg;

        msg.setWindowTitle("Error");
        msg.setText("Invalid subject passed to WalkTest");
        msg.setIcon(QMessageBox::Critical);
        msg.exec();
    }

    auto subj = ClinicalDB->db_getpatdetails(patid);

    subject_id = patid;
    subject_rxr = subj.rxr;
    subject_nhs = subj.nhs;
    subject_fname = subj.fname;
    subject_sname = subj.sname;
    subject_dob = subj.dob;
}

void WalkTest::setTest(int testid)
{

}
