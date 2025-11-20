import os
import subprocess
import sys

def run_command(command, cwd=None):
    """Runs a command and prints its output in real-time."""
    print(f"Executing: {' '.join(command)}")
    process = subprocess.Popen(
        command,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        cwd=cwd,
        bufsize=1,
        universal_newlines=True,
    )
    for line in process.stdout:
        print(line, end='')
    process.wait()
    if process.returncode != 0:
        print(f"Error: Command failed with exit code {process.returncode}")
        sys.exit(process.returncode)

def main():
    """Main function to configure and build the project."""
    build_dir = "build"

    # Create build directory if it doesn't exist
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    # Configure the project with CMake
    cmake_configure_command = ["cmake", "-S", ".", "-B", build_dir]
    run_command(cmake_configure_command)

    # Build the project
    cmake_build_command = ["cmake", "--build", build_dir]
    run_command(cmake_build_command)

if __name__ == "__main__":
    main()
