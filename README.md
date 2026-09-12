# Wireless Android Auto Dongle

DIY Wireless Android Auto adapter to use with a car that supports only wired Android Auto using a Raspberry Pi.

This repository consists of the buildroot setup to generate an sd card image to create your own Wireless Android Auto adapter.

## Features

- Passes through all Android Auto traffic without any modifications to ensure seamless and safe experience.
- Fast bootup, connection under 30 seconds.
- Supports the Raspberry Pi Zero 2 W target hardware.

## Supported Hardware
This project is currently built and tested for:
- **Raspberry Pi Zero 2 W**

The board should support USB OTG or Gadget mode, have Wifi and Bluetooth, and be able to operate on car power.

## How to use
1. Flash the built image to an SD card.
2. Connect the board to the car using the USB OTG-enabled port.
3. Pair the device named `AndroidAuto-Dongle-*` or `WirelessAADongle-*` on your phone.
4. The phone should automatically connect via Wifi and the dongle will connect to the headunit via USB and start Android Auto on the car screen.

### Subsequent connections

1. Turn on the car.
2. Wait until the dongle boots and the phone connects automatically.
3. Android Auto should start on the car screen.

## Troubleshoot

### Common issues

#### Bluetooth and Wifi seems connected, but the phone stuck at "Looking for Android Auto"
The most common issue behind this is either bad USB cable or use of wrong USB port on the device. Make sure:
1. The cable is good quality data cable and not power-only cable
2. You're using the OTG enabled usb port on the board, and not the power-only port.

#### "Device not responding" error on headunit
Make sure that "Wireless Android Auto" is enabled in your phone's Andriod Auto settings. This option is only available and required on some older phones.

### Getting logs
Once you've already tried multiple times and it still does not work, you can ssh into the device and try to get some logs.

- Set a static password by setting the `AAWG_WIFI_PASSWORD` config and rebuild the Zero 2 W image.
- Connect the device to the headunit, let it boot and try to connect once. The logs are not persisted across reboots, so you need to get the logs in the same instance soon after you observe the issue.
- Connect to the device using wifi (SSID: AAWirelessDongle, Password: <as set in the first step>).
- SSH into the device (username: root, password: password, see [raspberrypizero2w_defconfig](aa_wireless_dongle/configs/raspberrypizero2w_defconfig)).
- Once you're in, try to have a look at `/var/log/messages` file, it should have most relevant logs to start with. You can also copy the file and attach to issues you create if any.

## Contribute
[Find or create a new issue](https://github.com/nisargjhaveri/WirelessAndroidAutoDongle/issues) for any bugs or improvements.

Feel free to [Create a PR](https://github.com/nisargjhaveri/WirelessAndroidAutoDongle/pulls) to fix any issues. Refer [BUILDING.md](BUILDING.md) for instructions on how to build locally.

## Acknowledgements

Thanks to all the contributors and open source projects that made this possible.

In any case, don't forget to star on github and spread the word if you think this project might be useful to someone else as well.

## Limitations
This is currently tested with very limited set of headunits and cars. Let me know if it does not work with your headunit.
