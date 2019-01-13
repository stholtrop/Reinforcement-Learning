from keras.models import Sequential
from keras.layers import Dense
import numpy as np
from gamenumpy import getboards

data = getboards(100)


model = Sequential()
model.add(Dense(32, input_dim=64, activation="sigmoid"))
model.add(Dense(1, activation="linear"))

model.compile()

