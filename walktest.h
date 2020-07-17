#ifndef WALKTEST_H
#define WALKTEST_H

#include <QDialog>
#include <QDate>

namespace Ui {
class WalkTest;
}

class WalkTest : public QDialog
{
    Q_OBJECT

public:
    explicit WalkTest(QWidget *parent = nullptr);
    ~WalkTest();

    void setSubject(int patid);
    void setTest(int testid);

private:
    Ui::WalkTest *ui;

    bool allOk;

    int subject_id;
    QString subject_rxr;
    QString subject_nhs;
    QString subject_fname;
    QString subject_sname;
    QDate subject_dob;
};

#endif // WALKTEST_H
