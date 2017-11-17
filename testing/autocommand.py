import socket
import time

TCP_IP = '192.168.1.75'
TCP_PORT = 1234
BUFFER_SIZE = 8

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))


#s.send("AT!MTR_CMD=GOTO,250,-250")
#data = s.recv(BUFFER_SIZE)

#s.send("AT!MTR_CMD=GOTO,-10,10")
#data = s.recv(BUFFER_SIZE)

#s.send("AT!MTR_CMD=GOTO,25,-25")
#data = s.recv(BUFFER_SIZE)

#s.send("AT!MTR_CMD=GOTO,-2,2")
#data = s.recv(BUFFER_SIZE)



for x in xrange(0, 50):
	time.sleep(0.0005)
	buf = "AT!MTR_CMD=GOTO,%d,30" % x
	print "sent:", buf, "\n"
	s.send(buf)
	data = s.recv(BUFFER_SIZE)
	print "received:", data


for x in range(50, 0, -1):
        time.sleep(0.0005)
        buf = "AT!MTR_CMD=GOTO,%d,30" % x
        print "sent:", buf, "\n"
        s.send(buf)
        data = s.recv(BUFFER_SIZE)
        print "received:", data



#for x in xrange(0, 50):
#        time.sleep(0.0005)
#        buf = "AT!MTR_CMD=GOTO,50,%d" % x
#        print "sent:", buf, "\n"
#        s.send(buf)
#        data = s.recv(BUFFER_SIZE)
#        print "received:", data

#for x in range(50, 0, -1):
#        time.sleep(0.0005)
#        buf = "AT!MTR_CMD=GOTO,%d,50" % x
#        print "sent:", buf, "\n"
#        s.send(buf)
#        data = s.recv(BUFFER_SIZE)
#        print "received:", data


#for x in range(50, 0, -1):
#        time.sleep(0.0005)
#        buf = "AT!MTR_CMD=GOTO,0,%d" % x
#        print "sent:", buf, "\n"
#        s.send(buf)
#        data = s.recv(BUFFER_SIZE)
#        print "received:", data


s.close()

