import os
import subprocess

def run_command(command):
    """Runs a command and prints its output."""
    print(f"Executing: {' '.join(command)}")
    result = subprocess.run(command, capture_output=True, text=True)
    if result.stdout:
        print(result.stdout)
    if result.stderr:
        print(result.stderr)
    if result.returncode != 0:
        raise Exception(f"Command failed with exit code {result.returncode}")

def build():
    """Builds the project using CMake."""
    build_dir = "build"
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    cmake_command = ["cmake", "-B", build_dir]
    run_command(cmake_command)

    build_command = ["cmake", "--build", build_dir]
    run_command(build_command)

if __name__ == "__main__":
    build()
