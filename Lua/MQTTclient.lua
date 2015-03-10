require "I2Cconn"
-- init mqtt client with keepalive timer 120sec
m = mqtt.Client("ESP_MQTT_1", 120, "", "")

-- setup Last Will and Testament (optional)
-- Broker will publish a message with qos = 0, retain = 0, data = "offline" 
-- to topic "/lwt" if client don't send keepalive packet
m:lwt("/lwt", "offline", 0, 0)



m:on("connect", function(con) print ("connect") end)
m:on("offline", function(con) 
     print ("reconnecting...") 
     --tmr.alarm(1, 10*1000, 1, function()
                --m:connect("192.168.2.201", 1884, 0, function(conn)
                --print("reconnected")
                --end)
              --end)
    end)

-- on publish message receive event
m:on("message", function(conn, topic, data) 
      print("message-->"..topic .. ":" )
      if data ~= nil then
        print(data)
      end
    end)

-- for secure: m:connect("192.168.11.118", 1880, 1)
--m:connect("192.168.2.201", 1884, 0, function(conn) 
 --           print("connection Done") 
            -- subscribe topic with qos = 0
            --m:subscribe("sensors/temperature/TEMP_AD150023",0, function(conn) print("success 1") end)
            -- publish a message with data = hello, QoS = 0, retain = 0
              
            --m:publish("test","hello world",0,0, function(conn) print("sent") end)
 --         end)
index = 0
state = false
--step = 0 
tmr.alarm(2, 3*1000, 1, function()
                      index = index +1
                      if index == 1 then
                        m:connect("192.168.2.201", 1884, 0, function(conn) print("connection Done") end)
                      elseif index <= 2 then
                        print("wait a term")
                      elseif index <= 7 then
                        dest = "sensors/temperature/TEMP_AD"..index
                        m:subscribe(dest,0, function(conn) print("success") end)
                      else
                        tmr.stop(2)
                        index = 0
                        init_config(SCALE_BIT)
                        state = true
                      end
          end)

tmr.alarm(3, 10*1000, 1, function()
      if state then
        
        X = acquireData((0x33+index), (0x32+index))
        --Y = acquireData(0x35, 0x34)
        --Z = acquireData(0x37, 0x36)
        m:publish("test", X,0,0, function(conn) 
            print("sent"..index) 
            index = (index+2)%6
            
            end)
        
      else
        print("empty")
      end
    end)
--m:close()
-- you can call m:connect again