#include "Server.h"

Server::Server(QString passwd, int port)
{
    m_passwd = passwd;
    m_port = port;
}

void Server::SetPasswd(QString passwd)
{
    m_passwd = passwd;
}

void Server::SetPort(int port)
{
    m_port = port;
}

void Server::Start()
{
    qDebug() << "Start.";

    tcpServer = new QTcpServer();
    if (!tcpServer->listen(QHostAddress::LocalHost, m_port))
    {
        qDebug() << "Unable to start the server: " << tcpServer->errorString() << ".";
        tcpServer->close();
        return;
    }

    connect(tcpServer, &QTcpServer::newConnection, this, &Server::clientConnected);
}

void Server::Restart()
{
    qDebug() << "restart port is now " << m_port;
 //   if(tcpServer == 0)
   // {
     //   qDebug() << "nullptr...";
   // }

    tcpServer->close();

  //  tcpServer = new QTcpServer();
  //  tcpServer->li
    if (!tcpServer->listen(QHostAddress::AnyIPv4, m_port))
    {
        qDebug() << "Unable to start the server: " << tcpServer->errorString() << ".";
        tcpServer->close();
        return;
    }

 //   connect(tcpServer, &QTcpServer::newConnection, this, &Server::clientConnected);
}

void Server::clientConnected()
{
    emit ShowTrayMsg("New Remote Control Connection.");

    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();

    connect(clientConnection, &QTcpSocket::readyRead, [=]()
    {
        QByteArray data = clientConnection->readAll();

        QString msg = data.data();

        msg.remove(QRegExp("[\\n\\t\\r]"));

        qDebug() << "RECIVED DATA: " << msg;

        if(msg.length() > 0)
            ExecuteCommand(msg);

        clientConnection->disconnectFromHost();
        clientConnection->deleteLater();

    });
}

void Server::ExecuteCommand(QString msg)
{
    QStringList strList = msg.split(" ");
    qDebug() << "strList.count " << strList.count();

    if(strList.count() != 2)
        return;

    if(strList.at(0) != m_passwd)
    {
        qDebug() << "Wrong password.";
        return;
    }
    else
    {
        qDebug() << "Password is okey.";
    }


    emit ShowTrayMsg("Command: " + strList[1]);

    if(strList[1] == "SLEEP")
    {
        //QProcess::startDetached("rundll32.exe powrprof.dll,SetSuspendState 0,1,0");
    }
}
