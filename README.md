# IoT Attendance Verification System

![System Overview](system_overview.png)

## Overview

The IoT Attendance Verification System is a comprehensive solution for efficient attendance tracking in educational institutions and workplaces. This system combines ESP32, fingerprint recognition, RFID technology, and Google Sheets integration, ensuring secure and accurate attendance management. Additionally, a mobile app provides students with easy access to their attendance records.

## Key Features

- **Multi-Mode Verification**: Combines RFID card and fingerprint recognition for precise attendance verification.
- **Google Sheets Integration**: Automatically records attendance data, including student names and timestamps, to a Google Sheets spreadsheet.
- **Mobile App**: Allows students to view their attendance history and number of times present in school.
- **Secure**: Ensures secure access control through biometric verification.
- **Scalable**: Suitable for various settings, from small classrooms to large organizations.
- **Real-Time Data**: Provides real-time attendance data and insights.

## Setup and Configuration

To set up and configure the IoT Attendance Verification System, follow these steps:

1. **Hardware Setup**: Assemble the ESP32, fingerprint sensor, and RFID reader. Ensure they are properly connected.

2. **Google Sheets Integration**: Create a Google Sheets spreadsheet to store attendance data. Set up the necessary credentials for Google Sheets API access.

3. **Mobile App**: Develop or configure a mobile app that communicates with the system to provide students with attendance information.

4. **Code Configuration**: Customize the ESP32 code to match your hardware and Google Sheets credentials. Ensure the code handles RFID card and fingerprint verification as described.

5. **Deployment**: Deploy the system in your institution or workplace.

## Usage

1. **Student Registration**: Collect student details and associate them with their RFID cards. Generate unique IDs for each student.

2. **Attendance Recording**: When students enter, they scan their RFID cards. The system prompts them for fingerprint verification to confirm attendance.

3. **Data Recording**: Attendance records, including student names and timestamps, are automatically recorded in the Google Sheets spreadsheet.

4. **Mobile App**: Students can access their attendance history and view the number of times they were present using the mobile app.

## Contributing

Contributions to this project are welcome! If you have ideas for improvements or bug fixes, please open an issue or submit a pull request.

## License

This project is open-source and licensed under the [MIT License](LICENSE).

## Acknowledgments

We would like to acknowledge the open-source community and libraries that made this project possible.

---

