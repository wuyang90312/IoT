-- Communicate with the Accelerometer of ADXL345

-- user defined function: read from reg_addr content of dev_addr
function read_reg(dev_addr, reg_addr)
  i2c.start(ID)
  i2c.address(ID, dev_addr ,i2c.TRANSMITTER)
  i2c.write(ID,reg_addr)
  i2c.stop(ID)
  i2c.start(ID)
  i2c.address(ID, dev_addr,i2c.RECEIVER)
  c=i2c.read(ID,1)
  i2c.stop(ID)
  return c
end

function write_reg(dev_addr, reg_addr, content)
  i2c.start(ID)
  i2c.address(ID, dev_addr ,i2c.TRANSMITTER)
  i2c.write(ID,reg_addr)
  num = i2c.write(ID,content)
  i2c.stop(ID)
  return num
end

-- set up the initial configuration of ADXL345 
function init_config(scale)
  i2c.setup(ID,SDA,SCL,i2c.SLOW)
  -- set the chip to a measurement mode
  res = write_reg(BASE_ADDR,PW_ADDR,0x08)
  -- put the right g scale of the measurement: 2g, 4g, 8g, and 16g
  res = write_reg(BASE_ADDR,FMT_ADDR,scale)
end

-- convert the received byte to an integer
function convertByte(input)
  conv = string.byte(input)
  conv = tonumber(conv)
  return conv
end

-- acquire the data of one axis
function acquireData(MSB_addr,LSB_addr)
  -- get content of the least significant byte
  LSB_data = read_reg(BASE_ADDR, LSB_addr)
  LSB_data = convertByte(LSB_data)
  -- get content of the most significant byte
  MSB_data = read_reg(BASE_ADDR, MSB_addr)
  MSB_data= convertByte(MSB_data)

  LSB_data = MSB_data*256+LSB_data
  if (MSB_data >= 128) then
    LSB_data = -(65536 - LSB_data);
  else
end
    LSB_data = LSB_data/COE_SCALE
  return LSB_data
end

----------------------------------------------------------------------------------------
ID=0              -- The communication channel ID
SDA=4             -- GPIO0
SCL=3             -- GPIO2
----------------------------------------------------------------------------------------
BASE_ADDR = 0x53  -- The base address when SDO/CTL pin is low
PW_ADDR = 0x2D    -- The address of power control
FMT_ADDR = 0x31   -- The address of data format
----------------------------------------------------------------------------------------
SCALE_BIT   =  0x01
COE_SCALE = 128 -- The scale coefficient for 4g
----------------------------------------------------------------------------------------

--init_config(SCALE_BIT)
-- Acquire data from x,y,z-axis and display it
--X = acquireData(0x33, 0x32)
--Y = acquireData(0x35, 0x34)
--Z = acquireData(0x37, 0x36)
--print(X.."  "..Y.."  "..Z)
--print(Y)
--print(Z)

