import os
import subprocess
import sys

def run_command(command):
    """Runs a command and prints its output in real-time."""
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    for line in process.stdout:
        print(line, end='')
    process.wait()
    return process.returncode

def main():
    # Create build directory if it doesn't exist
    if not os.path.exists("build"):
        os.makedirs("build")

    # Change to build directory
    os.chdir("build")

    # Configure the project with CMake
    print("--- Configuring CMake ---")
    if run_command(["cmake", ".."]) != 0:
        print("CMake configuration failed.")
        sys.exit(1)

    # Build the project
    print("\n--- Building Project ---")
    if run_command(["cmake", "--build", "."]) != 0:
        print("Build failed.")
        sys.exit(1)

    # Find and run the executable
    print("\n--- Running Executable ---")
    executable_name = "chtholly"  # As defined in src/CMakeLists.txt
    if os.path.exists(executable_name):
        run_command(["./" + executable_name])
    else:
        print(f"Executable '{executable_name}' not found.")

    # Run tests
    print("\n--- Running Tests ---")
    test_executable_name = "chtholly_tests"
    if os.path.exists(test_executable_name):
        if run_command(["./" + test_executable_name]) != 0:
            print("Tests failed.")
            sys.exit(1)
    else:
        print(f"Test executable '{test_executable_name}' not found.")

if __name__ == "__main__":
    main()
