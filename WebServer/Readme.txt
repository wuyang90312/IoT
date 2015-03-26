The purpose of this folder is to set up a Web browser as user interface as well as to explore the possibility of storing data permanently. Therefore, users can configure the setting of ESP board and reboot the board with the same configuration.

-----------------------------------------------------------------------------------------------------------------
<WebServer.lua> is the initial example file from the Internet, which tells the mechanism of hosting a webserver on ESP8266. I have modified a bit the code in order to access 192.168.4.1(IP of the AP mode). Hence, user can access it as either AP(192.168.4.1) or STA(192.168.2.XXX).

-----------------------------------------------------------------------------------------------------------------
<Communication.cpp> is a C++/CLI file, which can be run on the Visual Studio. This code helps us further understand how the HTTP connection b/t ESP8266 and device works. Here, when device send a HTTP request:
(**We have to type 192.168.4.1:333 here if you have not set up the listening port)

1. the TCP connection won't be killed immediately after the HTTP is received by ESP board; instead, it will hold.
2. ESP board can return the HTML code back to the device.
3. Step 2 is not the end of story. At the end, we have to send "AT+CIPCLOSE=CHANNELID"; otherwise, the device will expect more incoming message. Here, it took me quite a while to figure it out.

----------------------------------------------------------------------------------------------------------------
The PRAM.ino only tries to verify the functionalites of EEPROM. The total size varies with different models, but we only need to remeber PROM size of two models: 1 KB (ATmega32u4) and 4 KB (Mega2560)

Since each position of the EEPROM is designed to have 1 byte memory (0-255 in integer). This configuration satisfies our spatial requirement for IP's(0-255) and ASCII(0-255).

----------------------------------------------------------------------------------------------------------------
EEPROM DATA STRUCTURE
BYTE 0: CONFIGURATION BYTE => 0(CONFIGURED))/255(NOT CONFIGURED)
BYTE 1-2: POSITION BYTES OF SSID => BYTE 1 contains the LSB 8 bits, BYTE 2 contains the rest of MSB bits. In a 1KB memory, they can concatenate together to a 10 bit number to indicate the initial position of SSID.
BYTE 3: LENGTH BYTEES OF SSID => give the length of SSID.
BYTE 4-5: POSITION BYTES OF PWD => BYTE 1 contains the LSB 8 bits, BYTE 2 contains the rest of MSB bits. In a 1KB memory, they can concatenate together to a 10 bit number to indicate the initial position of SSID.
BYTE 6: LENGTH BYTEES OF PWD=> give the length of PWD.

** SSID and PWD here is the one to associate with.

BYTE 7: AP/STA MODE BYTE => 0(STA)/1(AP)/2(DUAL MODE)
BYTE 8-11: STA IP => The IP of ESP itself as a station. For instance, [192] [168] [2] [102]
BYTE 12-15: MQTT IP => The IP of MQTT broker
BYTE 16-17: MQTT PORT NUMBER => byte 16 has to LSB byte.
BYTE 18-21: CLOUD IP => The IP of cloud
BYTE 22-23: CLOUD PORT NUMBER => byte 16 has to LSB byte.

For now,
BYTE 24-(23+SIZE OF X): SSID => each byte is an ASCII character
BYTE (24+SIZE OF X) - (23+SIZE OF X + SIZE OF Y): password