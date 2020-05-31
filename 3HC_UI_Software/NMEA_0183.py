# /* ========================================
#  * FILE:   NMEA_0183.py
#  * AUTHOR: Nick Jannuzzi
#  *
#  * CREATED ON April 13, 2020, 4:10 PM
#  *
#  * ========================================
#  *
#  * Copyright SLUGSAT, 2020
#  * All Rights Reserved
#  * UNPUBLISHED, LICENSED SOFTWARE.
#  *
#  * CONFIDENTIAL AND PROPRIETARY INFORMATION
#  * WHICH IS THE PROPERTY OF SLUGSAT.
#  *
#  * ========================================
# */



import re
#encodes a message and payload using NMEA_0183 GPS protocol
def Encode(msgid, payload):
	a = str(msgid) + ',' + str(payload)
	
	chk = Checksum(a)
	m = '$'+str(a)+'*'+str(hex(chk)[2:])+"\n"
	# print(m+'\n')
	return m

#calculates bytewise XOR checksum of id and payload
def Checksum(string):
	checksum = 0
	j = 0
	for i in string:
		checksum ^= ord(string[j])
		j+=1
	return checksum	


#decodes NMEA_0183 Messages and returns a list containing the separated id, payload and checksum
def Decode(string):
	# print("Before:",string)
	msg = re.split('[$ , * \n]',string)
	#trims some leading and trailing $ and \n 
	msg.pop(0)
	msg.pop(-1)

	#current sensor had weird hex padding, so this will trim anything else that we don't want
	if(len(msg) > 3):
		while(len(msg) > 3):
			msg.pop(-1)	
	# print("AfterTrim:",msg)

	#save ID, payload and checksum into single variables from tuple
	msgid = msg[0]
	payload = msg[1]
	chk = int(msg[-1],16)


	#checksum comparison determines if the message is valid or not
	if(chk != Checksum(str(msgid) + ',' + str(payload))):
		# print("Checksum error, expected checksum did not match received checksum\n")
		msg = ["ERR"]
		return msg	
	return msg




