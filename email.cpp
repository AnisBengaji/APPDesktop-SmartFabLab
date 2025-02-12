#include "email.h"
#include <QtNetwork>
#include <QDebug>
#include <QFileInfo>
#include <QByteArray>
#include <QTextStream>
#include <QtCharts>
#include <QtGui>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QtWidgets>
#include <QtNetwork>
#include <QFileDialog>
#include <QDateTime>

Email::Email()
{
    destinataire="";
    object="";
    body="";
}
int Email::sendEmail(QString dist, QString obj, QString bdy)
{
    qDebug()<<"sslLibraryBuildVersionString: "<<QSslSocket::sslLibraryBuildVersionString();
    qDebug()<<"sslLibraryVersionNumber: "<<QSslSocket::sslLibraryVersionNumber();
    qDebug()<<"supportsSsl: "<<QSslSocket::supportsSsl();
    qDebug()<<QCoreApplication::libraryPaths();
    QString smtpServer ="smtp.gmail.com";
    int smtpPort = 465;
    QString username = "ammar.ahmed.aziz@gmail.com";
    QString password = "kbyg xyno ikgz mnmr";
    QString from = "ammar.ahmed.aziz@gmail.com";
    QString to =dist;
    QString subject = obj;
    QString body = bdy;
    QSslSocket socket;
    socket.connectToHostEncrypted(smtpServer, smtpPort);
    if (!socket.waitForConnected()) {
          qDebug() << "Error connecting to the server:" << socket.errorString();
          return -1;
    }
    if (!socket.waitForReadyRead()) {
         qDebug() << "Error reading from server:" << socket.errorString();
         return -1;
     }
        qDebug() << "Connected to the server.";
        socket.write("HELO localhost\r\n");
        socket.waitForBytesWritten();
        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }
        socket.write("AUTH LOGIN\r\n");
        socket.waitForBytesWritten();

        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }
        socket.write(QByteArray().append(username.toUtf8()).toBase64() + "\r\n");
        socket.waitForBytesWritten();
        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }
        socket.write(QByteArray().append(password.toUtf8()).toBase64() + "\r\n");
        socket.waitForBytesWritten();
        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }
        socket.write("MAIL FROM:<" + from.toUtf8() + ">\r\n");
        socket.waitForBytesWritten();

        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }
        socket.write("RCPT TO:<" + to.toUtf8() + ">\r\n");
        socket.waitForBytesWritten();
        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }
        socket.write("DATA\r\n");
        socket.waitForBytesWritten();
        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }
        socket.write("From: " + from.toUtf8() + "\r\n");
        socket.write("To: " + to.toUtf8() + "\r\n");
        socket.write("Subject: " + subject.toUtf8() + "\r\n");
        socket.write("\r\n");
        socket.write(body.toUtf8() + "\r\n");
        socket.write(".\r\n");
        socket.waitForBytesWritten();
        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }
        socket.write("QUIT\r\n");
        socket.waitForBytesWritten();
        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }
        qDebug() << "Email sent successfully.";
        socket.close();
        return 0;
}
