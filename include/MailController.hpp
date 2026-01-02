/**
 * @file MailController.hpp
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @brief Controller to monitor files and trigger sending
 * @version 0.1.0
 * @date 2026-01-02
 *
 * @copyright Copyright (c) 2025 ZHENG Robert
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include "SmtpService.hpp"
#include <QObject>
#include <QFileSystemWatcher>
#include <QTimer>
#include <QMap>

namespace rz {
namespace controller {

/**
 * @brief The MailController class manages file monitoring and email dispatching.
 *
 * It watches a specified directory for files ending in .SendMail.
 * When a file detects, it waits for a short delay and then triggers the SmtpService
 * to send the file content as an email.
 */
class MailController : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Construct a new Mail Controller object.
     *
     * @param service Pointer to the initialized SmtpService.
     * @param parent The parent QObject.
     */
    explicit MailController(service::SmtpService* service, QObject* parent = nullptr);

    /**
     * @brief Start monitoring a directory.
     *
     * @param path Absolute or relative path to the directory to watch.
     */
    void startMonitoring(const QString& path);

private slots:
    /**
     * @brief Slot triggered when the watched directory changes.
     *
     * @param path The path that changed.
     */
    void onDirectoryChanged(const QString& path);

    /**
     * @brief Placeholder for processing files (unused directly, logic in lambdas).
     */
    void processFile();

private:
    service::SmtpService* m_service;
    QFileSystemWatcher m_watcher;
    QString m_watchedPath;

    /**
     * @brief Handle a newly detected file.
     *
     * Schedules a timer to process the file after a delay.
     *
     * @param filePath The absolute path to the detected file.
     */
    void handleNewFile(const QString& filePath);
};

} // namespace controller
} // namespace rz
