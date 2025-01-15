import bpy
for o in bpy.data.objects:
    if o.name in ['Cube']:
        o.select_set(True)