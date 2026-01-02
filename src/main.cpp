/**
 * @file main.cpp
 * @author ZHENG Robert (robert@hase-zheng.net)
 * @brief Send mail based on input
 * @version 0.1.0
 * @date 2026-01-02
 *
 * @copyright Copyright (c) 2025 ZHENG Robert
 *
 * SPDX-License-Identifier: MIT
 */


#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>

#include "rz_config.hpp"
#include "ConfigModel.hpp"
#include "SmtpService.hpp"
#include "MailController.hpp"



/**
 * @brief Main entry point for the SendMail application.
 *
 * Initialises the Qt Core Application, loads configuration,
 * creates the SmtpService and MailController, and starts the event loop.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return int Exit code.
 */
int main(int argc, char *argv[]) {
  QCoreApplication qtApp(argc, argv);

  // 1. App Info aus rz_config setzen
  qInfo() << "Starte" << rz::config::PROG_LONGNAME.data() << "v"
          << rz::config::VERSION.data();

  // 2. Load Configuration
  rz::model::ConfigModel config;
  QString envPath = QCoreApplication::applicationDirPath() + "/SendMail.env";
  if (!QFile::exists(envPath)) {
      envPath = "SendMail.env";
  }
  config.loadEnv(envPath.toStdString());

  // 3. Initialize Service and Controller
  rz::service::SmtpService smtpService(config);
  rz::controller::MailController controller(&smtpService);

  // 4. Start Monitoring
  QString watchDir = config.getWatchDir();
  // Override with args if provided (optional but good practice)
  if (argc > 1) {
      watchDir = argv[1];
  }

  controller.startMonitoring(watchDir);

  return qtApp.exec();
}
