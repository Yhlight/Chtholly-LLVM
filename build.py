import os
import subprocess
import sys

def run_command(command):
    """Runs a command and checks for errors."""
    try:
        subprocess.run(command, check=True, shell=True)
    except subprocess.CalledProcessError as e:
        print(f"Error running command: {command}", file=sys.stderr)
        print(e, file=sys.stderr)
        sys.exit(1)

def main():
    """Builds and runs the Chtholly compiler."""
    # Create build directory if it doesn't exist
    if not os.path.exists("build"):
        os.makedirs("build")

    # Change to build directory
    os.chdir("build")

    # Run CMake
    run_command("cmake ..")

    # Build the project
    run_command("cmake --build .")

    # Run the executable
    executable_name = "chtholly"  # This should match the target name in CMakeLists.txt
    if os.name == 'nt':
        executable_name += ".exe"

    executable_path = os.path.join(os.getcwd(), executable_name)

    if os.path.exists(executable_path):
        print(f"--- Running {executable_name} ---")
        run_command(f"./{executable_name}")
        print(f"--- {executable_name} finished ---")
    else:
        print(f"Executable {executable_name} not found.", file=sys.stderr)
        sys.exit(1)

    # Change back to the root directory
    os.chdir("..")

if __name__ == "__main__":
    main()
