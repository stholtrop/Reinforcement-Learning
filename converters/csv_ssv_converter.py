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
            numbers = line.split(',')[skip_column : (len(line.split(',')) if end_column is None else end_column)]
            columns = len(numbers)
            for number in numbers:
                data.append(number.strip())
    with open(filename_to, 'w') as f:
        f.write("{} {} ".format(rows, columns))
        for i in data:
            f.write(str(float(i)/normalization) + " ")
def output_split(filename_from, filename_to, skip_column=0, end_column=1):
    data = []
    rows = 0
    columns = 0
    with open(filename_from) as f:
        lines = f.readlines()
        rows = len(lines)
        for line in lines:
            numbers = line.split(',')[skip_column : (len(line.split(',')) if end_column is None else end_column)]
            columns = len(numbers)
            for number in numbers:
                data.append(number.strip())
    with open(filename_to, 'w') as f:
        f.write("{} {} ".format(rows, 10))
        for i in data:
            to_write = ['0']*10
            to_write[int(i)] = '1'
            f.write(' '.join(to_write) + ' ')

output_split("./data/mnist_test.csv", "./data/mnist_test_output.ssv")
csv_to_ssv("./data/mnist_test.csv", "./data/mnist_test_input.ssv", skip_column=1, end_column=None, normalization=255)
