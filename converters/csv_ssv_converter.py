"""
CSV to SSV converter and vice versa
"""
import argparse

def csv_to_ssv(filename_from, filename_to):
    data = []
    rows = 0
    columns = 0
    with open(filename_from) as f:
        lines = f.readlines()
        rows = len(lines)
        for line in lines:
            numbers = line.split(',')
            columns = len(numbers)
            for number in numbers:
                data.append(number.strip())
    with open(filename_to, 'w') as f:
        f.write("{} {} ".format(rows, columns))
        for i in data:
            f.write(i + " ")

csv_to_ssv("./data/mnist_train.csv", "./data/mnist_train.ssv")
