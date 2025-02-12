#ifndef ASSISTANCE_H
#define ASSISTANCE_H
#include<QSql>
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>
#include<QTableView>



class assistance
{
private:

    int id_formation;
    int id_client;

public:
    assistance();

   //constructeurs

        assistance(int id_formation,int id_client);

        //getters
        int getid_formation() { return id_formation; }
        int getid_client(){return id_client;}


        //setters:
        void setid_formation(int id_formation) { this->id_formation = id_formation; }
        void setid_client(int id_client) { this->id_client = id_client ;}
        bool ajouter();
        QSqlQueryModel* afficher();
        bool supprimer(int);
        bool modifier(int idf);
      bool  idExists(int id_formation) const;
      bool idcExist(int idc) const;
      QString recupereadr(int);

};


#endif // ASSISTANCE_H
