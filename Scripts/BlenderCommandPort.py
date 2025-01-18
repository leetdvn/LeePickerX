import socket

HOST = '127.0.0.1'  # the local host
PORT = 5000  # The same port as used by the server
ADDR = (HOST, PORT)

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

def send_command(command=str):
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect(ADDR)
    message = command
    client.send(str.encode(message))
    data = client.recv(1024)  # receive the result info
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
    command =str('''import bpy\nfor o in bpy.data.objects:\n\tif o.name in {}:\n\t\to.select_set(True)''').format(inObjects)
    print(command)
    send_command(command)

def PickerAddSelect(inObjects):
    #check port Open
    if not PortIsOpen(): return
    command = str("cmds.select({},add=1)").format(inObjects)
    send_command(command)    

def PickerDeSelect(inObjects):
    command =str('''import bpy\nfor o in bpy.data.objects:\n\tif o.name in {}:\n\t\to.select_set(False)''').format(inObjects)
    print(command)
    send_command(command) 

def PickerClearSelection():
    cmd='''bpy.ops.object.select_all(action='DESELECT')'''
    try:
        send_command(cmd)
    except socket.error: print("connection false: Error:Port not found exec Mel commandPort -name \"localhost:54322\" -sourceType \"python\";")

def PickerGetSelection():
    command="import bpy\nselected_obj = [obj.name for obj in bpy.context.selected_objects]\nprint(selected_obj)"
    
    return send_command(command)
