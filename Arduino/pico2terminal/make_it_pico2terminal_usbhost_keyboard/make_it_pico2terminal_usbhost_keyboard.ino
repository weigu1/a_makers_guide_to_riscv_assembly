// make_it_pico2terminal_usbhost_keyboard.ino
// Code from https://learn.adafruit.com/using-a-keyboard-with-usb-host/arduino
// altered by weigu.lu
// This sketch uses the Adafruit_TinyUSB library and the Pico-PIO-USB library
// from sekigon-gonnoc, both licensed under MIT License
// Original libraries: https://github.com/adafruit/Adafruit_TinyUSB_Arduino, 
// https://github.com/sekigon-gonnoc/Pico-PIO-USB
//
// SPDX-FileCopyrightText: 2025 Tim Cocks for Adafruit Industries
// SPDX-License-Identifier: MIT
//
// Adafruit invests time and resources providing this open source code,
//  please support Adafruit and open-source hardware by purchasing
// products from Adafruit!
// MIT license, check LICENSE for more information
// Copyright (c) 2019 Ha Thach for Adafruit Industries
// All text above, and the splash screen below must be included in
// any redistribution

#include "pio_usb.h"
#include "Adafruit_TinyUSB.h"
#include "tusb.h"

#define PIN_USB_HOST_DP  14 // Pin D+ for host, D- = D+ + 1

Adafruit_USBH_Host USBHost;

bool printed_blank = false;

void setup() {
  Serial.begin(115200);
  Serial2.setRX(5);
  Serial2.setTX(4);    
  Serial2.begin(1000000); 
  rp2040_configure_pio_usb();   // configure pio-usb: defined in usbh_helper.h
  USBHost.begin(1);
  delay(3000);
  Serial.print("USB D+ Pin:");
  Serial.println(PIN_USB_HOST_DP); 
}
void loop() {
  USBHost.task();
  Serial.flush();
  Serial2.flush();
}

/****** HID Host Callback Functions  ******/
void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len) {
  Serial.printf("HID device mounted (address %d, instance %d)\n", dev_addr, instance);  
  if (!tuh_hid_receive_report(dev_addr, instance)) { // Start receiving HID reports
    Serial.printf("Error: cannot request to receive report\n");
  }
}
void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
  Serial.printf("HID device unmounted (address %d, instance %d)\n", dev_addr, instance);
}
void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len) {
      if (len > 0){
        printKeyboardReport(report);
      }
  if (!tuh_hid_receive_report(dev_addr, instance)) {   // Continue to receive the next report
    Serial.println("Error: cannot request to receive report");
  }
}

