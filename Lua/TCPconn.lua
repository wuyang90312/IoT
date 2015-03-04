-- Build up a TCP connection with Thingspeak
function TCPconn(input)
  IP = "184.106.153.149"
  PORT = 80
  ALTER = true

  conn = nil
  conn = net.createConnection(net.TCP, 0) 

  -- show the retrieved web page

  conn:on("receive", function(conn, payload) 
                         print(payload)
                         end) 

  -- once connected, request page (send parameters to a php script)

  conn:on("connection", function(conn, payload) 
                          print('\nConnected')
                          if(ALTER) then
                            print("*********************The input value is: " ,input,"*************************")
                            conn:send("GET /update?key=GPVP0E6QQVWU47LZ&field1="..input.."\r\n")
                          else
                            conn:send("GET /channels/26355/feeds.json?results=1\r\n")
                          end
                         end) 

  -- when disconnected, let it be known
  conn:on("disconnection", function(conn, payload) 
                            print('\nDisconnected') 
                            end)

  conn:connect(PORT,IP)
end

local input = 10
tmr.alarm(1, 30*1000, 1,  function()
                            input = input + 2
                            TCPconn(input)
                          end)
