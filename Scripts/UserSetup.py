import maya.cmds as cmds
cmds.commandPort(name=":54321", sourceType="mel")
cmds.commandPort(name=":54322", sourceType="python")