from flask import Flask, render_template, jsonify
import subprocess
import re

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')  # Assuming you have an index.html file

def run_and_parse_cpp_program(cpp_output):
    pattern = r"Average (.*?) Time: (.*?) seconds."
    results = re.findall(pattern, cpp_output)

    # Convert results to a dictionary
    parsed_results = {name: float(time) for name, time in results}

    return parsed_results

@app.route('/get_results')
def get_results():
    try:
        # Run the compiled C++ program and capture its output
        result = subprocess.run(['./1'], stdout=subprocess.PIPE, check=True)
        output = result.stdout.decode('utf-8')

        # Parse the output and return as JSON
        parsed_data = run_and_parse_cpp_program(output)
        return jsonify(parsed_data)
    except Exception as e:
        # Handle errors (e.g., subprocess failure, parsing errors)
        return jsonify({"error": str(e)})

if __name__ == '__main__':
    app.run(debug=True)


