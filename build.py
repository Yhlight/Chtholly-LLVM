import os
import subprocess
import sys

def run_command(command):
    """Runs a command and prints its output."""
    try:
        subprocess.run(command, check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error executing command: {' '.join(command)}")
        sys.exit(1)

def main():
    """Main function for the build script."""
    build_dir = "build"

    # Create build directory if it doesn't exist
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    # Configure the project with CMake
    run_command(["cmake", "-S", ".", "-B", build_dir])

    # Build the project
    run_command(["cmake", "--build", build_dir])

    print("\nBuild complete.")
    print(f"The executable is located at: {build_dir}/src/chtholly_cli")
    print(f"To run the tests, execute: {build_dir}/tests/chtholly_tests")


if __name__ == "__main__":
    main()
