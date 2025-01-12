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

cmd = '''
import bpy
bpy.data.objects["Cube"].select_set(True)
print("taoday")
'''
cmd2='''
print("taoday")
'''
print(send_command(cmd))