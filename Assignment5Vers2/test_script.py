import subprocess

def run_calculator(input_data):
    process = subprocess.Popen(
        ['Assignment5Vers2.exe'],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    stdout, stderr = process.communicate(input_data)
    return stdout.strip(), stderr.strip()

def test_calculator():
    tests = [
        {
            "input": "1+4\nmax(4, min(2, 3))\nexit",
            "expected_output": "Enter an arithmetic expression or 'exit' to leave: >>> 5\n"
                               "Enter an arithmetic expression or 'exit' to leave: >>> 4\n"
                               "Enter an arithmetic expression or 'exit' to leave:"
        },
        {
            "input": "12345-345\nexit",
            "expected_output": "Enter an arithmetic expression or 'exit' to leave: >>> 12000\n"
                               "Enter an arithmetic expression or 'exit' to leave:"
        },
        {
            "input": "max(4, min(2, 3))\nexit",
            "expected_output": "Enter an arithmetic expression or 'exit' to leave: >>> 4\n"
                               "Enter an arithmetic expression or 'exit' to leave:"
        },
        {
            "input": "-pow(3, 2) + (4 * (abs(-5) - 3)) / min(2, 3)\nexit",
            "expected_output": "Enter an arithmetic expression or 'exit' to leave: >>> -5\n"
                               "Enter an arithmetic expression or 'exit' to leave:"
        },
        {
            "input": "(3 + 5 * (2 - 1) + 4) / 2 + (7 * (4 / 2))\nexit",
            "expected_output": "Enter an arithmetic expression or 'exit' to leave: >>> 20\n"
                               "Enter an arithmetic expression or 'exit' to leave:"
        },
        {
            "input": "pow(2, 4) + (8 * 5 - (20 / 4) + 4) * 12 - (7 * (4 / 2))\nexit",
            "expected_output": "Enter an arithmetic expression or 'exit' to leave: >>> 470\n"
                               "Enter an arithmetic expression or 'exit' to leave:"
        }
    ]
    for test in tests:
        input_data = test["input"]
        expected_output = test["expected_output"]

        output, error = run_calculator(input_data)

        if error:
            print(f"Test failed for input:\n{input_data}")
            print(f"Error:\n{error}")
        elif output.strip() != expected_output:
            print(f"Test failed for input:\n{input_data}")
            print(f"Expected:\n{expected_output}")
            print(f"Got:\n{output}")
        else:
            print(f"Test passed for input:\n{input_data}")

if __name__ == "__main__":
    test_calculator()
