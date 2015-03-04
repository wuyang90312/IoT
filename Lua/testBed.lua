-- Test the API of I2Cconn

require "I2Cconn"

print("The start of the test")
init_config(SCALE_BIT)

tmr.alarm(1 ,1000, 1, function()
                        X = acquireData(0x33, 0x32)
                        Y = acquireData(0x35, 0x34)
                        Z = acquireData(0x37, 0x36)
                        print(X.."  "..Y.."  "..Z)
                      end)
