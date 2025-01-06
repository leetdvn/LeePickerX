import maya.standalone
maya.standalone.initialize(name='python')
import socket

HOST = '127.0.0.1'  # the local host
PORT = 54322  # The same port as used by the server
ADDR = (HOST, PORT)


def send_command():
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect(ADDR)
    command = 'cmds.polyCube()'

    message = command
    client.send(str.encode(message))
    data = client.recv(1024)  # receive the result info
    client.close()

    print(data)

send_command()