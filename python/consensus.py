# Authors: Armin Toepfer and Jason Chin

from __future__ import print_function
from keras.models import Sequential
from keras.layers import Dense, Activation, Dropout, TimeDistributed, Convolution1D, Conv2D
from keras.layers import LSTM, GRU
from keras.optimizers import RMSprop
from keras.utils.data_utils import get_file
from keras.models import load_model
from keras.layers import Dense, Dropout, Flatten
import glob
import os
import numpy as np
import random
import sys

learn = True

# cut the text in semi-redundant sequences of region_size characters
region_size = 21

train_size = 1000
test_size = 1000
ver_size = 230

col = 6

# input_num_units = col * region_size
# hidden_num_units = 50
# output_num_units = 2

X = np.zeros((train_size, region_size, col), dtype=np.float32)
Y = np.zeros((train_size, 2), dtype=np.bool)

X_test = np.zeros((test_size, region_size, col), dtype=np.float32)
Y_test = np.zeros((test_size, 2), dtype=np.bool)

X_ver = np.zeros((ver_size, region_size, col), dtype=np.float32)
Y_ver = np.zeros((ver_size, 2), dtype=np.bool)

ref_base = {}
ref_base_test = {}

basemap = dict(zip("ACGT- N", range(0, 7)))


def fill(f, m, n, t, hit):
    i = 0
    for row in f:
        row = row.strip().split()
        m[t, i, :] = [float(row[0]), float(row[1]), float(row[2]),
                      float(row[3]), float(row[4]), float(row[5])]
        i += 1
    n[t, int(hit)] = True


if learn:
    def lstm_model():
        model = Sequential()
        # model.add(GRU(10, input_shape=(region_size, col), return_sequences=True))
        # model.add(Convolution1D(100, 26, border_mode='valid', activation='relu', input_shape=(region_size, col)))
        # model.add(GRU(128, return_sequences=True, input_shape=(region_size, col)))
        # model.add(LSTM(input_length=num_reads, input_dim=region_size,
        # output_dim=num_reads * region_size, return_sequences=True))

        model.add(LSTM(region_size * col, return_sequences=True,
                       input_shape=(region_size, col)))  # , dropout_W=0.2, dropout_U=0.2))
        model.add(Dropout(0.25))
        model.add(Flatten())
        # model.add(LSTM(100))
        model.add((
            Dense(2, activation='sigmoid')))
        model.compile(loss='categorical_crossentropy',
                      optimizer='adam', metrics=['accuracy'])
        return model

print("Start parsing")
t = 0
for file in glob.glob("/Users/atoepfer/tf_data/train/mix*"):
    # sys.stdout.write('.')
    # sys.stdout.flush()
    with open(file) as f:
        if t < train_size:
            fill(f, X, Y, t, file.endswith("1"))
        elif t - train_size < test_size:
            fill(f, X_test, Y_test, t - train_size, file.endswith("1"))
        t += 1
t = 0
for file in glob.glob("/Users/atoepfer/tf_data/ver/mix*"):
    with open(file) as f:
        fill(f, X_ver, Y_ver, t, file.endswith("1"))
        t += 1

print("End parsing")
if learn:
    model = lstm_model()
    print(model.summary())
    trained_model = model.fit(X, Y, epochs=10,
                              batch_size=10, validation_data=(X_test, Y_test))

    model.save("model.h5")
else:
    model = load_model("model.h5")

i = 0
tp = 0
ap = 0
preds = model.predict_classes(X_ver, verbose=0)
for p in preds:
    truth = int(Y_ver[i, 1])
    if truth == p:
        tp += 1
    else:
        print(Y_ver[i, 1], p)
    ap += 1
    i += 1
print(tp, ap, tp / ap)
