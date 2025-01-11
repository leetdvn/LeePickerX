import socket

def PortIsOpen():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    result = sock.connect_ex(('127.0.0.1',54322))
    isOpen = False
    if result == 0:
        print("Port is open")
        isOpen = True
    else:
        print ("Port is not open")
    sock.close()
    return isOpen
    
print(PortIsOpen())
maya.standalone.uninitialize();
exit()