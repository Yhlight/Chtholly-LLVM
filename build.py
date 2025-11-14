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
    """Builds the Chtholly compiler."""
    # Create build directory if it doesn't exist
    if not os.path.exists("build"):
        os.makedirs("build")

    # Change to build directory
    os.chdir("build")

    # Run CMake
    print("--- Configuring CMake ---")
    run_command("cmake ..")

    # Build the project
    print("\n--- Building Project ---")
    run_command("cmake --build .")

    print("\n--- Build Complete ---")
    print("Executable is at: build/chtholly")

    # Change back to the root directory
    os.chdir("..")

if __name__ == "__main__":
    main()
