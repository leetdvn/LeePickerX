#exec python file blender
filename = "C:/Users/thang/Documents/GitHub/LeePickerX/Scripts/testCommandPort.py"
exec(compile(open(filename).read(), filename, 'exec'))
#find port net Shell
#netstat -ano|findstr 54322

'''command Plugins'''
##bpy.ops.wm.open_command_port()

'''File target'''
#-y -P "C:\Users\thang\Documents\GitHub\LeePickerX\Scripts\BlenderStartup.py"