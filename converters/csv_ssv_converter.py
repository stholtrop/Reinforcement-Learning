"""
CSV to SSV converter and vice versa
"""
import argparse

def csv_to_ssv(filename_from, filename_to, skip_column, normalization, end_column):
    data = []
    rows = 0
    columns = 0
    with open(filename_from) as f:
        lines = f.readlines()
        rows = len(lines)
        for line in lines:
            numbers = line.split(',')
            columns = len(numbers)
            for number in numbers[skip_column: len(numbers) if end_column is None else end_column]:
                data.append(number.strip())
    with open(filename_to, 'w') as f:
        f.write("{} {} ".format(rows, columns))
        for i in data:
            f.write(str(float(i)/normalization) + " ")

csv_to_ssv("./data/mnist_test.csv", "./data/mnist_test_input.ssv", skip_column=1, normalization=255, end_column=None)
