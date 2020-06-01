# /* ========================================
#  * FILE:   USB.py
#  * AUTHOR: Nick Jannuzzi
#  *
#  * CREATED ON March 3, 2020
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

import usb.core
import usb.util
import sys
import array

		

#implements the USB class for allowing USB transfer (Primary USB connection for usage with 3HC)
class USBprimary:
	def __init__(self):
		self.OUT = None
		self.IN = None
		self.dev = None
	#find our device
		self.dev = usb.core.find(idVendor = 0x4b4, idProduct=0x80)
		#check if we found a device
		if self.dev is None:
			print("USB Device error: Device 1 not found: It appears the HHC is not connected or turned on. Please plug the power supply in or turn it on if it is already plugged in. ")
			sys.exit(1)
			
		self.dev.set_configuration()
		cfg = self.dev.get_active_configuration()
		intf = cfg[(0,0)]
		epOut = usb.util.find_descriptor(
			intf, 
			custom_match = \
			lambda e: \
				usb.util.endpoint_direction(e.bEndpointAddress) == \
		        usb.util.ENDPOINT_OUT)	
		self.OUT = epOut	

		epIn = usb.util.find_descriptor(
			intf,
			custom_match= \
			lambda e: \
				usb.util.endpoint_direction(e.bEndpointAddress) == \
				usb.util.ENDPOINT_IN)
		self.IN = epIn	
		#check endpoints, return true if both are found
		

	#verifies that we connected because __init__ method cannot return something 	
	def verify(self):

		if self.IN is not None and self.OUT is not None:
			return True
		else: 
			return False	


	#read IN endpoint from device and processes it so we 
	def readUSB(self):
		#pyusb returns array of bytes so this converts it to something we can use
		#conditionals catch errors and allow us to react to them
		if(self.IN != None):
			try:
				wrapped = self.IN.read(64,1000).tobytes().decode()
				# print(wrapped)
			except usb.core.USBError as e:
				wrapped = None
				# print(wrapped)

				# if e.args == ('Operation timed out',):
					# continue
					
			# print("Received:" +str(wrapped))
			return wrapped
		


	def writeUSB(self,writing):	
		# print("Sent: "+str(writing))
		# print(writing)
		if(self.OUT != None):
			self.OUT.write(writing,1000) # send it

	#resets device without disconnecting it completely
	def resetUSB(self):
		self.dev.reset()


	#terminate USB connection and reset device	
	def EndUSB(self):
		self.IN = None
		self.OUT = None
		self.dev.reset()





#setup for a secondary USB device, to be used exclusively by the Solar Vector Simulation Module of the HHC
#to configure properly, idVendor and idProduct will need to be set to match the microncontroller used
class USBsecondary:
	def __init__(self):
		self.OUT = None
		self.IN = None
		self.dev = None
	#find our device
		self.dev = usb.core.find(idVendor = 0x4b4, idProduct=0x80)
		#check is we found a device
		if self.dev is None:
			print("USB Device error: Device 2 not found: It appears the HHC is not connected or turned on. Please plug the power supply in or turn it on if it is already plugged in. ")
			sys.exit(1)
			
		self.dev.set_configuration()
		cfg = self.dev.get_active_configuration()
		intf = cfg[(0,0)]
		epOut = usb.util.find_descriptor(
			intf, 
			custom_match = \
			lambda e: \
				usb.util.endpoint_direction(e.bEndpointAddress) == \
		        usb.util.ENDPOINT_OUT)	
		self.OUT = epOut	

		epIn = usb.util.find_descriptor(
			intf,
			custom_match= \
			lambda e: \
				usb.util.endpoint_direction(e.bEndpointAddress) == \
				usb.util.ENDPOINT_IN)
		self.IN = epIn	
		#check endpoints, return true if both are found
		

	#verifies that we connected because __init__ method cannot return something 	
	def verify(self):

		if self.IN is not None and self.OUT is not None:
			return True

	#read IN endpoint from device and processes it so we 
	def readUSB(self):
		wrapped = self.IN.read(64, 1000).tobytes().decode()
		# print(wrapped)
		return wrapped
		


	def writeUSB(self,writing):	
		# print("Sent: "+str(writing))
		self.OUT.write(writing) # send it




	


