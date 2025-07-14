/**============================================================================
Name        : ClientWindow.cpp
Created on  : 12.07.2025
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : ClientWindow.cpp
============================================================================**/

#include "ClientWindow.h"
#include "TcpClient.h"
#include "RequestBuilder.h"

#include <QFrame>
#include <QMainWindow>
#include <QPushButton>
#include <QApplication>
#include <QtCore>
#include <QStyle>
#include <QTextEdit>
#include <QComboBox>
#include <QStandardItemModel>

namespace ui
{
    using namespace std::string_literals;

    class DarkThemeApplication : public QApplication
    {
    public:
        DarkThemeApplication(int &argc, char **argv) : QApplication(argc, argv) {
            enableDarkMode();
        }

    private:

        static void enableDarkMode()
        {
            qApp->setStyle("Fusion");
            auto darkPalette = QPalette();
            auto darkColor = QColor(48, 48, 48);
            auto disabledColor = QColor(127,127,127);
            darkPalette.setColor(QPalette::Window, darkColor);
            darkPalette.setColor(QPalette::WindowText, Qt::white);
            darkPalette.setColor(QPalette::Base, QColor(18,18,18));
            darkPalette.setColor(QPalette::AlternateBase, darkColor);
            darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
            darkPalette.setColor(QPalette::ToolTipText, Qt::white);
            darkPalette.setColor(QPalette::Text, Qt::white);
            darkPalette.setColor(QPalette::Disabled, QPalette::Text, disabledColor);
            darkPalette.setColor(QPalette::Button, darkColor);
            darkPalette.setColor(QPalette::ButtonText, Qt::white);
            darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, disabledColor);
            darkPalette.setColor(QPalette::BrightText, Qt::red);
            darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
            darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
            darkPalette.setColor(QPalette::HighlightedText, Qt::black);
            darkPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, disabledColor);

            qApp->setPalette(darkPalette);
            qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
        }
    };


    struct ClientWindow: public QMainWindow
    {
        ClientWindow()
        {
            client = std::make_unique<client::TcpClient>();

            textFieldRequest.move(10, 10);
            textFieldRequest.setFixedHeight(100);
            textFieldRequest.setFixedWidth(280);

            textFieldResponse.move(10, 130);
            textFieldResponse.setFixedHeight(100);
            textFieldResponse.setFixedWidth(280);

            btnSend.move(410, 10);
            btnSend.setText("Send");
            // btnSend.setDisabled(true);

            comboBoxMessageType.move(300, 10);

            QStandardItemModel *model = new QStandardItemModel();

            // Add items to the model
            model->appendRow(new QStandardItem("Event"));
            model->appendRow(new QStandardItem("Message"));
            model->appendRow(new QStandardItem("Command"));

            comboBoxMessageType.setModel(model);

            connect(&btnSend, &QPushButton::clicked, [this] {
                sendMessage();
            });

            setCentralWidget(&frame);
            resize(500, 250);
        }

        ~ClientWindow() override = default;

    private:

        QFrame frame;

        QComboBox comboBoxMessageType { &frame };
        QPushButton btnSend {&frame};
        QTextEdit textFieldRequest{&frame};
        QTextEdit textFieldResponse {&frame};
        std::unique_ptr<client::IClient> client { nullptr };


        void sendMessage()
        {
            const std::string messageType = comboBoxMessageType.currentText().toStdString();
            const std::string messageText = textFieldRequest.toPlainText().toStdString();

            common::Request request;
            if ("Command"s == messageType) {
                request = common::RequestBuilder::createShutdownCommand();
            } else if ("Message"s == messageType) {
                request = common::RequestBuilder::createMessageRequest(messageText);
            } else if ("Event"s == messageType) {
                request = common::RequestBuilder::createEventRequest(messageText);
            }

            if (!client->connect("0.0.0.0", 52525)) {
                qDebug() << "Failed to connect to the server 0.0.0.0:52525";
                return;
            }

            const ssize_t bytesSend = client->send(request);
            if (0 >= bytesSend) {
                qDebug() << "Failed to send data";
            }

            common::Response response;
            const ssize_t bytesReceived = client->receive(response);
            textFieldResponse.setText(response.payload().data());
            if (0 >= bytesReceived) {
                qDebug() << "Failed to receive data";
                return;
            }


            // btnSend.setEnabled(false);
        };
    };
}

int ui::run(int argc, char** argv)
{
    DarkThemeApplication app(argc, argv);

    ClientWindow window;
    window.show();

    return ui::DarkThemeApplication::exec();
}

