#ifndef NETWORKLOGIC_H
#define NETWORKLOGIC_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class NetworkLogic : public QObject
{
    Q_OBJECT
public:
    explicit NetworkLogic(QObject *parent = nullptr);

public slots:
    void startServer(const QString &ip, quint16 port);
    void connectToServer(const QString &ip, quint16 port);
    void stopServer();
    void disconnectClient();

signals:
    void serverStarted(const QString &ip, quint16 port);
    void serverError(const QString &error);
    void clientConnected();
    void clientError(const QString &error);
    void messageReceived(const QString &message);

private slots:
    void onNewConnection();
    void onClientReadyRead();
    void onClientDisconnected();

private:
    QTcpServer *m_server;
    QTcpSocket *m_clientSocket;
};

#endif // NETWORKLOGIC_H