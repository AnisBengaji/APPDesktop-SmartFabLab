#ifndef EMAIL_H
#define EMAIL_H
#include <QString>
class Email
{

public:
    Email();
    Email(QString, QString, QString);
    static int sendEmail(QString dist, QString obj, QString bdy);
    private:
       QString destinataire;
       QString object,body;
};

#endif // EMAIL_H
