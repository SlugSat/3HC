# /* ========================================
#  * FILE:   NMEA0183.py
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
	# print("AfterTrim:",msg)
	# msgid.pop(-1)
	msgid = msg[1]
	payload = msg[2]
	chk = int(msg[-2],16)


	#checksum comparison determines if the message is valid or not
	# print(chk)
	# print(Checksum(str(msgid) + ',' + str(payload)))
	if(chk != Checksum(str(msgid) + ',' + str(payload))):
		# print("Checksum error, expected checksum did not match received checksum\n")
		msg = ["ERR", 0, 0]
		return msg	
	return msg[1:3]




