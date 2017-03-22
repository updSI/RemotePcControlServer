#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QNetworkSession>
#include <QtNetwork/QTcpSocket>
#include <QDebug>
#include <QObject>
#include <QtEndian>
#include <QProcess>

class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(QString passwd, int port);
    void SetPasswd(QString passwd);
    void SetPort(int port);
    void Start();
    void Restart();

signals:
    void ShowTrayMsg(QString);

private:
    void ExecuteCommand(QString msg);

private slots:
    void clientConnected();

private:
    QTcpServer *tcpServer;
    QStringList fortunes;
    QNetworkSession *networkSession;
    std::vector<char> buffer;
    QString m_passwd;
    int m_port;
};

#endif // SERVER_H
