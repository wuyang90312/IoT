--require "MQTTclient"
--init.lua
print("Setting up WIFI...")
print(wifi.setmode(wifi.STATIONAP))
--modify according your wireless router settings
wifi.sta.config("BCRLovs","23456")
wifi.sta.connect()
tmr.alarm(1, 1000, 1, function() 
  if wifi.sta.getip()== nil then 
  print("IP unavaiable, Waiting...") 
  else 
  tmr.stop(1)
  print("Config done, IP is "..wifi.sta.getip())
  --dofile("TCPconn.lua")
  dofile("MQTTclient.lua") -- subcribe and publish to the MQTT broker
  end 
end)
