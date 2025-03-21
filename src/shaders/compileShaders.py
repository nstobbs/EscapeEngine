import os, sys, subprocess
"""
compileShaders.py

Script for compiling all of the glsl source
code files with .vert or .frag file extension
in the current directory. 

Requirements:
    Requires VulkanSDK 1.3 or above.

    Set System Environment Variable "GLSLC_PATH" to Vulkan's SDK GLSL Compiler.
        Can be found near "{VulkanSDK Install Location}/x.x.xxx/Bin/glslc"
"""

if sys.platform == "linux":
    GLSLC_PATH = "glslc"
else:
    try:
        GLSLC_PATH = os.environ["GLSLC_PATH"]
    except KeyError:
        sys.exit("GLSLC_PATH was't set!")

shaderPaths = []
for file in os.listdir("./"):
    if file.endswith(".frag") or file.endswith(".vert") or file.endswith(".comp"):
        print("Found Shader: {filename}".format(filename=file))
        shaderPaths.append(file)

if shaderPaths == []:
    sys.exit("Found no Shaders!")

for sourcePath in shaderPaths:
    if sourcePath.endswith(".frag"):
        outputFilename = sourcePath.split('.', 1)[0] + "Frag.spv"
    elif sourcePath.endswith(".vert"):
        outputFilename = sourcePath.split('.', 1)[0] + "Vert.spv"
    elif sourcePath.endswith(".comp"):
        outputFilename = sourcePath.split('.', 1)[0] + "Comp.spv"

    compileCommand = "{compilerPath} {sourcePath} -o {outputPath}".format(compilerPath=GLSLC_PATH, sourcePath=sourcePath, outputPath=outputFilename)
    try:        
        runCommand = subprocess.run(compileCommand, shell=True, check=True, capture_output=True, encoding="utf-8")
        print(f"Command Finished: {runCommand.args} exited with {runCommand.returncode} code, output: \n{runCommand.stdout}")
    except subprocess.CalledProcessError as error:
        print(f"Command Failed:")
        print(error.stderr)