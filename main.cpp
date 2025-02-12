#include "rendezVous.h"
#include "connexion.h"
#include <QApplication>
#include <QMessageBox>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Connection c;
     bool test=c.createconnect();
    rendezVous w;
    if(test)
    {
        w.show();
        QMessageBox::information(nullptr, QObject::tr("database is open"),
                    QObject::tr("connection successful.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);

}
    else
        QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                    QObject::tr("connection failed.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);



    return app.exec();
}
