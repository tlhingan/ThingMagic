#All item for the config file are in a specific order and should not be changed
#1. Reader ID
#2. IP Address that the data will be sent to.
#3. First part of the string sent to the IP
#4. Power level we want the reader to operate at. 
#5. String representation of the the location of the reader/readers we want
#Otherwise the application will ignore any and all lines beginning with the hash (#) sign
#Reader ID
#This is the name you'd like the reader to identify itself as
DEFAULTCHANGEME
#IP to send the data packet to. 
#This is the IP address you'd like to send the data to
127.0.0.1
#First part of the URL to attach to the IP
/alma/read?readerId=
#The power level for the device that can be set here for ease of use
#Valid values between 500 and 3000 with 2100 being 25% power.
#Power values are as follows:
#3000 = 100% (DEFAULT)
#2875 = 75%
#2700 = 50%
#2400 = 25%
#2000 = 10%
#1000 = 1%
3000
#Reader location
tmr:///dev/ttyACM0