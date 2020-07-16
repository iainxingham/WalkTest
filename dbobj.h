#ifndef DBOBJ_H
#define DBOBJ_H

#include <QSqlDatabase>
#include <QString>
#include <QDate>

enum class DB_Sex
{
    Male,
    Female
};

enum class DB_Physiology
{
    Height,
    Weight,
    FeNO
};

class DBObj
{
public:
    DBObj();
    ~DBObj();

    // Various direct database access / setup functions
    bool db_init(QString driver="QSQLITE");
    void db_setpath(QString path);
    int db_getpatid(QString rxr, QString nhs=QString());
    int db_getpatid(QString rxr, QString nhs, QString fname, QString sname, QDate dob, DB_Sex sex);
    bool db_rxrexists(QString rxr);
    void db_insertphysiology(int patid, DB_Physiology type, double result, QDate testdate=QDate::currentDate());
    void db_setpat_updated(int patid, QDate newdate=QDate::currentDate());

    // Validate data for database
    bool valid_rxr(QString rxr);
    bool valid_nhs(QString nhs);

    QString DBSexToString(DB_Sex s);
    QString DBPhysiologyToString(DB_Physiology p);

private:
    QSqlDatabase db;
    QString db_path;

    QString clean_rxr(QString rxr);
    QString clean_nhs(QString nhs);
    QString clean_name(QString name);
};

#ifndef MAIN_CPP
extern DBObj *ClinicalDB;
#endif // MAIN_CPP

#endif // DBOBJ_H
