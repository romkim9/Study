import numpy as np
from tensorflow import keras
from tensorflow.keras.layers import Conv2D,MaxPooling2D,Dense,Flatten,Dropout
from tensorflow.keras.callbacks import TensorBoard,EarlyStopping,ModelCheckpoint
from tensorflow.keras.models import Sequential
import kerastuner as kt
from tensorflow import keras
import os

np_load_old = np.load
np.load = lambda *a,**k: np_load_old(*a, allow_pickle=True, **k)

X_train, X_test, Y_train, Y_test = np.load('./mask.npy')

X_train = X_train.astype(float) / 255.0
X_test = X_test.astype(float) / 255.0

model_dir = './model'
log_dir = './log'

if not os.path.exists(model_dir):
    os.mkdir(model_dir)
if not os.path.exists(log_dir):
    os.mkdir(log_dir)

Input_Shape = X_train.shape[1:]
print(Input_Shape)
Count_Of_Categories = 2
print(Input_Shape)

def build(hp):

    model = Sequential()
    model.add(Conv2D(hp.Int('L1_Conv2D_Filters',min_value=8,max_value=64,step=4)
                     ,kernel_size=(5, 5), padding="same", input_shape=Input_Shape, activation='relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Dropout(0.25))

    model.add(Conv2D(hp.Int('L2_Conv2D_Filters',min_value=8,max_value=64,step=4)
                     ,kernel_size= (5, 5), padding="same", activation='relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Dropout(0.25))

    model.add(Flatten())
    model.add(Dense(hp.Int("L3_Dense_num",min_value=4,max_value=216,step=4)
                    , activation='relu'))
    model.add(Dropout(0.25))
    model.add(Dense(Count_Of_Categories, activation='softmax'))
    model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])


    return model

tuner = kt.RandomSearch(build,
                        objective = 'val_loss',
                        max_trials =50,
                        directory="log",
                        project_name="test")

tuner.search_space_summary()
tuner.search(X_train, Y_train, epochs = 10, validation_data = (X_test, Y_test))

get_best_model = tuner.get_best_hyperparameters()[0]
tuner.results_summary()

model = tuner.hypermodel.build(get_best_model)
checkpoint = ModelCheckpoint(filepath='./model/test.model' , monitor='val_loss', verbose=1, save_best_only=True)
early_stopping = EarlyStopping(monitor='val_loss', mode='min',patience=1)
tensorboard_ = TensorBoard(log_dir=log_dir)
model.fit(X_train, Y_train, batch_size=32, epochs=100, validation_data=(X_test, Y_test), callbacks=[checkpoint, early_stopping, tensorboard_])

print("result: %.4f" % (model.evaluate(X_test, Y_test)[1]))