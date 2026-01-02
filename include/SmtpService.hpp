/**
 * @file SmtpService.hpp
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @brief SMTP Service to send emails
 * @version 0.1.0
 * @date 2026-01-02
 *
 * @copyright Copyright (c) 2025 ZHENG Robert
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include "ConfigModel.hpp"
#include <QString>
#include <QObject>

// Forward declarations
namespace SimpleMail {
    class ServerReply;
}

namespace rz {
namespace service {

/**
 * @brief The SmtpService class handles email transmission via SMTP.
 *
 * It uses the SimpleMail library to connect to an SMTP server, authenticate,
 * and send MIME messages. It supports STARTTLS and explicit AUTH LOGIN.
 */
class SmtpService : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Construct a new Smtp Service object.
     *
     * @param config The configuration model containing SMTP settings.
     * @param parent The parent QObject.
     */
    explicit SmtpService(const model::ConfigModel& config, QObject* parent = nullptr);

    /**
     * @brief Destroy the Smtp Service object.
     */
    ~SmtpService();

    /**
     * @brief Send an email asynchronously.
     *
     * Creates a new SMTP connection (for each mail, currently) and sends the message.
     *
     * @param to The recipient's email address.
     * @param subject The email subject.
     * @param body The email body content.
     * @return SimpleMail::ServerReply* A pointer to the reply object for signal handling.
     *         The caller is responsible for deleting this object (e.g. via deleteLater() in slot).
     */
    SimpleMail::ServerReply* sendEmail(const QString& to, const QString& subject, const QString& body);

private:
    model::ConfigModel m_config;
};

} // namespace service
} // namespace rz
