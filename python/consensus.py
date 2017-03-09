# Authors: Armin Toepfer and Jason Chin

from __future__ import print_function
from keras.models import Sequential
from keras.layers import Dense, Activation, Dropout, TimeDistributed
from keras.layers import LSTM, GRU
from keras.optimizers import RMSprop
from keras.utils.data_utils import get_file
from keras.regularizers import l1l2, l1, l2, activity_l2, activity_l1l2, activity_l1
import numpy as np
import random
import sys

# cut the text in semi-redundant sequences of maxlen characters
maxlen = 50
train_size = 500
test_size = 100
ver_size = 100

X = np.zeros((train_size, maxlen, 5), dtype=np.uint8)
y = np.zeros((train_size, maxlen, 5), dtype=np.bool)

X_test = np.zeros((test_size, maxlen, 5), dtype=np.uint8)
y_test = np.zeros((test_size, maxlen, 5), dtype=np.bool)

X_ver = np.zeros((ver_size, maxlen, 5), dtype=np.uint8)
y_ver = np.zeros((ver_size, maxlen, 5), dtype=np.bool)
ref_base = {}
ref_base_test = {}

def fill_matrix(M, t, i, d):
    M[t, i, :] = d

with open("test") as f:
    t = 0
    i = 0
    for row in f:
        row = row.strip().split()

        d = [ int(float(row[0]) * float(row[5])),
               int(float(row[1]) * float(row[5])),
               int(float(row[2]) * float(row[5])),
               int(float(row[3]) * float(row[5])),
               int(float(row[4]) * float(row[5])) ]

        if t < train_size:
            X[t, i, :] = d
            y[t, i, int(row[6])] = 1
        elif t < train_size + test_size:
            X_test[t-train_size, i, :] = d
            y_test[t-train_size, i, int(row[6])] = 1
            ref_base_test[(t, i)] = row[6]
        elif t < train_size + test_size + ver_size:
            X_ver[t-train_size-ver_size, i, :] = d
            y_ver[t-train_size-ver_size, i, int(row[6])] = 1
            ref_base_test[(t, i)] = row[6]
        else:
            break

        i += 1
        if i == 50:
            i = 0
            t += 1

# build the model: a single LSTM
print('Build model...')
model = Sequential()
model.add(GRU(24, input_shape=(maxlen, 5), return_sequences=True))
# model.add(LSTM(24, input_shape=(maxlen, 5), return_sequences=True))
model.add(Dropout(0.25))
# model.add(TimeDistributed(Dense(5, activation='softmax')))
model.add(TimeDistributed(Dense(5, activation='softmax')))#, input_dim=5, W_regularizer=l1l2(l1=0.01, l2=0.01), activity_regularizer=activity_l1l2(l1=0.01, l2=0.01))))
#model.add(Activation('softmax'))
model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])


# train the model, output generated text after each iteration
for iteration in range(1, 50):
    print()
    print('-' * 50)
    print('Iteration', iteration)
    model.fit(X, y, verbose=2, validation_data=(X_test,y_test), batch_size=20, nb_epoch=1)

tp = 0
ap = 0
t = train_size + ver_size
preds = model.predict_classes(X_ver, verbose=0)
for p in preds:
    i = 0
    for pp in p:
        if int(ref_base_test[(t,i)]) == int(pp):
            tp += 1;
        ap += 1
        i+=1
    t+=1

print(tp,ap,tp/ap)