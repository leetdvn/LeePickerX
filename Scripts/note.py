#exec python file blender
filename = "C:/Users/thang/Documents/GitHub/LeePickerX/Scripts/testCommandPort.py"
exec(compile(open(filename).read(), filename, 'exec'))
#find port net Shell
#netstat -ano|findstr 54322

'''command Plugins'''
##bpy.ops.wm.open_command_port()

'''File target'''
#-y -P "C:\Users\leepl\Documents\GitHub\LeePickerX\build\Debug\startup\BlenderStartup.py"

'''Task Kill ID'''
#taskkill /F /PID 1111