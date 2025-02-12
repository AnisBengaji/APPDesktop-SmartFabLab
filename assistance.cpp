#include "assistance.h"
#include<QDebug>

assistance::assistance()
{

}

assistance::assistance(int id_formation, int id_client)
{
   this->id_formation=id_formation;
    this->id_client=id_client;
}

bool assistance::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO ASSISTER (id_formation, id_client) "
                  "VALUES (:idf, :idc)");
    query.bindValue(":idf", id_formation);
    query.bindValue(":idc", id_client);

    if (query.exec()) {
        return true; // Insertion successful
    } else {
        return false; // Insertion failed
    }
}

QSqlQueryModel *assistance::afficher()
{
    QSqlQueryModel *model=new QSqlQueryModel();
    model->setQuery("SELECT * FROM ASSISTER");
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("id_formation"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("id_client"));
    return model;

}

bool assistance::supprimer(int formations)
{
    QSqlQuery query;
    query.prepare("DELETE FROM ASSISTER WHERE id_formation = :idf");
    query.bindValue(":idf", formations);

    if (query.exec()) {
        return true; // Deletion successful
    } else {
        return false; // Deletion failed
    }

}

bool assistance::modifier(int idf)
{
    QSqlQuery query;
    query.prepare("UPDATE assister SET id_formation = :idf, id_client = :idc "
                  "WHERE id_formation = :idfo");
    query.bindValue(":idfo", idf);
    query.bindValue(":idf", id_formation);
    query.bindValue(":idc", id_client);

    if (query.exec()) {
        // Update object's data if the query was successful

        return true; // Update successful
    } else {
        return false; // Update failed
    }
}

bool assistance::idExists(int id_formation) const
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM formation WHERE id_formation = :id");
    query.bindValue(":id", id_formation);
    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        return count > 0;
    }
    return false;
}


bool assistance::idcExist(int idc) const
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM client WHERE cin_client = :id");
    query.bindValue(":id", idc);
    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        return count > 0;
    }
    return false;
}

QString assistance::recupereadr(int idc)
{
    QSqlQuery query;
    query.prepare("SELECT mail_client FROM client WHERE cin_client = :id");
    query.bindValue(":id", idc);
    if (query.exec() && query.next()) {
         QString mail= query.value(0).toString();
        return mail;
    }
}

