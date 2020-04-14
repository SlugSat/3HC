import re
import numpy as num


#encodes a message and payload using NMEA_0183 GPS protocol
def Encode(msgid, payload):
	a = str(msgid) + ',' + str(payload)
	
	chk = Checksum(a)
	m = '$'+str(a)+'*'+str(hex(chk)[2:])+"\n"
	print(m+'\n')
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
	msgid = re.split('[$ , * \n]',string)
	#trims some leading and trailing $ and \n 
	msgid.pop(0)
	msgid.pop(-1)
	print(msgid)
	return msgid




