from USB import * 

test = USB()
test.verify()
t = input()
l = len(t)
#test.OUT.write(t)
test.writeUSB(t)
#print("Received: " + str(test.IN.read(l))) # receive the echo
print("Received: " + str(test.readUSB(l))) # receive the echo
