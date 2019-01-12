import numpy as np
import sys
sys.path.append("./build/lib.linux-x86_64-3.7")
import game

def getboard():
    boards = game.getBoard()
    result = np.empty((len(boards), 8, 8))
    for index, i in enumerate(boards):
        result[index] = np.array(i).reshape((8,8))
    return result

def getboards(num):
    return np.array([getboard() for i in range(num)])