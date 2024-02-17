import os
import shutil

def run_cmd(cmd):
    print(cmd)
    os.system(cmd)

def folder_exists(folder):
    if not os.path.exists(folder):
        os.makedirs(folder)

def copy_folder(src, dst):
    shutil.rmtree(dst, ignore_errors=True)
    shutil.copytree(src, dst)

def main():
    print("Packing Prism Engine Editor")

    # run cmake build
    run_cmd("cd build && cmake --build . --config=Release --target=editor && cd ..")

    folder_exists("release")
    folder_exists("release/assets")
    folder_exists("release/editor")
    folder_exists("release/editor/assets")

    shutil.copy("build/editor/editor.exe", "release/editor.exe")
    shutil.copy("editor/imgui.ini", "release/editor/imgui.ini")


    # copy folders
    copy_folder("assets", "release/assets")
    copy_folder("editor/assets", "release/editor/assets")
    print("Packing Prism Engine Editor done")

if __name__ == "__main__":

    main()