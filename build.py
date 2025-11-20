import os
import subprocess
import argparse

def run_command(command, cwd=None):
    """Runs a command in the shell."""
    try:
        subprocess.run(command, check=True, shell=True, cwd=cwd)
    except subprocess.CalledProcessError as e:
        print(f"Error executing command: {e}")
        exit(1)

def configure_cmake(build_dir="build"):
    """Configures the CMake project."""
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)
    run_command(f"cmake -S . -B {build_dir}", cwd=None)

def build_cmake(build_dir="build"):
    """Builds the CMake project."""
    run_command(f"cmake --build {build_dir}", cwd=None)

def main():
    parser = argparse.ArgumentParser(description="Chtholly build script.")
    parser.add_argument("action", choices=["build", "configure"], help="Action to perform.")
    args = parser.parse_args()

    if args.action == "configure":
        configure_cmake()
    elif args.action == "build":
        configure_cmake()
        build_cmake()

if __name__ == "__main__":
    main()
