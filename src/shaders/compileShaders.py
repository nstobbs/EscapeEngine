# compileShaders.py 
"""
Python Script for Compiling Shaders
By Nathan Stobbs
"""
import os, sys
# check that the GLSL system enviorment is set
try:
    glslcPath = os.environ["GLSLC_PATH"]
execpt KeyError:
    sys.exit("GLSLC_PATH needs to be set before compiling shaders!")
# What out what OS we are if needed.

# Find all of the Shaders files *.frag *.vert
fragmentShaderPaths = []
vertexShaderPaths = []
for file in os.listdir("./"):
    if file.endswith(".frag"):
        fragmentShaderPaths.append(file)
    elif file.endswith(".vert"):
        vertexShaderPaths.append(file)
    
# Try Compiling all of the shaders within the list.

# If the file-extenion is .frag add Frag to the shader name
# If the file-extenion is .vert add Vert to the shader name