/****** Helper functions ******/
void printKeyName(uint8_t key) {
  switch (key) {
    case 0x1E: Serial.print("1"); Serial2.print("1"); break;
    case 0x1F: Serial.print("2"); Serial2.print("2"); break;
    case 0x20: Serial.print("3"); Serial2.print("3"); break;
    case 0x21: Serial.print("4"); Serial2.print("4"); break;
    case 0x22: Serial.print("5"); Serial2.print("5"); break;
    case 0x23: Serial.print("6"); Serial2.print("6"); break;
    case 0x24: Serial.print("7"); Serial2.print("7"); break;
    case 0x25: Serial.print("8"); Serial2.print("8"); break;
    case 0x26: Serial.print("9"); Serial2.print("9"); break;
    case 0x27: Serial.print("0"); Serial2.print("0"); break;
    case 0x04: Serial.print("a"); Serial2.print("a"); break;
    case 0x05: Serial.print("b"); Serial2.print("b"); break;
    case 0x06: Serial.print("c"); Serial2.print("c"); break;
    case 0x07: Serial.print("d"); Serial2.print("d"); break;
    case 0x08: Serial.print("e"); Serial2.print("e"); break;
    case 0x09: Serial.print("f"); Serial2.print("f"); break;

    case 0x0A: Serial.print("g"); Serial2.print("g"); break;
    case 0x0B: Serial.print("h"); Serial2.print("h"); break;
    case 0x0C: Serial.print("i"); Serial2.print("i"); break;
    case 0x0D: Serial.print("j"); Serial2.print("j"); break;
    case 0x0E: Serial.print("k"); Serial2.print("k"); break;
    case 0x0F: Serial.print("l"); Serial2.print("l"); break;
    case 0x10: Serial.print("m"); Serial2.print("m"); break;
    case 0x11: Serial.print("n"); Serial2.print("n"); break;
    case 0x12: Serial.print("o"); Serial2.print("o"); break;
    case 0x13: Serial.print("p"); Serial2.print("p"); break;
    case 0x14: Serial.print("q"); Serial2.print("q"); break;
    case 0x15: Serial.print("r"); Serial2.print("r"); break;
    case 0x16: Serial.print("s"); Serial2.print("s"); break;
    case 0x17: Serial.print("t"); Serial2.print("t"); break;
    case 0x18: Serial.print("u"); Serial2.print("u"); break;
    case 0x19: Serial.print("v"); Serial2.print("v"); break;
    case 0x1A: Serial.print("w"); Serial2.print("w"); break;
    case 0x1B: Serial.print("x"); Serial2.print("x"); break;
    case 0x1C: Serial.print("y"); Serial2.print("y"); break;
    case 0x1D: Serial.print("z"); Serial2.print("z"); break;
    case 0x28: Serial.print("ENTER"); Serial2.print("R"); break;
    case 0x29: Serial.print("ESC"); Serial2.print("E"); break;
    case 0x2A: Serial.print("BACKSPACE"); Serial2.print("B"); break;
    case 0x2B: Serial.print("TAB"); Serial2.print("T"); break;
    case 0x2C: Serial.print("SPACE"); Serial2.print("S"); break;
    case 0x2D: Serial.print("MINUS"); Serial2.print("M"); break;
    case 0x2E: Serial.print("EQUAL"); Serial2.print("A"); break;
    case 0x2F: Serial.print("LBRACKET"); Serial2.print("("); break;
    case 0x30: Serial.print("RBRACKET"); Serial2.print(")"); break;
    case 0x31: Serial.print("BACKSLASH"); Serial2.print("/"); break;
    case 0x33: Serial.print("SEMICOLON"); Serial2.print(";"); break;
    case 0x34: Serial.print("QUOTE"); Serial2.print("A"); break;
    case 0x35: Serial.print("GRAVE"); Serial2.print("A"); break;
    case 0x36: Serial.print("COMMA"); Serial2.print("A"); break;
    case 0x37: Serial.print("PERIOD"); Serial2.print("A"); break;
    case 0x38: Serial.print("SLASH"); Serial2.print("A"); break;
    case 0x39: Serial.print("CAPS_LOCK"); Serial2.print("A"); break;
    case 0x4F: Serial.print("RIGHT_ARROW"); Serial2.print("A"); break;
    case 0x50: Serial.print("LEFT_ARROW"); Serial2.print("A"); break;
    case 0x51: Serial.print("DOWN_ARROW"); Serial2.print("A"); break;
    case 0x52: Serial.print("UP_ARROW"); Serial2.print("A"); break;
    default:
      if (key >= 0x3A && key <= 0x45) { // F1-F12
        Serial.print("F");
        Serial.print(key - 0x3A + 1);
      } else {
        // For keys not handled above, just print the HID code
        Serial.print("0x");
        Serial.print(key, HEX);
      }
      break;
  }
}

void printKeyboardReport(uint8_t const* report) {  
  uint8_t modifiers = report[0];  // First byte contains modifier keys  
  if (modifiers > 0) { // Print modifier keys if pressed
    Serial.print("Modifiers: ");    
    if (modifiers & 0x01) Serial.print("LEFT_CTRL ");
    if (modifiers & 0x02) Serial.print("LEFT_SHIFT ");
    if (modifiers & 0x04) Serial.print("LEFT_ALT ");
    if (modifiers & 0x08) Serial.print("LEFT_GUI ");
    if (modifiers & 0x10) Serial.print("RIGHT_CTRL ");
    if (modifiers & 0x20) Serial.print("RIGHT_SHIFT ");
    if (modifiers & 0x40) Serial.print("RIGHT_ALT ");
    if (modifiers & 0x80) Serial.print("RIGHT_GUI ");    
    Serial.println();
  }  
  bool keysPressed = false;// 2. byte is reserved (def. 0), Bytes 2-7 contain up to 6 key codes
  for (int i = 2; i < 8; i++) {
    uint8_t key = report[i];
    if (key == 0 || key == 1) {     // Skip if no key or error rollover
      continue;
    }
    if (!keysPressed) {
      keysPressed = true;
    }
    printKeyName(key);     // Print key name based on HID Usage Tables for standard keyboard    
  }
}

static void rp2040_configure_pio_usb(void) {
  Serial.println("Core1 setup to run TinyUSB host with pio-usb");
  pio_usb_configuration_t pio_cfg = PIO_USB_DEFAULT_CONFIG;
  pio_cfg.pin_dp = PIN_USB_HOST_DP;
  USBHost.configure_pio_usb(1, &pio_cfg);
}
