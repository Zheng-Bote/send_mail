/**
 * @file SmtpService.cpp
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @brief SMTP Service implementation
 * @version 0.1.0
 * @date 2026-01-02
 *
 * @copyright Copyright (c) 2025 ZHENG Robert
 *
 * SPDX-License-Identifier: MIT
 */

#include "SmtpService.hpp"
#include <SimpleMail>
#include <QDebug>

namespace rz {
namespace service {

SmtpService::SmtpService(const model::ConfigModel& config, QObject* parent)
    : QObject(parent), m_config(config) {
}

SmtpService::~SmtpService() = default;

SimpleMail::ServerReply* SmtpService::sendEmail(const QString& to, const QString& subject, const QString& body) {
    qInfo() << "Preparing to send email to:" << to;

    auto server = new SimpleMail::Server(this);
    server->setHost(m_config.getSmtpServer());
    server->setPort(m_config.getSmtpPort());
    server->setUsername(m_config.getSmtpUsername());
    server->setPassword(m_config.getSmtpPassword());

    if (m_config.getSmtpStartTls()) {
        server->setConnectionType(SimpleMail::Server::TlsConnection);
        qInfo() << "Connection settings: STARTTLS ENABLED" << "on port" << m_config.getSmtpPort();
    } else {
        server->setConnectionType(SimpleMail::Server::TcpConnection);
        qInfo() << "Connection settings: PLAIN TCP (Risk of insecure auth!)" << "on port" << m_config.getSmtpPort();
    }

    qInfo() << "Authenticating as user:" << m_config.getSmtpUsername();

    SimpleMail::MimeMessage message;
    message.setSender(SimpleMail::EmailAddress(m_config.getSmtpFrom(), m_config.getSmtpFrom()));
    message.addTo(SimpleMail::EmailAddress(to));
    message.setSubject(subject);

    auto textPart = std::make_shared<SimpleMail::MimeText>();
    textPart->setText(body);
    message.addPart(textPart);

    qDebug() << "Connecting to SMTP server...";

    return server->sendMail(message);
}

} // namespace service
} // namespace rz
