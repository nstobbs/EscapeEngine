import os, sys, subprocess
# check that the GLSL system environment is set
try:
    glslcPath = os.environ["GLSLC_PATH"]
except KeyError:
    sys.exit("GLSLC_PATH needs to be set before compiling shaders!")

# Find all of the Shaders files *.frag *.vert
fragmentShaderPaths = []
vertexShaderPaths = []
for file in os.listdir("./"):
    if file.endswith(".frag"):
        fragmentShaderPaths.append(file)
    elif file.endswith(".vert"):
        vertexShaderPaths.append(file)
    
# Try Compiling all of the shaders within the list.
# If the file-extension is .frag add Frag to the shader name
for shader in fragmentShaderPaths:
    out = shader.split('.', 1)[0] + "Frag.spv"
    command = "{GLSLC} {sourcePath} -o {compiledPath}".format(GLSLC=glslcPath, sourcePath=shader, compiledPath=out)
    print("Running command...: " + command)
    exe = subprocess.run(command, shell=True, check=True, capture_output=True, encoding="utf-8")
    print(f"Command {exe.args} exited with {exe.returncode} code, output: \n{exe.stdout}")

# If the file-extension is .vert add Vert to the shader name
for shader in vertexShaderPaths:
    out = shader.split('.', 1)[0] + "Vert.spv"
    command = "{GLSLC} {sourcePath} -o {compiledPath}".format(GLSLC=glslcPath, sourcePath=shader, compiledPath=out)
    print("Running command...: " + command)
    exe = subprocess.run(command, shell=True, check=True, capture_output=True, encoding="utf-8")
    print(f"Command {exe.args} exited with {exe.returncode} code, output: \n{exe.stdout}")