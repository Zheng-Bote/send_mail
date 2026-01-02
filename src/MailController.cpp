/**
 * @file MailController.cpp
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @brief Controller implementation
 * @version 0.1.0
 * @date 2026-01-02
 *
 * @copyright Copyright (c) 2025 ZHENG Robert
 *
 * SPDX-License-Identifier: MIT
 */

#include "MailController.hpp"
#include <SimpleMail>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QDebug>
#include <QTimer>

namespace rz {
namespace controller {

MailController::MailController(service::SmtpService* service, QObject* parent)
    : QObject(parent), m_service(service) {
    connect(&m_watcher, &QFileSystemWatcher::directoryChanged, this, &MailController::onDirectoryChanged);
}

/**
 * @brief Start monitoring a directory for new files.
 *
 * adds the path to QFileSystemWatcher and performs an initial scan
 * to process any existing files.
 *
 * @param path The directory path.
 */
void MailController::startMonitoring(const QString& path) {
    m_watchedPath = path;
    if (!QDir(path).exists()) {
        qWarning() << "Directory does not exist:" << path;
        return;
    }
    m_watcher.addPath(path);
    qInfo() << "Monitoring directory:" << path;

    // Initial scan
    onDirectoryChanged(path);
}

/**
 * @brief Slot called when the directory content changes.
 *
 * Scans for files matching the *.SendMail pattern and triggers handling.
 *
 * @param path The path that changed.
 */
void MailController::onDirectoryChanged(const QString& path) {
    QDir dir(path);
    QStringList filters;
    filters << "*.SendMail";
    dir.setNameFilters(filters);

    QFileInfoList list = dir.entryInfoList();
    for (const QFileInfo& fileInfo : list) {
        handleNewFile(fileInfo.absoluteFilePath());
    }
}

/**
 * @brief Handle a detected file with a delay.
 *
 * Waits 2 seconds before processing the file to ensure write completion.
 * Extracts the recipient from the filename, reads content, sends email,
 * and deletes the file upon success.
 *
 * @param filePath Absolute path to the file.
 */
void MailController::handleNewFile(const QString& filePath) {
    // Check if we are already processing this file?
    // For now, stateless processing with delay.
    // The requirement says "Wait 2 seconds".
    // If file exists after 2 seconds, process it.

    // Use QTimer::singleShot with a lambda
    QTimer::singleShot(2000, [this, filePath]() {
        QFile file(filePath);
        if (!file.exists()) {
            return; // Maybe deleted or moved
        }

        QFileInfo info(filePath);
        QString fileName = info.fileName(); // e.g., "empfänger@mailadresse.domain.SendMail"

        // Extract email
        // Logic: filename is "<email>.SendMail"
        if (!fileName.endsWith(".SendMail")) return;

        QString email = fileName.left(fileName.length() - 9); // Remove .SendMail

        // Basic validation or just use it
        if (email.isEmpty()) return;

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
             qWarning() << "Could not open file:" << filePath;
             return;
        }

        QByteArray content = file.readAll();
        file.close();
        QString body = QString::fromUtf8(content);

        // Send Email
        SimpleMail::ServerReply* reply = m_service->sendEmail(email, "New Message from SendMail", body);

        if (reply) {
            connect(reply, &SimpleMail::ServerReply::finished, [reply, filePath, fileName]() {
                if (reply->error()) {
                    qWarning() << "Failed to send email to" << fileName << ":" << reply->responseText();
                } else {
                    qInfo() << "Email sent successfully to" << fileName;
                    // Delete file
                    QFile file(filePath);
                    if (file.remove()) {
                        qInfo() << "Processed and deleted file:" << fileName;
                    } else {
                        qWarning() << "Failed to delete file:" << fileName;
                    }
                }
                reply->deleteLater();
            });
        } else {
            qWarning() << "Failed to initiate email sending for" << fileName;
        }
    });
}

void MailController::processFile() {
    // Helper if needed
}

} // namespace controller
} // namespace rz
