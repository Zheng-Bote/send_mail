# SendMail

SendMail is a robust C++23 console application designed to automate email dispatching based on file system events. Built with Qt6 and the SimpleMail library, it monitors a specific directory and automatically emails the contents of distinctively named text files.

## Architecture

The application follows a strict **Model-View-Controller (MVC)** architectural pattern to ensure separation of concerns and maintainabilty:

- **Model (`ConfigModel`)**: Responsible for loading and validating application configuration from environment variables (`.env`). It provides a clean interface for accessing SMTP credentials and runtime settings.
- **View**: As a background console application, the "View" is represented by the standard output/logging (QDebug/QInfo) which provides real-time status updates and error reporting to the administrator.
- **Controller (`MailController`)**: The core logic engine. It manages the `QFileSystemWatcher` to monitor the target directory. Upon detecting valid files, it orchestrates the workflow by reading the file and delegating the transmission task to the Service layer.
- **Service (`SmtpService`)**: A dedicated service wrapper around `SimpleMail`. It handles the low-level SMTP protocol details, including connection establishment, STARTTLS upgrades, finding the correct authentication method (`AUTH LOGIN`), and MIME message construction.

### Workflow

1. **Initialization**: The app loads settings from `SendMail.env`.
2. **Monitoring**: It starts watching the configured `WATCH_DIR` for files matching `*.SendMail`.
3. **Detection**: When a file like `user@example.com.SendMail` is created:
   - The Controller detects the event.
   - It waits for a stabilization period (2 seconds) to ensure the write is complete.
4. **Processing**:
   - The recipient email is extracted from the filename (`user@example.com`).
   - The file content is read and used as the email body.
5. **Transmission**: `SmtpService` connects to the SMTP server (using STARTTLS/AuthLogin) and sends the email.
6. **Cleanup**: On successful transmission, the source file is deleted from the disk.

## Prerequisites

- **Linux** (Tested on current distributions)
- **C++23 Compiler** (GCC 12+ or Clang 16+)
- **CMake** 3.24 or higher
- **Qt6** (Core, Network)
- **OpenSSL**

## Build Instructions

```bash
# Clone the repository
git clone https://github.com/Zheng-Bote/send-mail.git
cd send-mail

# Create build directory
mkdir build && cd build

# Configure and Build
cmake ..
make -j$(nproc)
```

## Configuration

Create a `SendMail.env` file in the same directory as the executable (or copy `docs/SendMail.env`):

```ini
SMTP_SERVER="smtp.example.com"
SMTP_PORT="587"
SMTP_USERNAME="user@example.com"
SMTP_PASSWORD="secret_password"
SMTP_FROM="sender@example.com"
# "true" to enforce STARTTLS
SMTP_STARTTLS="true"
# Directory to monitor
WATCH_DIR="../data"
```

## Usage

Run the application:

```bash
./SendMail
```

To trigger an email, create a file in the watched directory:

```bash
echo "Hello World" > data/recipient@domain.com.SendMail
```

The application will pick up this file, send an email to `recipient@domain.com` with the body "Hello World", and then delete the file.

### Troubleshooting

- **Error 535 5.7.8**: This usually indicates an authentication failure. Ensure your `SMTP_USERNAME` and `SMTP_PASSWORD` are correct. The application enforces `AUTH LOGIN` which is required by many providers like Netcup.
- **STARTTLS**: The application logs the connection type on startup. Check the console output to verify if secure connection is enabled.

## License

This project is licensed under the MIT License.
