import os
import subprocess
import sys

def run_command(command):
    """Runs a command and prints its output."""
    print(f"Executing: {' '.join(command)}")
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    for line in iter(process.stdout.readline, ''):
        sys.stdout.write(line)
    process.wait()
    if process.returncode != 0:
        print(f"Error: Command failed with exit code {process.returncode}")
        sys.exit(process.returncode)

def main():
    """Main function to build and run the Chtholly compiler."""
    # Create build directory if it doesn't exist
    if not os.path.exists("build"):
        os.makedirs("build")

    # Change to build directory
    os.chdir("build")

    # Configure CMake
    run_command(["cmake", ".."])

    # Build the project
    run_command(["cmake", "--build", "."])

    # Run the executable
    if sys.platform == "win32":
        run_command([".\\chthollyc.exe"])
    else:
        run_command(["./chthollyc"])

if __name__ == "__main__":
    main()
