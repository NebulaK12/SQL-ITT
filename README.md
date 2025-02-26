# SQL-ITT

## Overview
SQL-ITT is a SQL Injection testing tool designed for ethical penetration testing purposes. It provides a user-friendly interface to test web applications for SQL Injection vulnerabilities. This tool allows security professionals and developers to assess whether a website is susceptible to SQL Injection attacks.

## Features
- Supports manual and automated SQL Injection testing
- Predefined SQL Injection payloads for quick testing
- Custom payload entry for advanced testing
- Detects Boolean-based SQL Injection by analyzing response differences
- Response logging with optional log export
- Dark mode toggle for improved visibility
- Built using C++ and Qt6 for cross-platform support

## Installation
### Prerequisites
- **Qt6** (Widgets and Network modules required)
- **CMake 3.5+**
- **Git** (optional for cloning the repository)

### Steps
1. Clone the repository:
   ```sh
   git clone https://github.com/NebulaK12/SQL-ITT.git
   cd SQL-ITT
   ```
2. Create a build directory and navigate into it:
   ```sh
   mkdir build && cd build
   ```
3. Run CMake to configure the project:
   ```sh
   cmake ..
   ```
4. Compile the project:
   ```sh
   cmake --build .
   ```
5. Run the application:
   ```sh
   ./SQL-ITT  # Linux/macOS
   SQL-ITT.exe # Windows
   ```

## Usage
### 1. Setting Up a Test
- Enter the target URL in the input field.
- Select a predefined SQL Injection payload from the dropdown menu or enter a custom payload.
- Click **Test SQL Injection** to execute the request.

### 2. Analyzing Results
- The response will be displayed in the response window.
- If the response indicates an SQL error or differs significantly from the normal response, the application will highlight a potential vulnerability.
- Logs can be saved automatically or exported manually.

### 3. Logging
- If the "Save Logs" option is enabled, the logs will be saved to `SQLi_Log.txt`.
- Logs contain:
  - Target URL
  - Normal response
  - Injected response
  - Timestamp

## Security & Ethical Considerations
- **SQL-ITT is intended for ethical penetration testing only.**
- Use only on systems you have explicit permission to test.
- Logs and responses should be handled securely and not shared without authorization.

## License
SQL-ITT is released under an open-source license. Refer to the repository for license details.

## Contribution
Contributions are welcome. To contribute:
1. Fork the repository
2. Create a feature branch
3. Submit a pull request with detailed explanations of changes

## Contact
For any issues, suggestions, or improvements, open an issue on the GitHub repository.

