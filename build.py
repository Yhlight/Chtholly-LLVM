import os
import subprocess
import sys

def run_command(command, cwd=None):
    """Runs a command and checks for errors."""
    print(f"Executing: {' '.join(command)}")
    result = subprocess.run(command, cwd=cwd, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"Error running command: {' '.join(command)}", file=sys.stderr)
        print(result.stdout, file=sys.stdout)
        print(result.stderr, file=sys.stderr)
        sys.exit(1)
    print(result.stdout)
    return result

def main():
    """Main function to build the project."""
    build_dir = "build"

    # Create build directory if it doesn't exist
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)

    # Configure the project with CMake
    run_command(["cmake", "-S", ".", "-B", build_dir])

    # Build the project
    run_command(["cmake", "--build", build_dir])

    # --- Optional: Run the executable ---
    cli_executable = os.path.join(build_dir, "src", "chtholly_cli")
    if os.path.exists(cli_executable):
        print(f"Build successful! You can find the executable at: {cli_executable}")
        # Example of how to run it:
        # if len(sys.argv) > 1:
        #     run_command([cli_executable, sys.argv[1]])
    else:
        print("Build finished, but the main executable was not found.", file=sys.stderr)


if __name__ == "__main__":
    main()
