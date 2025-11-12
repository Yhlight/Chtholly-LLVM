import os
import subprocess
import sys

def run_command(command, cwd):
    """Runs a command in a specified directory and checks for errors."""
    print(f"Running command: {' '.join(command)}")
    result = subprocess.run(command, cwd=cwd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"Error executing command: {' '.join(command)}", file=sys.stderr)
        print(result.stdout)
        print(result.stderr)
        sys.exit(1)
    print(result.stdout)
    return result

def main():
    """Main function to build and run the Chtholly compiler."""
    # Create build directory if it doesn't exist
    build_dir = "build"
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    # Configure the project with CMake
    cmake_command = ["cmake", ".."]
    run_command(cmake_command, cwd=build_dir)

    # Build the project
    build_command = ["cmake", "--build", "."]
    run_command(build_command, cwd=build_dir)

    # Run the executable
    executable_path = os.path.join(build_dir, "chtholly")
    if os.path.exists(executable_path):
        print("\n--- Running Chtholly ---")
        run_command([executable_path], cwd=".")
    else:
        print(f"Error: Executable '{executable_path}' not found.", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()
