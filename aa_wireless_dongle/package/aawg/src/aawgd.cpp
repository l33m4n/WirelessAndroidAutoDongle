#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <atomic>

#include "common.h"
#include "bluetoothHandler.h"
#include "proxyHandler.h"
#include "uevent.h"
#include "usb.h"

std::atomic<bool> should_exit(false);

void sigterm_handler(int signal) {
    (void)signal;
    should_exit = true;
}

int main(void) {
    Logger::instance()->info("AA Wireless Dongle\n");

    struct sigaction sa = {};
    sa.sa_handler = sigterm_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);

    // Global init
    std::optional<std::thread> ueventThread =  UeventMonitor::instance().start();
    UsbManager::instance().init();
    BluetoothHandler::instance().init();

    ConnectionStrategy connectionStrategy = Config::instance()->getConnectionStrategy();
    if (connectionStrategy == ConnectionStrategy::DONGLE_MODE) {
        BluetoothHandler::instance().powerOn();
    }

    while (!should_exit) {
        Logger::instance()->info("Connection Strategy: %d\n", connectionStrategy);

        // Per connection setup and processing
        if (connectionStrategy == ConnectionStrategy::USB_FIRST) {
            Logger::instance()->info("Waiting for the accessory to connect first\n");
            UsbManager::instance().enableDefaultAndWaitForAccessory();
        }

        if (should_exit) break;

        AAWProxy proxy;
        std::optional<std::thread> proxyThread = proxy.startServer(Config::instance()->getWifiInfo().port);

        if (!proxyThread) {
            Logger::instance()->info("Proxy server failed to start, retrying in 2 seconds\n");
            sleep(2);
            continue;
        }

        if (connectionStrategy != ConnectionStrategy::DONGLE_MODE) {
            BluetoothHandler::instance().powerOn();
        }

        std::optional<std::thread> btConnectionThread = BluetoothHandler::instance().connectWithRetry();

        proxyThread->join();

        if (btConnectionThread) {
            BluetoothHandler::instance().stopConnectWithRetry();
            btConnectionThread->join();
        }

        UsbManager::instance().disableGadget();

        if (connectionStrategy != ConnectionStrategy::DONGLE_MODE && !should_exit) {
            // sleep for a couple of seconds before retrying
            sleep(2);
        }
    }

    if (ueventThread && ueventThread->joinable()) {
        ueventThread->detach();
    }

    Logger::instance()->info("Exiting AA Wireless Dongle\n");

    return 0;
}
