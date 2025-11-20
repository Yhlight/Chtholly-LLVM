import os
import subprocess
import sys

def run_command(command):
    """Runs a command and prints its output."""
    process = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    for line in iter(process.stdout.readline, b''):
        print(line.decode('utf-8', errors='ignore').strip())
    process.stdout.close()
    return process.wait()

def main():
    """Configures and builds the CMake project."""
    build_dir = 'build'

    # Create build directory if it doesn't exist
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    # Configure CMake
    cmake_configure_command = f'cmake -S . -B {build_dir}'
    if run_command(cmake_configure_command) != 0:
        print("CMake configuration failed.")
        sys.exit(1)

    # Build the project
    cmake_build_command = f'cmake --build {build_dir}'
    if run_command(cmake_build_command) != 0:
        print("Project build failed.")
        sys.exit(1)

    print("Build successful.")

if __name__ == "__main__":
    main()
