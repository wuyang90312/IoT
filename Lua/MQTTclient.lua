-- init mqtt client with keepalive timer 120sec
m = mqtt.Client("ESP_MQTT_1", 120, "", "")

-- setup Last Will and Testament (optional)
-- Broker will publish a message with qos = 0, retain = 0, data = "offline" 
-- to topic "/lwt" if client don't send keepalive packet
m:lwt("/lwt", "offline", 0, 0)



m:on("connect", function(con)
    print ("connect")
     tmr.alarm(2, 10*1000, 1, function()
               print ("connected")
               end)
    end)
m:on("offline", function(con) 
     print ("reconnecting...") 
     tmr.alarm(1, 10*1000, 1, function()
                m:connect("192.168.2.201", 1884, 0, function(conn)
                print("reconnected")
                end)
              end)
    end)

-- on publish message receive event
m:on("message", function(conn, topic, data) 
      print("message-->"..topic .. ":" )
      if data ~= nil then
        print(data)
      end
    end)

-- for secure: m:connect("192.168.11.118", 1880, 1)
m:connect("192.168.2.201", 1884, 0, function(conn) 
            print("connection Done") 
            -- subscribe topic with qos = 0
            m:subscribe("sensors/temperature/TEMP_AD150023",0, function(conn) 
                          print("subscribe success") 
                          --m:publish("test","Connect with the broker",0,0, function(conn) print("sent") end)
                          m:subscribe("sensors/temperature/TEMP_AD150014",0, function(conn) print("subscribe success2") end)
                        end)
            -- publish a message with data = hello, QoS = 0, retain = 0
            --m:publish("test","hello world",0,0, function(conn) print("sent") end)
          end)



--m:close()
-- you can call m:connect again