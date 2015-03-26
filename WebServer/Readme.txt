<WebServer.lua> is the initial example file from the Internet, which tells the mechanism of hosting a webserver
on ESP8266. I have modified a bit the code in order to access 192.168.4.1(IP of the AP mode). Hence, user can
access it as either AP(192.168.4.1) or STA(192.168.2.XXX).

-----------------------------------------------------------------------------------------------------------------
<Communication.cpp> is a C++/CLI file, which can be run on the Visual Studio. This code helps us further understand
how the HTTP connection b/t ESP8266 and device works. 
Here, when device send a HTTP request:
(**We have to type 192.168.4.1:333 here if you have not set up the listening port)

1. the TCP connection won't be killed immediately after the HTTP is received by ESP board; instead, it will hold.
2. ESP board can return the HTML code back to the device.
3. Step 2 is not the end of story. At the end, we have to send "AT+CIPCLOSE=CHANNELID"; otherwise, the device will
expect more incoming message. Here, it took me quite a while to figure it out.