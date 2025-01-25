import os, sys, subprocess

try:
    GLSLC_PATH = os.environ["GLSLC_PATH"]
except KeyError:
    sys.exit("GLSLC_PATH was't set!")

shaderPaths = []
for file in os.listdir("./"):
    if file.endswith(".frag") or file.endswith(".vert"):
        print("Found Shader: {filename}".format(filename=file))
        shaderPaths.append(file)

if shaderPaths == []:
    sys.exit("Found no Shaders!")

for sourcePath in shaderPaths:
    if sourcePath.endswith(".frag"):
        outputFilename = sourcePath.split('.', 1)[0] + "Frag.spv"
    elif sourcePath.endswith(".vert"):
        outputFilename = sourcePath.split('.', 1)[0] + "Vert.spv"
    
    compileCommand = "{compilerPath} {sourcePath} -o {outputPath}".format(compilerPath=GLSLC_PATH, sourcePath=sourcePath, outputPath=outputFilename)
    try:        
        runCommand = subprocess.run(compileCommand, shell=True, check=True, capture_output=True, encoding="utf-8")
        print(f"Command {runCommand.args} exited with {runCommand.returncode} code, output: \n{runCommand.stdout}")
    except subprocess.CalledProcessError as error:
        print(f"Command Failed!")
        print(error.output)