import socket

HOST = '127.0.0.1'  # the local host
PORT = 5000  # The same port as used by the server
ADDR = (HOST, PORT)

def send_command(command=str):
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect(ADDR)
    message = command
    client.send(str.encode(message))
    data = client.recvfrom(1024)  # receive the result info
    client.close()
    return data.decode()

def PortIsOpen():
    return True
    # try:
    #     client.send("print('abc')")
    #     return True
    # except:
    #     return False
    
    
def PickerSelect(inObjects):
    if not PortIsOpen(): return
    command =str('''
    for o in bpy.data.objects:
        if o.name in {}:
            o.select_set(True)
    ''').format(inObjects)
    #command = str("cmds.select({},r=1)").format(inObjects)
    send_command(command)

def PickerAddSelect(inObjects):
    #check port Open
    if not PortIsOpen(): return
    command = str("cmds.select({},add=1)").format(inObjects)
    send_command(command)    

def PickerDeSelect(inObjects):
    command =str('''
    try : bpy.ops.object.mode_set(mode = "OBJECT")
    except : pass
    for obj in inObjects:
        if obj in inObjects:
            obj.select_set(True)
    ''').format(inObjects)
    print(command)
    send_command(command) 

def PickerClearSelection():

    cmd='''bpy.ops.object.select_all(action='DESELECT')'''
    return send_command(cmd)