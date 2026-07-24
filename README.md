# ESP32-S3 Live Camera Viewfinder 📸

This project turns an ESP32-S3 into a real-time live video viewfinder. It captures video frames from an OV3660 camera sensor and streams them directly to a 2.8" ILI9341 TFT display using the ESP-IDF framework.

By leveraging Direct Memory Access (DMA) over the SPI bus and keeping the image format in pure RGB565, the ESP32 acts as a high-speed pipeline, transferring pixels from the camera's PSRAM buffer directly to the screen with zero CPU overhead for color conversion.

## ⚙️ Hardware Requirements
* **Board:** ESP32-S3 (Model N16R8 - 16MB Flash, 8MB Octal PSRAM is highly recommended).
* **Camera:** OV3660 module.
* **Display:** 2.8" TFT SPI Display (ILI9341 Controller, 240x320 resolution).

## 🔌 Wiring & Pinout

Since the camera module reserves a large chunk of the ESP32's GPIO matrix, the TFT display is mapped to the upper GPIO pins.

### TFT Display (ILI9341) Connection
The display is driven via the `SPI2_HOST` on the ESP32-S3. Note that the MISO pin is not needed since the data flow is strictly unidirectional (ESP32 to Display).

| TFT Pin Name | ESP32-S3 GPIO | Description |
| :--- | :--- | :--- |
| **VCC** | 3.3V | Main power supply (Do NOT use 5V) |
| **GND** | GND | Ground |
| **CS** | GPIO 38 | Chip Select - Enables the display on the SPI bus |
| **RESET** | GPIO 39 | Hardware Reset pin |
| **DC** | GPIO 40 | Data/Command - Distinguishes between control commands and pixel data |
| **SDI (MOSI)** | GPIO 41 | Master Out Slave In - The high-speed data line |
| **SCK** | GPIO 42 | Serial Clock - Driven at 40MHz |
| **LED** | 3.3V | Backlight power (Tied to 3.3V for 100% brightness) |
| **SDO (MISO)**| *Not Connected* | Master In Slave Out (Ignored in this project) |
| **T_* Pins** | *Not Connected* | Touchscreen pins (Ignored) |
| **SD_* Pins** | *Not Connected* | SD Card reader pins (Ignored) |

### Camera (OV3660) Connection
| Camera Pin | ESP32-S3 GPIO |
| :--- | :--- |
| D0 - D7 | 11, 9, 8, 10, 12, 18, 17, 16 |
| XCLK | 15 |
| PCLK | 13 |
| VSYNC | 6 |
| HREF | 7 |
| SDA | 4 |
| SCL | 5 |

## 🛠️ Software & Configuration

This project was built using **ESP-IDF v5.x**. 

### 1. Dependencies
The project relies on the official ESP LCD components. To add the ILI9341 driver to your project, run:
```bash
idf.py add-dependency "espressif/esp_lcd_ili9341"
