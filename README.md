# ESP32-C6 examples

## Description

This repository comes with a number of example sketches. You can see them in the `examples/` folder.
Uncomment a `src_dir=` in the `platformio.ini` file or start a new project with the `src/main.cpp` file.

## Supported boards

If you want to support this project, you can use these affiliate links ...

- [NanoESP32-C6 (Aliexpress affiliate link)](https://s.click.aliexpress.com/e/_ooBtUih) (affiliate link) with up to 16MB flash [datasheet](doc/nanoESP32C6.pdf)

- [Super Mini ESP32-C6 (Aliexpress affiliate link)](https://s.click.aliexpress.com/e/_DeLjVMb) (affiliate link) with 4MB flash and W2812 RGB LED

- [LilyGo T-QT-C6 (Aliexpress affiliate link)](https://github.com/mcuw/esp32-t-qt-c6-sdk): use Arduino SDK https://github.com/mcuw/esp32-t-qt-c6-sdk

## Prerequisites

- [Visual Studio Code](https://code.visualstudio.com/) IDE
- [pioarduino](https://marketplace.visualstudio.com/items?itemName=pioarduino.pioarduino-ide) extension

## Get Started

1. build and flash your ESP32-C6 - default app is under src/main.cpp

2. try out examples - comment out a `src_dir` in platformio.ini

3. try out OTA to flash fast and w/o USB cable - flash once with an USB cable then configure the `extra_configs/ota.ini` to flash over Wi-Fi.

## Troubleshooting

### No update after flashing

Some boards requires a click on the reset button or disconnect/ reconnect of the board.

## Disclaimer

Contribution and help - if you find an issue or wants to contribute then please do not hesitate to create a pull request or an issue.

We provide our build template as is, and we make no promises or guarantees about this code.
