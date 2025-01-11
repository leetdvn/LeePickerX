import socket

HOST = '127.0.0.1'  # the local host
PORT = 54322  # The same port as used by the server
ADDR = (HOST, PORT)


def send_command(command=str):
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect(ADDR)
    message = command
    client.send(str.encode(message))
    data = client.recv(1024)  # receive the result info
    client.close()
    return data.decode()

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

def PickerSelect(inObjects):
    command = str("cmds.select({},r=1)").format(inObjects)
    send_command(command)

def PickerAddSelect(inObjects):
    command = str("cmds.select({},add=1)").format(inObjects)
    send_command(command)    

def PickerDeSelect(inObjects):
    command = str("cmds.select({},d=1)").format(inObjects)
    send_command(command) 

def PickerClearSelection():
    command = str("cmds.select(cl=1)")
    send_command(command)  