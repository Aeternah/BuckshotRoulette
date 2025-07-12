#include "networklogic.h"
#include <QDebug>

NetworkLogic::NetworkLogic(QObject *parent) : QObject(parent), m_server(nullptr), m_clientSocket(nullptr)
{
}

void NetworkLogic::startServer(const QString &ip, quint16 port)
{
    m_server = new QTcpServer(this);
    connect(m_server, &QTcpServer::newConnection, this, &NetworkLogic::onNewConnection);

    if (m_server->listen(QHostAddress(ip), port)) {
        emit serverStarted(ip, port);
        qDebug() << "Server started on" << ip << ":" << port;
    } else {
        emit serverError(m_server->errorString());
        qDebug() << "Server error:" << m_server->errorString();
        m_server->deleteLater();
        m_server = nullptr;
    }
}

void NetworkLogic::connectToServer(const QString &ip, quint16 port)
{
    m_clientSocket = new QTcpSocket(this);
    connect(m_clientSocket, &QTcpSocket::readyRead, this, &NetworkLogic::onClientReadyRead);
    connect(m_clientSocket, &QTcpSocket::disconnected, this, &NetworkLogic::onClientDisconnected);

    m_clientSocket->connectToHost(ip, port);
    if (m_clientSocket->waitForConnected(5000)) {
        emit clientConnected();
        m_clientSocket->write("hello\n");
        qDebug() << "Connected to server at" << ip << ":" << port;
    } else {
        emit clientError(m_clientSocket->errorString());
        qDebug() << "Client error:" << m_clientSocket->errorString();
        m_clientSocket->deleteLater();
        m_clientSocket = nullptr;
    }
}

void NetworkLogic::stopServer()
{
    if (m_server) {
        m_server->close();
        m_server->deleteLater();
        m_server = nullptr;
        qDebug() << "Server stopped";
    }
}

void NetworkLogic::disconnectClient()
{
    if (m_clientSocket) {
        m_clientSocket->disconnectFromHost();
        m_clientSocket->deleteLater();
        m_clientSocket = nullptr;
        qDebug() << "Client disconnected";
    }
}

void NetworkLogic::onNewConnection()
{
    QTcpSocket *client = m_server->nextPendingConnection();
    connect(client, &QTcpSocket::readyRead, this, &NetworkLogic::onClientReadyRead);
    connect(client, &QTcpSocket::disconnected, this, &NetworkLogic::onClientDisconnected);
    client->write("Server: Welcome!\n");
    emit clientConnected();
    qDebug() << "New client connected";
}

void NetworkLogic::onClientReadyRead()
{
    QTcpSocket *sender = qobject_cast<QTcpSocket*>(QObject::sender());
    if (sender) {
        QString message = QString::fromUtf8(sender->readAll());
        emit messageReceived(message);
        qDebug() << "Message received:" << message;
    }
}

void NetworkLogic::onClientDisconnected()
{
    QTcpSocket *sender = qobject_cast<QTcpSocket*>(QObject::sender());
    if (sender) {
        sender->deleteLater();
        qDebug() << "Client disconnected";
    }
}