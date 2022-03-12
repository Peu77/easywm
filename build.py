import os

if not os.path.exists('build'):
    os.mkdir('build')

os.chdir('build')

if not os.system('cmake ..'):
    print("CMake has been executed successfully")

if not os.system('make'):
    print("Make has been executed successfully")

print("Compilation complete")

if not os.system('DISPLAY=:1 alacritty &'):
    print("start alacritty")

if not os.system('DISPLAY=:1 ./easywm'):
    print("start")

