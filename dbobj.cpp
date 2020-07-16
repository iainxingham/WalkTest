#include "dbobj.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QVariant>
#include <QRegularExpression>

DBObj::DBObj()
{

}

DBObj::~DBObj()
{
    if(db.isOpen()) db.close();
}

bool DBObj::db_init(QString driver)
{
    if(!QSqlDatabase::isDriverAvailable(driver)) {
        QMessageBox msg;

        msg.setWindowTitle("Error");
        msg.setText("Database driver unavailable");
        msg.setIcon(QMessageBox::Critical);
        msg.exec();

        return false;
    }

    db = QSqlDatabase::addDatabase(driver);
    db.setDatabaseName(db_path);

    if(!db.open()) {
        QMessageBox msg;

        msg.setWindowTitle("Error");
        msg.setText("Can't open database");
        msg.setIcon(QMessageBox::Critical);
        msg.exec();

        return false;
    }

    return true;
}

void DBObj::db_setpath(QString path)
{
    db_path = path;
}


// Look for rxr in database
// If exists then return patient id
// If it doesn't exist create and return id
// Update nhs number if not already in database
// Return negative number if something goes wrong
int DBObj::db_getpatid(QString rxr, QString nhs)
{
    QSqlQuery q1, q2;

    if (!valid_rxr(rxr)) return -1;
    else rxr = clean_rxr(rxr);

    if (!valid_nhs(nhs)) nhs = QString();
    else nhs = clean_nhs(nhs);

    q1.prepare("SELECT id, nhs FROM pats WHERE rxr = ?");
    q1.addBindValue(rxr);
    q1.exec();

    if(q1.first()) {
        if((!nhs.isEmpty()) && (q1.value(1).toString().isEmpty())) {
            q2.prepare("UPDATE pats SET nhs = ? WHERE id = ?");
            q2.addBindValue(nhs);
            q2.addBindValue(q1.value(0).toInt());
            q2.exec();
        }
        return q1.value(0).toInt();
    }

    q2.prepare("INSERT INTO pats (rxr, nhs) VALUES (?, ?)");
    q2.addBindValue(rxr);
    q2.addBindValue(nhs);
    q2.exec();

    q1.exec();
    if(q1.first()) return q1.value(0).toInt();

    return -1;
}

int DBObj::db_getpatid(QString rxr, QString nhs, QString fname, QString sname, QDate dob, DB_Sex sex)
{
    QSqlQuery q1, q2;

    if (!valid_rxr(rxr)) return -1;
    else rxr = clean_rxr(rxr);

    if (!valid_nhs(nhs)) nhs = QString();
    else nhs = clean_nhs(nhs);

    fname = clean_name(fname);
    sname = clean_name(sname);

    q1.prepare("SELECT id, nhs, fname, surname, dob, sex, dateadded FROM pats WHERE rxr = ?");
    q1.addBindValue(rxr);
    q1.exec();

    if(q1.first()) {
        // Should this UPDATE pats ????
        //q2.prepare("UPDATE pats SET nhs = :nhs, fname = :fname, surname = :surname, dob = :dob, sex = :sex")
        return q1.value(0).toInt();
    }

    q2.prepare("INSERT INTO pats (rxr, nhs, fname, surname, dob, sex, dateadded, updated)"
               "VALUES (:rxr, :nhs, :fname, :surname, :dob, :sex, :dateadded, :updated)");

    q2.bindValue(":rxr", rxr);
    q2.bindValue("nhs", nhs);
    q2.bindValue(":fname", fname);
    q2.bindValue(":surname", sname);
    q2.bindValue(":dob", dob.toString());
    q2.bindValue(":sex", DBSexToString(sex));
    QString datenow = QDate::currentDate().toString();
    q2.bindValue(":dateadded", datenow);
    q2.bindValue(":updated", datenow);
    q2.exec();

    q1.exec();
    if(q1.first()) return q1.value(0).toInt();
    return -1;
}

bool DBObj::valid_rxr(QString rxr)
{
    QRegularExpression re("\\d{7}");

    rxr = rxr.trimmed();

    if(rxr.length() != 10) return false;
    if(!rxr.startsWith("rxr", Qt::CaseInsensitive)) return false;
    if(re.match(rxr.right(7)).hasMatch()) return true;

    return false;
}

bool DBObj::valid_nhs(QString nhs)
{
    QRegularExpression re("^\\d{3}\\s*\\d{3}\\s*\\d{4}$");

    if(re.match(nhs.trimmed()).hasMatch()) return true;

    return false;
}

QString DBObj::clean_rxr(QString rxr)
{
    return rxr.trimmed().toUpper();
}

QString DBObj::clean_nhs(QString nhs)
{
    QRegularExpression re("(\\d{3})\\s*(\\d{3})\\s*(\\d{4})");
    QRegularExpressionMatch match;

    match = re.match(nhs.trimmed());
    if(match.hasMatch()) {
        return (match.captured(1) + " " + match.captured(2) + " " + match.captured(3));
    }

    return QString();
}

bool DBObj::db_rxrexists(QString rxr)
{
    QSqlQuery q1;

    if (!valid_rxr(rxr)) return false;
    else rxr = clean_rxr(rxr);

    q1.prepare("SELECT id FROM pats WHERE rxr = ?");
    q1.addBindValue(rxr);
    q1.exec();

    if(q1.first()) return true;

    return false;
}

// Capitalise first letter of name, rest as lower case.
// Handle hyphenated names
QString DBObj::clean_name(QString name)
{
    QStringList words = name.trimmed().toLower().split('-', QString::SkipEmptyParts);
    for (int i = 0; i < words.size(); ++i)
        words[i].replace(0, 1, words[i][0].toUpper());

    return words.join("-");
}

QString DBObj::DBSexToString(DB_Sex s)
{
    if(s == DB_Sex::Male) return "Male";
    if(s == DB_Sex::Female) return "Female";

    return QString();
}

void DBObj::db_insertphysiology(int patid, DB_Physiology type, double result, QDate testdate)
{
    if(patid < 0) return;

    QSqlQuery q1;

    q1.prepare("INSERT INTO physiology (pat_id, test_date, measurement, result)"
               "VALUES (:patid, :testdate, :type, :result)");
    q1.bindValue(":patid", patid);
    q1.bindValue(":testdate", testdate.toString());
    q1.bindValue(":type", DBPhysiologyToString(type));
    q1.bindValue(":result", QString::number(result));

    // execute query

    db_setpat_updated(patid, testdate);
}

// db_setpat_updated() and DBPhysiologyToString()
